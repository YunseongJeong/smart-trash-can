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

void setup() {
  Serial.begin(9600);
}

void loop() {
 uploadInfo(70, 40);
 delay(1000);
}
