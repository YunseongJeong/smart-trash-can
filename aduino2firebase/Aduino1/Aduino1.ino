void uploadInfo(int fillData, int airData) {
  char buffer[4];
    
  Serial.print("F");
  sprintf(buffer, "%03d", fillData);
  Serial.print(buffer);
  
  Serial.print("A");
  sprintf(buffer, "%03d", airData);
  Serial.print(buffer);

  Serial.println();
}

int lastFillData = 0, lastAirData = 0;

int curAirData = 70, curFillData = 20;

void setup() {
  Serial.begin(9600);
}

void loop() {
 if(lastFillData != curFillData || lastAirData != curAirData) {
   lastFillData = curFillData;
   lastAirData = curAirData;
   uploadInfo(curFillData, curAirData);
 }
 delay(1000);
}
