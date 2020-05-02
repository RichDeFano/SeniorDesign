#include <SPI.h>

#include "display.h"
#include "bitmaps.h"
#include "bitmapStorage.h"
#include "bitmapLetter.h"
#include "textStorage.h"

//---Buttons
#define BTN1 5
#define BTN2 6
#define BTN3 9

int buttonHeld[] = {0,0,0};
int buttonState[] = {0,0,0};

ISR(TIMER0_COMPA_vect){
    buttonHeld[0] = 1;
}

ISR(TIMER1_COMPA_vect){
    buttonHeld[1] = 1;
}

ISR(TIMER3_COMPA_vect){
    buttonHeld[2] = 1;
}
//

Display_obj disp;
Bitmap waddle_map((double)24,(double)24, (int)6, (size_t)sizeof(waddle_dee_0),waddle_table);

bitmapLetter fnt_controller(0,0,0x00,0,0);
bitmapLetter zelda_alph[0x7F];
bitmapLetter small_numbs[0xA];

// Home Char Arrays
char timeStr[] = "11:50pm";

// Music Char Arrays
char trackStr[] = "Comfortably Numb";
char artistStr[] = "Pink Floyd";
char positionStr[] = "00:00";
char trackLengthStr[] = "06:22";

// Navigation Char Arrays
char instrStr[] = "Left on Burlington St";
char instrDistStr[] = "800 ft";
char timeRemainingStr[] = "2:30 hr";
char distRemainingStr[] = "1.25 mi";

// Ride Char Arrays
char currentSpeedStr[] = "10.5 mph";
char avgSpeedStr[] = "AVG: 5.5 mph";
char rideTimeStr[] = "0:45:02";
char distTraveledStr[] = "0.25 mi";

int screen = 0;

void setup() {
  
  delay(1000);
  Serial.begin(115200); 
  disp.setupDisplay();
  delay(100);
  setupButtons();

  fnt_controller.createDictionary(zelda_alph,8,16);
  fnt_controller.createSmallDictionary(small_numbs,8,7);

  homeUI();
  // musicUI();
  // navStartUI();
  // navRouteUI();
  // rideStartUI();
  // rideRunningUI();
  
  delay(1000);
}

void loop() {
checkButtonStatus(buttonState);

switch(screen) {

  case 0:
    homeUI();
    break;
  case 1:
    musicUI();
    break;
  case 2:
    navStartUI();
    break;
  case 3:
    rideStartUI();
    break;
  default:
    homeUI();
    break;        
  
}

// disp.drawRect(0,55,180,4,true,(uint8_t)0x0);
//  for (int w=180; w>0; w--)
//  {
//    
//    disp.drawRect(0,55,w,4,true,(uint8_t)0x0);
//    disp.drawRect(w,55,180-(w-1),4,true,(uint8_t)0xF);
//    //disp.drawBitmap(w-2,54,4,7,second_marker,sizeof(second_marker));
//    
// 
//    delay(100);
//  }
}


void setupButtons() {
  noInterrupts();
  //Timer 0
  TCCR0A = 0;
  //TCCR0B = 0;
  TCNT0 = 0;
  OCR0A = 46875;
  TCCR0B |= (1<<WGM01);
  TCCR0B |= (0<<CS02) | (0<<CS01) | (0<<CS00);
  TIMSK0 |= (1<<OCIE0A);
  
  //Timer 1
  TCCR1A = 0;
  //TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 46875;
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (0<<CS12) | (0<<CS11) | (0<<CS10);
  TIMSK1 |= (1<<OCIE1A);
  //
  
  //Timer 2
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;
  OCR3A = 46875;
  TCCR3B |= (1<<WGM32);
  TCCR3B |= (0<<CS32) | (0<<CS31) | (0<<CS30);
  TIMSK3 |= (1<<OCIE3A);
  //
  interrupts();
  
  //Button Pinmodes
  pinMode(BTN1,INPUT_PULLUP);
  pinMode(BTN2,INPUT_PULLUP);
  pinMode(BTN3,INPUT_PULLUP);

  Serial.println("Buttons Initialized.");
  delay(100);
}

void checkButtonStatus(int pastBtnState[]){
  uint8_t btn1, btn2, btn3;
  int btnState[] = {0,0,0};
  btn1 = digitalRead(BTN1);
  btn2 = digitalRead(BTN2);
  btn3 = digitalRead(BTN3);
  
  if (btn1 == HIGH)
  {btnState[0] = 1;}
  else
  {btnState[0] = 0;}

  if (btn2 == HIGH)
  {btnState[1] = 1;}
  else
  {btnState[1] = 0;}

  if (btn3 == HIGH)
  {btnState[2] = 1;}
  else
  {btnState[2] = 0;}

  if (btnState[0] != pastBtnState[0])
  {
    if (btnState[0] == 1)
      {
        Serial.println("Button 1 has been pressed");
        TCCR0B |= (1<<CS02)|(1<<CS00);
      }
    if (btnState[0] == 0)
      {
        Serial.println("Button 1 has been released");
        
        if (buttonHeld[0] == 1)
          {
          Serial.println("Button 1 has been held");
//          if(screen == 0) {
//            screen = 3;
//          } else {
//            screen = screen - 1;
//          }
          Serial.println(screen);
          disp.wipeDisplay();
          buttonHeld[0] = 0;
          }
          TCCR0B |= (0<<CS02)|(0<<CS00);
      }
      
  }

  if (btnState[1] != pastBtnState[1])
  {
    if (btnState[1] == 1)
      {
        TCCR1B |= (1<<CS12)|(1<<CS10);
        Serial.println("Button 2 has been pressed");
      }
    if (btnState[1] == 0)
      {
        Serial.println("Button 2 has been released");
        if (buttonHeld[1] == 1)
          {
          Serial.println("Button 2 has been held");
          buttonHeld[1] = 0;
          }
          TCCR1B |= (0<<CS12)|(0<<CS10);
      }
  }
  

  if (btnState[2] != pastBtnState[2])
  {
    if (btnState[2] == 1)
      {
        TCCR3B |= (1<<CS32)|(1<<CS30);
        Serial.println("Button 3 has been pressed");
      }
    if (btnState[2] == 0)
      {
       Serial.println("Button 3 has been released");
       if (buttonHeld[2] == 1)
          {
          Serial.println("Button 3 has been held");
//          if(screen == 3) {
//            screen = 0;
//          } else {
//            screen++;
//          }
          Serial.println(screen);
          disp.wipeDisplay();
          buttonHeld[2] = 0;
          }
          TCCR3B |= (0<<CS32)|(0<<CS30);
      }
  }
  pastBtnState[0] = btnState[0];
  pastBtnState[1] = btnState[1];
  pastBtnState[2] = btnState[2];


}

bool isButtonPressed(int button){
  if (buttonState[button] == 1){
    return true;
    }
  else{
    return false;
  }
}

void homeUI() {

  disp.drawText(140,24,zelda_alph,timeStr,sizeof(timeStr)-1,0x00);
  delay(10);
  
}

void musicUI() {

  disp.drawText(180,0,zelda_alph,trackStr,sizeof(trackStr)-1,0x00);
  delay(10);
  disp.drawText(180,16,zelda_alph,artistStr,sizeof(artistStr)-1,0x00);
  delay(10);
  disp.drawText(180,45,small_numbs,positionStr,sizeof(positionStr)-1,0x30);
  delay(10);
  disp.drawText(36,45,small_numbs,trackLengthStr,sizeof(trackLengthStr)-1,0x30);
  delay(10);
  
  disp.drawRect(0,55,180,4,true,(uint8_t)0x0);
  delay(10);
  disp.drawBitmap(106,35,16,16,shuffle_bits,sizeof(shuffle_bits));
  delay(10);
  disp.drawBitmap(70,35,16,16,repeat_bits,sizeof(repeat_bits));
  delay(10);
  disp.drawBitmap(192,0,64,64,wall_bits1,sizeof(wall_bits1));
  delay(10);
  
}

void navStartUI() {

  const char routeSelectStr1[] = "Start a Route from the";
  const char routeSelectStr2[] = "Arrow App";

  disp.drawText(200,16,zelda_alph,routeSelectStr1,sizeof(routeSelectStr1)-1,0x00);
  delay(10);
  disp.drawText(160,32,zelda_alph,routeSelectStr2,sizeof(routeSelectStr2)-1,0x00);
  delay(10);
  
}

void navRouteUI() {

  const char arrival[] = "Arrival";

  // Instruction
  disp.drawText(240,0,zelda_alph,instrDistStr,sizeof(instrDistStr)-1,0x00);
  delay(10);
  disp.drawText(240,16,zelda_alph,instrStr,sizeof(instrStr)-1,0x00);
  delay(10);

  disp.drawRect(0,45,256,20,true,(uint8_t)0x0);
  delay(10);

  // Arrival
  disp.drawText(240,48,zelda_alph,arrival,sizeof(arrival)-1,0x00);
  delay(10);
  disp.drawText(160,48,zelda_alph,timeRemainingStr,sizeof(timeRemainingStr)-1,0x00);
  delay(10);
  disp.drawText(80,48,zelda_alph,distRemainingStr,sizeof(distRemainingStr)-1,0x00);
  delay(10);
  
}

void rideStartUI() {

  const char rideSelectStr1[] = "Start a Ride";
  const char rideSelectStr2[] = "by pressing";
  const char rideSelectStr3[] = "the center";
  const char rideSelectStr4[] = "button";

  // Selection Message
  disp.drawText(100,0,zelda_alph,rideSelectStr1,sizeof(rideSelectStr1)-1,0x00);
  delay(10);
  disp.drawText(100,16,zelda_alph,rideSelectStr2,sizeof(rideSelectStr2)-1,0x00);
  delay(10);
  disp.drawText(100,32,zelda_alph,rideSelectStr1,sizeof(rideSelectStr1)-1,0x00);
  delay(10);
  disp.drawText(100,48,zelda_alph,rideSelectStr2,sizeof(rideSelectStr2)-1,0x00);
  delay(10);

  // Speed
  disp.drawText(220,24,zelda_alph,currentSpeedStr,sizeof(currentSpeedStr)-1,0x00);
  delay(10);

  disp.drawRect(127,0,1,64,true,(uint8_t)0x0);
  delay(10);
  
}

void rideRunningUI() {

  disp.drawText(220,8,zelda_alph,currentSpeedStr,sizeof(currentSpeedStr)-1,0x00);
  delay(10);
  disp.drawText(240,40,zelda_alph,avgSpeedStr,sizeof(avgSpeedStr)-1,0x00);
  delay(10);
  disp.drawText(80,8,zelda_alph,rideTimeStr,sizeof(rideTimeStr)-1,0x00);
  delay(10);
  disp.drawText(80,40,zelda_alph,distTraveledStr,sizeof(distTraveledStr)-1,0x00);
  delay(10);

  disp.drawRect(127,0,1,64,true,(uint8_t)0x0);
  delay(10);
  disp.drawRect(0,31,255,1,true,(uint8_t)0x0);
  delay(10);
  
}
