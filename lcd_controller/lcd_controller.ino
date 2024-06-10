#include <Adafruit_TFTLCD.h>
#include <pin_magic.h>
#include <registers.h>
#include <stdint.h>

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#define BLACK 0x0000 /* 0, 0, 0 */
#define NAVY 0x000F /* 0, 0, 128 */
#define DARKGREEN 0x03E0 /* 0, 128, 0 */
#define DARKCYAN 0x03EF /* 0, 128, 128 */
#define MAROON 0x7800 /* 128, 0, 0 */
#define PURPLE 0x780F /* 128, 0, 128 */
#define OLIVE 0x7BE0 /* 128, 128, 0 */
#define LIGHTGREY 0xC618 /* 192, 192, 192 */
#define DARKGREY 0x7BEF /* 128, 128, 128 */
#define BLUE 0x001F /* 0, 0, 255 */
#define GREEN 0x07E0 /* 0, 255, 0 */
#define CYAN 0x07FF /* 0, 255, 255 */
#define RED 0xF800 /* 255, 0, 0 */
#define MAGENTA 0xF81F /* 255, 0, 255 */
#define YELLOW 0xFFE0 /* 255, 255, 0 */
#define WHITE 0xFFFF /* 255, 255, 255 */
#define ORANGE 0xFDA0 /* 255, 180, 0 */
#define GREENYELLOW 0xB7E0 /* 180, 255, 0 */
#define PINK 0xFC9F
#define BACKGROUND BLACK
#define GOOD BLUE
#define NORMAL GREEN
#define BAD ORANGE
MCUFRIEND_kbv tft;

int signal_1 = 12;
uint8_t now_face = 1; // 1 = good, 2 = normal, 3 = bad
uint32_t data_int = 0;
boolean get_time = false;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
   Serial1.begin(9600);
  
  tft.reset();
  tft.begin();
  tft.setRotation(2);

  drawBorder();
  // 320 x 240
  tft.setCursor(46, 50);
  tft.setTextSize(2);
  tft.setTextColor(BLUE);
  tft.println("Auto TrashCan");
  tft.setCursor(30, 85);
  tft.setTextSize(1);
  tft.setTextColor(BLACK);
  tft.setCursor(117, 70);
  tft.println("made by UnJukJung");

  tft.setCursor(30, 180);
  tft.println("please waiting for operating...");
  tft.setCursor(10, 250);
  tft.print("|");
  tft.setCursor(220, 250);
  tft.print("|");

  tft.setCursor(15, 250);
  feellike_delaying(34);
  delay(1000);
    

  print_info(1, data_int);
}

void loop() {
  // put your main code here, to run repeatedly:
  data_int+=1;
 
  // signal through uart
   //test code of UART 
   if(!get_time){
    if(Serial1.available()){
        String data = Serial1.readStringUntil('\n');
        data_int = data.toInt();
        data_int += 4;
        get_time = true;
    }
   }
 
  if (signal_1) {
    show_status(16);
  }
  
  data_int = print_clock(data_int);
  delay(1000);
}

void drawBorder () {

  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;

  tft.fillScreen(DARKGREEN);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), CYAN);
}

void feellike_delaying(int cnt) {
  for (int i = 0; i < cnt; i++) {
    tft.print("=");
    delay(100);
  }
}

void print_info(int d, uint32_t time6) {
  /*
     @ brief: print out the current state of trash can, by info
     @ parameter: the data of sensor, 아직 안정해짐.
     @ return: None, only print
  */
  tft.fillScreen(BLACK);
  tft.fillRoundRect(10, 10, 100, 140, 20, WHITE);
  tft.fillRoundRect(130, 10, 100, 140, 20, WHITE);
  tft.fillRoundRect(10, 170, 220, 140, 20, WHITE);
  tft.drawLine(119, 0, 119, 159, BLACK);

  // first above all, It is assumed that air condition is good;
  draw_good_face();
  now_face = 1;

  // clock operation
  tft.setCursor(140, 20);
  tft.setTextSize(2);
  tft.print("Now! ");
  tft.setCursor(140, 70);
  int time_h = time6 / 10000;
  tft.print(time_h);
  tft.print(" : ");
  time6 = time6 % 10000;
  int time_m = time6 / 100;
  tft.print(time_m);
  time6 = time6 % 100;
  tft.setCursor(170, 100);
  tft.print(time6);

  // trash can
  draw_trashcan();

  // left side of trash Can
  tft.setCursor(20, 180);
  tft.setTextSize(2);
  tft.println("NOW ");
  tft.setCursor(20, 220);
  tft.println("TRASH ");
  tft.setCursor(20, 260);
  tft.println("CAN... ");
}

// functions that visualize trash can (start)
void show_status(uint8_t percentage) {
  /*
     @brief: Show status of trash can get byt percentage
     @parameter: percentage -> The percentage of how full of trash can
     @return: None
  */
  clear_trashcan();
  tft.setCursor(165, 225);
  tft.print(percentage);
  tft.print("%");
}

void draw_trashcan() {
  /*
     @brief: Draw trashcan that is empty
     @parameter: None
     @return: None
  */
  /*
    version of rectangle
    tft.fillRect(130, 185, 90, 115, WHITE);
    tft.drawRect(130, 185, 90, 110, BLACK);
    tft.drawLine(130, 185, 220, 185, WHITE);
  */

  // version of trapezoid
  tft.drawLine(130, 185, 150, 295, BLACK);
  tft.drawLine(150, 295, 200, 295, BLACK);
  tft.drawLine(220, 185, 200, 295, BLACK);

}

void clear_trashcan() {
  tft.fillRect(165, 225, 40, 40, WHITE);
}
// end

// functions that draw face ba affected by air condition (start)
// There is three conditon of air, good, normal, bad
void draw_face(uint8_t percentage){
    /*
     * @ brief: Draw a face depending on the percentage
     * @ parameter: percentage (0%~100%)
     * @ return: None
     */
    // 0~40 good, 40~70 normal, 70~ bad
    if(percentage <= 40){
        if(now_face == 1) return;
        clear_face();
        draw_good_face();
        now_face = 1;
    }
    else if(percentage <= 70){
        if(now_face==2) return;
        clear_face();
        draw_normal_face();
        now_face = 2;
    }
    else{
        if(now_face == 3) return;
        clear_face();
        draw_bad_face();
        now_face = 3;
    }
}
void clear_face(){
    /*
     * @ brief: clear the area of face imoticon
     * @ parameter: None
     * @ return: None
     */
    tft.fillRect(20, 40, 80, 80, WHITE);
}

void draw_good_face(){
    /*
     * @ brief: draw the blue good face on the area.
     * @ parameter: None
     * @ return: None
     */
    tft.fillCircle(60, 80, 40, GOOD);
    tft.drawCircle(40, 70, 10, WHITE);
    tft.drawCircle(80, 70, 10, WHITE);
    tft.fillRect(30, 70, 25, 20, GOOD);
    tft.fillRect(70, 70, 25, 20, GOOD);
    tft.fillCircle(60, 90, 15, WHITE);
    tft.fillRect(40, 70, 45, 20, GOOD); 

}
void draw_normal_face() {
    /*
     * @ brief: draw the green normal face on the area.
     * @ parameter: None
     * @ return: None
     */
    tft.fillCircle(60, 80, 40, NORMAL);
    tft.fillCircle(40, 70, 5, WHITE);
    tft.fillCircle(80, 70, 5, WHITE);
    tft.drawCircle(60, 90, 15, WHITE);
    tft.fillRect(45, 75, 30, 15, NORMAL);
}

void draw_bad_face(){
     /*
     * @ brief: draw the orange bad face on the area.
     * @ parameter: None
     * @ return: None
     */
    tft.fillCircle(60, 80, 40, BAD);
    tft.fillCircle(40, 70, 5, WHITE);
    tft.fillCircle(80, 70, 5, WHITE);
    tft.drawCircle(60, 100, 10, WHITE);
    tft.fillRect(50, 100, 22, 12, BAD);
}
// end

// functions of clock operation (start)
uint32_t print_clock(uint32_t time6){
    /*
     * @ brief: Calculate the time that cosist of six digits hhmmss
     * @ parameter: time6 that hhmmss
     * @ return: Value of time 6, that calculating 
     */
    uint8_t time_h = time6/10000;
    time6 %=10000;
    uint8_t time_m = time6/100;
    time6 %= 100;
    uint8_t time_s = time6;

    if(time_s >= 60){
        time_m += 1;
        time_s -= 60;
    }
    if(time_m >= 60){
        time_h += 1;
        time_m -= 60;
    }
    if(time_h >= 25){
        time_h = 0;
    }
    uint32_t time_return;
    time_return = (uint32_t)time_h*10000 + (uint32_t)time_m*100 + (uint32_t)time_s;
    tft.fillRect(140, 70, 85, 60, WHITE);
    
    tft.setCursor(140, 70);
    tft.print(time_h);
    tft.print(" : ");
    tft.print(time_m);
    tft.setCursor(170, 100);
    tft.print(time_s);
    return time_return;
}
// end
