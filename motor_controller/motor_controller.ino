#include <Servo.h>
#define minute 60000
// for time check 
unsigned long time_previous, time_current;
boolean isopen = false;

Servo myServo1, myServo2, myServo3; // If there is not enough power, we need a lot motor
int num_motor = 1; // number of motors we use
int servoPin[] = {11, 12, 13};
int open_angle = 90; // when open
int shut_angle = 0; // when shut down 


int motionPin = 8;

int trigPin = 3;
int echoPin = 2;

int lumiPin = 54;
int ledPin = 5;

int read_lumi(int lumiPin){
    /*
     * @ brief: Get int value(0~255) of output of light sensor
     * @ parameter: pin number of light sensor
     * @ return: return int value
     */
    int reading = analogRead(lumiPin);
    Serial.println(reading);
    int value = map(reading, 0, 1024, 0, 255); 
    return value;
}

float get_duration(int trigPin, int echoPin){
    /*
     *@ brief : Get distance from a something by using ultrasonic sensor
     *@ parameter : trigPin number, echoPin number 
     *@ return : (float)length from a something
     */
    digitalWrite(trigPin,HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    float duration = pulseIn(echoPin, HIGH); 

    float distance = duration*340/10000/2; // 이거 함수로 묶자.
    return distance;
}

void servo_start(int i){
    /*
     * @ brief : Operate basic function for using servo motor
     * @ parameter : int i, The motor operates as many as i.1<= i<=3
     * @ return : none
     */
     if (i==1){
        myServo1.attach(servoPin[0]);
        myServo1.write(shut_angle);
     }
     else if (i==2){
        myServo1.attach(servoPin[0]);
        myServo1.write(shut_angle);
        myServo2.attach(servoPin[1]);
        myServo2.write(shut_angle);
     }
     else if (i==3){
        myServo1.attach(servoPin[0]);
        myServo1.write(shut_angle);
        myServo2.attach(servoPin[1]);
        myServo2.write(shut_angle);
        myServo3.attach(servoPin[2]);
        myServo3.write(shut_angle);
     }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(motionPin, INPUT); // motion detect
    
    // distance sensor
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
    // servo motor
    servo_start(num_motor);

    // time checker
    time_previous=millis();

    // led
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

void loop() {
    // put your main code here, to run repeatedly:
    int led_value = read_lumi(lumiPin);
    Serial.println(led_value);
    analogWrite(ledPin, led_value);
    time_current = millis();
    int motion = digitalRead(motionPin);
    if(motion == HIGH){

        float distance = get_duration(trigPin, echoPin);
        Serial.println("Motion detected!");
        if(distance < 50){
            // Open Intrance
            Serial.println("Open!");
            myServo1.write(open_angle);
            isopen = true;
            time_previous = millis(); // set open time.
        }
    }
    else{
        Serial.println("Not detected!");
    }

    if(!motion && (isopen && (time_current - time_previous >= minute/12))){
        // if there is no motion and it takes 1 minute from open.
        isopen = false;
        time_previous = 0; 
        myServo1.write(shut_angle);  
        Serial.println("Close");      
    }

    delay(1000);
}
