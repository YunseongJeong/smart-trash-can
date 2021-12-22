#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(4,5);
Servo myServo1, myServo2;
int servoPin[] = {11, 12};
int open_angle = 30; // when open
int shut_angle = 100; // when shut down
bool isopen = false;
void openS(){
  Serial.println("tlqkf0");
      for (int i = shut_angle; i >= open_angle; i--) {
      myServo1.write(i-15);
      myServo2.write(i+15);
      delay(25);
    }
    isopen = true;
}
void closeS(){
  Serial.println("tlqkf2");
      for (int i = open_angle; i <= shut_angle; i++) {
      myServo1.write(i-15);
      myServo2.write(i+15);
      delay(25);
    }
    isopen = false;
}

void servo_start() {
  /*
     @ brief : Operate basic function for using servo motor
     @ parameter : int i, The motor operates as many as i.1<= i<=3
     @ return : none
  */
    myServo1.attach(11);
    myServo1.write(shut_angle-15);
    myServo2.attach(12);
    myServo2.write(shut_angle+15);
}

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
bool timeIsCome = false;
int curAirData = 10, curFillData = 10;

String sTime = "000000";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial1.begin(9600);
  Serial.println('T');

  servo_start();
}

void loop() {
  if (Serial.available()){
    sTime = Serial.readStringUntil('\n');
    timeIsCome = true;
  }

  if (Serial1.available()){
    String sData = Serial1.readStringUntil('\n');
    Serial.println(sData);
    if (sData == "O" && !isopen){
      openS();
    } else if (sData =="C" && isopen){
      closeS();
    } else {
      long int iData = sData.toInt();
      curAirData = iData/10000;
      curFillData = iData%10000;
    }
  }
  
  if (timeIsCome){ 
    int len = sTime.length()+1;
    char cTime[len];
    sTime.toCharArray(cTime, len);
    mySerial.write(cTime);
    mySerial.write('\n');
  }
  
  if(lastFillData != curFillData || lastAirData != curAirData) {
    lastFillData = curFillData;
    lastAirData = curAirData;
    uploadInfo(curFillData, curAirData);
  }
 
 delay(1000);
}
