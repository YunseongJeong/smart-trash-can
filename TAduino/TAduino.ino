#include <SoftwareSerial.h>
#include <Servo.h>

#define minute 60000

// for time check
unsigned long time_previous, time_current;
boolean isopen = false;

SoftwareSerial mySerial(14, 15);
SoftwareSerial mySerial2(16, 17);

// 1. PIR & 초음파 거리 센서 & 압력센서
int PIR = 9;
int trigPin1 = A14; // 거리 재기용
int echoPin1 = A15;

int pSensor = 55; // 압력센서

int trigPin2 = 29; // 거리 재기용
int echoPin2 = 27;


// 2. 공기 품질 센서
int gSensor = 54;

// 3. 조도 센서 & LED
int LED[] = {6, 7, 8};
int lSensor = A2;



// ① 기능 수행
float get_duration(int trigPin, int echoPin) {
  /*
    @ brief : Get distance from a something by using ultrasonic sensor
    @ parameter : trigPin number, echoPin number
    @ return : (float)length from a something
  */
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);

  float distance = duration * 340 / 10000 / 2; // 이거 함수로 묶자.
  return distance;
}

// ⑤ 기능 수행
int read_lumi(int lSensor) {
  /*
     @ brief: Get int value(0~255) of output of light sensor
     @ parameter: pin number of light sensor
     @ return: return int value
  */
  int reading = analogRead(lSensor);  // 조도센서값은 밝으면 값이 커짐
  Serial.println(reading);
  int value = map(reading, 0, 1024, 255, 0);
  return value;
}

int read_pressure(int pSensor){
    /*
     * @ brief: Read pressure sensor
     * @ parameter: pSensor
     * @ return: 0~1023 int value
    */
    int pressure = analogRead(pSensor);
    return pressure;
}

int pressure_to_per(int pressure){
    /*
     * @ brief: translate pressure value to percent
     * @ parameter: int pressure value 0~1023
     * @ return: int precent value
    */
    float pressure_per = pressure / 1024.0 * 100.0;
    return (int)pressure_per;
}


void setup() {
  mySerial.begin(9600);
  mySerial2.begin(9600);

  Serial.begin(9600);
  pinMode(PIR, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW);
  }
}




int fillCan() {
  // ② 쓰레기통이 가득 차면 휴대폰에 알림을 보내는 기능
  int pValue = analogRead(pSensor);
  Serial.print("쓰레기 무게 : ");

  int pressure = read_pressure(A1);
  pressure = pressure_to_per(pressure);
  Serial.println(pressure);
  
  int distance = (int)get_duration(trigPin2, echoPin2);
  int full = map(distance,0,25,100,0);
  if (full<=0)
  full = 0;
  if (pressure <= 20)
    full = (int)(full * 0.9);
  Serial.println(distance);
  Serial.println(full);
  return full;
}

int smellCan() {
  // ③ 쓰레기통에 연기가 나거나 악취가 심하면 휴대폰에 알림을 보내는 기능
  int gValue = analogRead(gSensor);

  Serial.print("가스 센서값 : ");
  Serial.println(gValue);

  return gValue;
}

void communication(int gValue, int full) {
  // ④ LCD판에 정보 전달
  char buffer[9];
  sprintf(buffer, "%04d%04d", gValue, full);
  buffer[8] = '\0';
  Serial.println(buffer);

  mySerial.write(buffer);
  mySerial.write("\n");
  mySerial2.write(buffer);
  mySerial2.write("\n");
}

void turnLED() {
  // ⑤ 밤이 되면 LED가 켜짐
  int led_value = read_lumi(lSensor);
  Serial.print("LED 밝기 : ");
  Serial.println(led_value);
  if (led_value <= 70) {
      analogWrite(LED[0], 0);
      analogWrite(LED[1], 0);
      analogWrite(LED[2], 0);
  } else {
    analogWrite(LED[0], 255);
      analogWrite(LED[1], 255);
      analogWrite(LED[2], 255);
  }
}


void loop() {
  // ① 가까이 가면 열리는 기능
  time_current = millis();
  int motion = digitalRead(PIR);
  float distance1 = get_duration(trigPin1, echoPin1);
  Serial.print("tkqjf : ");
  Serial.println(distance1);
  if ((motion && distance1 < 50) && !isopen) { // 움직임이 감지된다면 PIR
    Serial.println("Motion detected!");

    // Open Intrance
    Serial.println("Open!");
    mySerial2.write("O");
    mySerial2.write("\n");
    delay(225);
    isopen = true;
    time_previous = millis(); // set open time.
    //}

  } else if ((isopen && distance1 >= 50)) {
    // if there is no motion and it takes 1 minute from open.
    // 닫힘 코드
    isopen = false;
    Serial.println("Close");
    delay(10);
    time_previous = millis();
    mySerial2.write("C");
    mySerial2.write("\n");
    delay(225);
    
  }
  if (!isopen) {
    communication(smellCan(), fillCan());
  }
  
  turnLED();

  delay(1000);
} 
