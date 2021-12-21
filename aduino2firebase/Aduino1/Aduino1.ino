#include <SoftwareSerial.h>

SoftwareSerial mySerial(4,5);
void uploadInfo(int fillData, int airData) {
  char buffer[4];
    
  Serial.print("F");
  sprintf(buffer, "%03d", fillData);
  Serial.print(buffer);
  
  Serial.print("A");
  sprintf(buffer, "%04d", airData);
  Serial.print(buffer);

  Serial.println();
}

int lastFillData = 0, lastAirData = 0;

int curAirData = 10, curFillData = 10;

String sTime = "246060";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
   Serial.println('T');
}

void loop() {
if (Serial.available()){
  sTime = Serial.readStringUntil('\n');
}

 int len = sTime.length()+1;
 char cTime[len];
 sTime.toCharArray(cTime, len);
 mySerial.write(cTime);
 mySerial.write('\n');
 
 //for test
 //if (curAirData<=1024)
 // curAirData += 10;
 //if (curFillData <= 100)
 // curFillData += 10;
  
  
 if(lastFillData != curFillData || lastAirData != curAirData) {
   lastFillData = curFillData;
   lastAirData = curAirData;
   uploadInfo(curFillData, curAirData);
 }
 delay(1000);
}
