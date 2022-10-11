//#include <ArduinoJson.h>

int motion=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(9,INPUT);
  pinMode(8,INPUT);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
//  DynamicJsonBuffer jBuffer;
//  JsonObject& root = jBuffer.createObject();
  if((digitalRead(8)==1)||(digitalRead(9)==1)){
    motion = 1;
    digitalWrite(13,HIGH);
    Serial.println("motion:"+String(1));
    delay(7000);
    }
    else{
      motion=0;
      digitalWrite(13,LOW);
      Serial.println("motion:"+String(0));
      delay(2000);
      }

  //root["motion"] = motion;
  //root.prettyPrintTo(Serial);
  //Serial.println();
  //delay(3000);
//  if(motion==1){
//    Serial.println("{\"motion\":"+String(motion)+"}");
//    delay(7000);
//    }else{
//      Serial.println("{\"motion\":"+String(motion)+"}");
//      }

// if(motion==1){
//    Serial.println("motion:"+String(motion));
//    delay(7000);
//    }else{
//      Serial.println("motion:"+String(motion));
//      }
  
  
}
