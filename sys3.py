#! /usr/bin/python2

import time
import sys
import spidev
global occupied
occupied = False
import os
import serial
import json
from threading import Thread

global motion
motion = 0

global i
global count 
i=0
count=0
irCount=0
halt=False

global perfumed
perfumed=False

global irDetect
irDetect = True

spi = spidev.SpiDev()
spi.open(0,0)
spi.max_speed_hz=1000000

ser = serial.Serial('/dev/ttyUSB_SERVO',9600, timeout = 5)

def readChannel(channel):
    val = spi.xfer2([1,(8+channel)<<4,0])
    data = ((val[1]&3) << 8) + val[2]
    return data

def ir():
    global i
    i=0
    while True:
        i=(readChannel(1)/1023.0)*3.3
        #print(i)

def motionUpdate():
    global motion
    motion = 0
    connection = serial.Serial("/dev/ttyUSB_MOTION", 9600,timeout=5)
    connection.reset_input_buffer()

    while True:
        data = connection.readline().decode("latin1","ignore")
        #print(data)
        #print(data.find('1'))
            
        if(data.find('0') != -1):
           motion=0
           
        elif(data.find('1') != -1):
           motion=1
           
        #print(motion)
        #try:
        #    dict = json.loads(data)
        #    #print(dict["motion"])
        #    motion=dict["motion"]
        #    #print(motion)
        #except json.JSONDecodeError as e:
        #    #print(data[data.index('1')])
        #    print(data.index('0'))
        #    print(data)

def playHi():
    global motion
    global hi
    hi = True
    motion = 0
    while True:
        
        if(motion==1 and hi == True):
            os.system("mpg321 -g 10 hi-new.mp3")

def sayit():
    global count
    count=0
    global say
    irDetect=True
    say=False
    while True:
        if say==True:
            os.system("mpg321 -g 10 tom-fraser-right.mp3")
            time.sleep(3)
            count=count+1

motion_thread=Thread(target=motionUpdate,)
motion_thread.start()
playHi_thread=Thread(target=playHi,)
playHi_thread.start()
thIr=Thread(target=ir,)
thIr.start()
thSay=Thread(target=sayit,)
thSay.start()





EMULATE_HX711=False

referenceUnit = 1

if not EMULATE_HX711:
    import RPi.GPIO as GPIO
    from hx711 import HX711
else:
    from emulated_hx711 import HX711

def cleanAndExit():
    print("Cleaning...")

    if not EMULATE_HX711:
        GPIO.cleanup()
        
    print("Bye!")
    sys.exit()

hx = HX711(5, 6)

# I've found out that, for some reason, the order of the bytes is not always the same between versions of python, numpy and the hx711 itself.
# Still need to figure out why does it change.
# If you're experiencing super random values, change these values to MSB or LSB until to get more stable values.
# There is some code below to debug and log the order of the bits and the bytes.
# The first parameter is the order in which the bytes are used to build the "long" value.
# The second paramter is the order of the bits inside each byte.
# According to the HX711 Datasheet, the second parameter is MSB so you shouldn't need to modify it.
hx.set_reading_format("MSB", "MSB")

# HOW TO CALCULATE THE REFFERENCE UNIT
# To set the reference unit to 1. Put 1kg on your sensor or anything you have and know exactly how much it weights.
# In this case, 92 is 1 gram because, with 1 as a reference unit I got numbers near 0 without any weight
# and I got numbers around 184000 when I added 2kg. So, according to the rule of thirds:
# If 2000 grams is 184000 then 1000 grams is 184000 / 2000 = 92.
hx.set_reference_unit(100)
#hx.set_reference_unit(referenceUnit)

hx.reset()

hx.tare()

print("Tare done! Add weight now...")

# to use both channels, you'll need to tare them both
#hx.tare_A()
#hx.tare_B()

while True:
    off = ser.readline().decode("latin1","ignore")
    #print(off)
    if(off.find('shutdown') != -1):
        os.system("sudo shutdown now")

    try:
        # These three lines are usefull to debug wether to use MSB or LSB in the reading formats
        # for the first parameter of "hx.set_reading_format("LSB", "MSB")".
        # Comment the two lines "val = hx.get_weight(5)" and "print val" and uncomment these three lines to see what it prints.
        
        # np_arr8_string = hx.get_np_arr8_string()
        # binary_string = hx.get_binary_string()
        # print binary_string + " " + np_arr8_string
        
        # Prints the weight. Comment if you're debbuging the MSB and LSB issue.
        val = hx.get_weight(5)
        #print(val)
        #kgVal = val/1000
        #print(hi)
        if val>=1500:
            #print(val)
            hi=False
            if perfumed==False:
                print("perfume")
                ser.write(b"t\n")
                time.sleep(2)
                while irDetect==True:
                    say=True
                    if count==4:
                        irDetect=False
                    if i <=1:
                        irCount=irCount+1
                        print(count)
                        if irCount>=300:
                            irDetect=False
                    else:
                        irCount=0
                print(i)
                irCount=0
                say=False
                print(count)
                if count<=3:
                    ser.write(b"p\n")
                    time.sleep(2)
                    os.system("mpg321 -g 10  tom_fraser_fragrance.mp3")
                count=0
                ser.write(b"n\n")
                halt=True
                perfumed=True
                while halt==True:
                    time.sleep(5)
                    halt=False
        else:
            perfumed=False
            hi=True
            irDetect=True 
            #occupied = True
            #os.system("mpg321 -g 10 tom_fraser_fragrance.mp3")
            #print("perfumed")
            #print(kgVal)
            #time.sleep(10)
        #else:
        #    occupied = False
        # To get weight from both channels (if you have load cells hooked up 
        # to both channel A and B), do something like this
        #val_A = hx.get_weight_A(5)
        #val_B = hx.get_weight_B(5)
        #print "A: %s  B: %s" % ( val_A, val_B )

        hx.power_down()
        hx.power_up()
        time.sleep(0.1)

    except (KeyboardInterrupt, SystemExit):
        cleanAndExit()
