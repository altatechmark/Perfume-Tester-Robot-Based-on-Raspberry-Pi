

#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial base(2, 4); // RX, TX
Servo right_shoulder_rotate;
Servo right_arm;
Servo left_shoulder_rotate;
Servo left_arm;
Servo left_hand;
Servo right_hand;
int baseCount=0;
float temp=0.00;
int count=0;
float av_temp=0.00;
int oncee=0;
int normal_count=0;
bool normal=false;
float temperature=0.00;
int once = 0;

int right_shoulder_rotate_pos = 70;//drawing 3.5A continously
int right_arm_pos=0;
int left_arm_pos=179;//drawing 170mA continously
int left_shoulder_rotate_pos=90;
int left_hand_pos=90;
int right_hand_pos=180;

int left_r_up=true;
int left_r_down=false;
int right_r_up=true;
int right_r_down=false;

bool left_a_up=true;
bool left_a_down=false;

bool right_a_up=true;
bool right_a_down=false;

int count_right_r_up;
int count_right_r_down;

bool stop=true;
bool left_shoulder_rotate_home=true;
bool right_shoulder_rotate_home=true;

bool left_arm_home=true;
bool right_arm_home=true;

bool dance=false;

bool right_arm_up=false;
bool left_arm_up=false;
bool right_shoulder_up=false;
bool left_shoulder_up=false;





void setup() {
  // put your setup code here, to run once:
pinMode(A0,OUTPUT);
pinMode(8,INPUT_PULLUP);
pinMode(7,INPUT_PULLUP);
digitalWrite(A0,HIGH);

Serial.begin(9600);

//Serial.println("start");

  left_hand.attach(6);
  left_hand.write(left_hand_pos);
  delay(1000);
  left_shoulder_rotate.attach(3);//start at 90 end at 0
  left_shoulder_rotate.write(left_shoulder_rotate_pos);
  delay(1000);
   left_arm.attach(5);
   left_arm.write(left_arm_pos);
   delay(1000);

     right_hand.attach(10);
   right_hand.write(right_hand_pos);
   
  
   right_shoulder_rotate.attach(9);
   right_shoulder_rotate.write(right_shoulder_rotate_pos);
   delay(1000);

   right_arm.attach(11);
   right_arm.write(right_arm_pos);
   delay(1000);

   base.begin(9600);
}

void loop() {
if(digitalRead(8)==0){ //shutdown
  Serial.println("0");
  }

if(digitalRead(7)==0){//motor
  digitalWrite(A0,LOW);
  }
  else{
    digitalWrite(A0,HIGH);
    }
 
   //Serial.println(temperature);
  // put your main code here, to run repeatedly:
  //Serial.println(left_arm_pos);
  if(dance==true){
  left_Shoulder_rotate_Up_down(0,stop);
  right_Shoulder_rotate_Up_down(160,stop);
  left_Arm_up_down(140,stop);
  right_Arm_up_down(40,stop);
  }
  if(right_arm_up==true){
    right_Arm_up(90,stop);
    }
   if(right_arm_up==false){
    right_Arm_down(90);
    }

   if(left_arm_up==true){
    left_Arm_up(90,stop);
    }

    if(left_arm_up==false){
    left_Arm_down(90);
    }

    if(right_shoulder_up==true){
      right_Shoulder_rotate_Up(160,stop);
      
      }
    if(right_shoulder_up==false){
      right_Shoulder_rotate_Down(160);
      }

    if(left_shoulder_up==true){
      left_Shoulder_rotate_Up(0,stop);
      }
    if(left_shoulder_up==false){
      left_Shoulder_rotate_Down(0);
      }
  
  
    if(Serial.available()>0){
    char ask=Serial.read();
    //Serial.println(ask);
    //Serial.println('c'); //rpi: "let me check your temperature"
    if(ask=='t'){ // for temperature
        dance=false;
        stop=false;
        left_shoulder_up=true;                
  }
  //delay(5000);
  
  
  
  if(ask=='n'){
  once=0;
     dance=false;
        stop=false;
        left_shoulder_up=false;
      
    }
    

  if(ask=='l'){
     dance=false;
        stop=false;
        right_shoulder_up=true;
  }

  if(ask=='o'){ // for temperature
      dance=false;
        stop=false;
        right_shoulder_up=false;
      }

   if(ask=='p'){
    digitalWrite(A0,LOW);
    delay(110);
    digitalWrite(A0,HIGH);
    }
   
    

 
}
}

void left_Shoulder_rotate_Up_down(int limit, bool halt){
  //Serial.println(left_shoulder_rotate_pos);
if((left_r_up)&&(halt==false)){
if(left_shoulder_rotate_pos>limit){
  left_shoulder_rotate_pos--;
  left_shoulder_rotate.write(left_shoulder_rotate_pos);
  delay(10);
  }
  if(left_shoulder_rotate_pos<=limit){
    left_r_up=false;
    left_r_down=true;
    }
  }  
if((left_r_down)&&(halt==false)){
  if(left_shoulder_rotate_pos>=limit){
    left_shoulder_rotate_pos++;
    left_shoulder_rotate.write(left_shoulder_rotate_pos);
    delay(10);
    
    }
    if(left_shoulder_rotate_pos==90){
      left_r_up=true;
    left_r_down=false;
      } 
  }

  if(halt){
    if(left_shoulder_rotate_home==true){
    if(left_shoulder_rotate_pos<=90){
      left_shoulder_rotate_pos++;
      left_shoulder_rotate.write(left_shoulder_rotate_pos);
      delay(10);
      if(left_shoulder_rotate_pos>=90){
        left_shoulder_rotate_home=false;
        left_r_up=true;
        left_r_down=false;
        }
      }
     
      
    }
  }
}

void right_Shoulder_rotate_Up_down(int limit, bool halt){//70 to 180
  //Serial.println(right_shoulder_rotate_pos);
if((right_r_up)&&(halt==false)){
 if(right_shoulder_rotate_pos<=limit){
  right_shoulder_rotate_pos++;
  right_shoulder_rotate.write(right_shoulder_rotate_pos);
  delay(10);
  
  }
  if(right_shoulder_rotate_pos>=limit){
    right_r_up=false;
    right_r_down=true;
    }
  
 
  }

  
if((right_r_down)&&(halt==false)){//180 to 70
  if(right_shoulder_rotate_pos<=limit){
    right_shoulder_rotate_pos--;
    right_shoulder_rotate.write(right_shoulder_rotate_pos);
    delay(10);
  }
  if(right_shoulder_rotate_pos==70){
    right_r_up=true;
    right_r_down=false;
    }
  
}

if (halt==true){
  if(right_shoulder_rotate_home==true){
  if(right_shoulder_rotate_pos>=70){
      right_shoulder_rotate_pos--;
      right_shoulder_rotate.write(right_shoulder_rotate_pos);
    delay(10);
    if(right_shoulder_rotate_pos==70){
      right_shoulder_rotate_home=false;
      right_r_up=true;
      right_r_down=false;
      }
      }
  }
  }
  
  }

void left_Arm_up_down(int limit, bool halt){
  //Serial.println(left_arm_pos);
if((left_a_up)&&(halt==false)){
if(left_arm_pos>limit){
  left_arm_pos--;
  left_arm.write(left_arm_pos);
  delay(10);
  }
  if(left_arm_pos<=limit){
    left_a_up=false;
    left_a_down=true;
    }
  }  
if((left_a_down)&&(halt==false)){
  if(left_arm_pos>=limit){
    left_arm_pos++;
    left_arm.write(left_arm_pos);
    delay(10);
    
    }
    if(left_arm_pos==179){
      left_a_up=true;
    left_a_down=false;
      } 
  }

  if(halt){
    if(left_arm_home==true){
    if(left_arm_pos<=179){
      left_arm_pos++;
      left_arm.write(left_arm_pos);
      delay(10);
      if(left_arm_pos>=179){
        left_arm_home=false;
        left_a_up=true;
        left_a_down=false;
        }
      }
     
      
    }
  }
}
  
void right_Arm_up_down(int limit, bool halt){
  //Serial.println(right_arm_pos);
if((right_a_up)&&(halt==false)){
if(right_arm_pos<limit){
  right_arm_pos++;
  right_arm.write(right_arm_pos);
  delay(10);
  }
  if(right_arm_pos>=limit){
    right_a_up=false;
    right_a_down=true;
    }
  }  
if((right_a_down)&&(halt==false)){
  if(right_arm_pos<=limit){
    right_arm_pos--;
    right_arm.write(right_arm_pos);
    delay(10);
    
    }
    if(right_arm_pos==0){
      right_a_up=true;
    right_a_down=false;
      } 
  }

  if(halt){
    if(right_arm_home==true){
    if(right_arm_pos>=0){
      right_arm_pos--;
      right_arm.write(right_arm_pos);
      delay(10);
      if(right_arm_pos<=0){
        right_arm_home=false;
        right_a_up=true;
        right_a_down=false;
        }
      }
     
      
    }
  }
}

void right_Arm_up(int limit, bool halt){
  //Serial.println(right_arm_pos);
if((right_a_up)&&(halt==false)){
if(right_arm_pos<limit){
  right_arm_pos++;
  right_arm.write(right_arm_pos);
  delay(10);
  }
  if(right_arm_pos>=limit){
    right_a_up=false;
    right_a_down=true;
    }
  }  
//if((right_a_down)&&(halt==false)){
//  if(right_arm_pos<=limit){
//    right_arm_pos--;
//    right_arm.write(right_arm_pos);
//    delay(10);
//    
//    }
//    if(right_arm_pos==0){
//      right_a_up=true;
//    right_a_down=false;
//      } 
//  }

  if(halt){
    if(right_arm_home==true){
    if(right_arm_pos>=0){
      right_arm_pos--;
      right_arm.write(right_arm_pos);
      delay(10);
      if(right_arm_pos<=0){
        right_arm_home=false;
        right_a_up=true;
        right_a_down=false;
        }
      }
     
      
    }
  }
} 

void right_Arm_down(int limit){
  if(right_a_down){
  if(right_arm_pos<=limit){
    right_arm_pos--;
    right_arm.write(right_arm_pos);
    delay(10);
    
    }
    if(right_arm_pos==0){
      right_a_up=true;
    right_a_down=false;
      } 
  }
  }

  void left_Arm_up(int limit, bool halt){
  //Serial.println(left_arm_pos);
if((left_a_up)&&(halt==false)){
if(left_arm_pos>limit){
  left_arm_pos--;
  left_arm.write(left_arm_pos);
  delay(10);
  }
  if(left_arm_pos<=limit){
    left_a_up=false;
    left_a_down=true;
    }
  }  


  if(halt){
    if(left_arm_home==true){
    if(left_arm_pos<=179){
      left_arm_pos++;
      left_arm.write(left_arm_pos);
      delay(10);
      if(left_arm_pos>=179){
        left_arm_home=false;
        left_a_up=true;
        left_a_down=false;
        }
      }
     
      
    }
  }
}

void left_Arm_down(int limit){
  if(left_a_down){
  if(left_arm_pos>=limit){
    left_arm_pos++;
    left_arm.write(left_arm_pos);
    delay(10);
    
    }
    if(left_arm_pos==179){
      left_a_up=true;
    left_a_down=false;
      } 
  }
  }

void right_Shoulder_rotate_Up(int limit, bool halt){//70 to 180
  //Serial.println(right_shoulder_rotate_pos);
if((right_r_up)&&(halt==false)){
 if(right_shoulder_rotate_pos<=limit){
  right_shoulder_rotate_pos++;
  right_shoulder_rotate.write(right_shoulder_rotate_pos);
  delay(10);
  }
  if(right_shoulder_rotate_pos>=limit){
    right_r_up=false;
    right_r_down=true;
    }
  
 
  }

  

if (halt==true){
  if(right_shoulder_rotate_home==true){
  if(right_shoulder_rotate_pos>=70){
      right_shoulder_rotate_pos--;
      right_shoulder_rotate.write(right_shoulder_rotate_pos);
    delay(10);
    if(right_shoulder_rotate_pos==70){
      right_shoulder_rotate_home=false;
      right_r_up=true;
      right_r_down=false;
      }
      }
  }
  }
  
  }

void right_Shoulder_rotate_Down(int limit){
if(right_r_down){//180 to 70
  if(right_shoulder_rotate_pos<=limit){
    right_shoulder_rotate_pos--;
    right_shoulder_rotate.write(right_shoulder_rotate_pos);
    delay(10);
  }
  if(right_shoulder_rotate_pos==70){
    right_r_up=true;
    right_r_down=false;
    }
  
}
}

void left_Shoulder_rotate_Up(int limit, bool halt){
  //Serial.println(left_shoulder_rotate_pos);
if((left_r_up)&&(halt==false)){
if(left_shoulder_rotate_pos>limit){
  left_shoulder_rotate_pos--;
  left_shoulder_rotate.write(left_shoulder_rotate_pos);
  delay(10);
  }
  if(left_shoulder_rotate_pos<=limit){
    left_r_up=false;
    left_r_down=true;
    }
  }  


  if(halt){
    if(left_shoulder_rotate_home==true){
    if(left_shoulder_rotate_pos<=90){
      left_shoulder_rotate_pos++;
      left_shoulder_rotate.write(left_shoulder_rotate_pos);
      delay(10);
      if(left_shoulder_rotate_pos>=90){
        left_shoulder_rotate_home=false;
        left_r_up=true;
        left_r_down=false;
        }
      }
     
      
    }
  }
}
void left_Shoulder_rotate_Down(int limit){
if(left_r_down){
  if(left_shoulder_rotate_pos>=limit){
    left_shoulder_rotate_pos++;
    left_shoulder_rotate.write(left_shoulder_rotate_pos);
    delay(10);
    
    }
    if(left_shoulder_rotate_pos==90){
      left_r_up=true;
    left_r_down=false;
      } 
  }
}
