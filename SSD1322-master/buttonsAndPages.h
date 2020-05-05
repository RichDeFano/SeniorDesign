//---Buttons
#define BTN1 5
#define BTN2 6
#define BTN3 9

#include "display.h"
#include "bitmaps.h"
#include "bitmapStorage.h"
//#include <U8glib.h>
#include "bitmapLetter.h"
#include "textStorage.h"
#include "blue.h"
Display_obj disp;

//U8GLIB_NHD31OLED_GR u8g(A0,A2);

bitmapLetter fnt_controller(0,0,0x00,0,0);
bitmapLetter sheldon_alph[0x7C];
//bitmapLetter small_numbs[0xA];

int buttonHeld[] = {0, 0, 0};
int buttonState[] = {0, 0, 0};
int btnPressed = 0;
int screen = 0;
bool pageInit = false;
bool pageUpdated = false;


/////////////////////////////////////////////////////

void setupButtons() {
  noInterrupts();
  //Timer 2
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3 = 0;
  OCR3A = 16000;
  TCCR3B |= (1<<WGM32);
  TCCR3B |= (0<<CS32) | (0<<CS31) | (0<<CS30);
  TIMSK3 |= (1<<OCIE3A);
  interrupts();

  //Button Pinmodes
  pinMode(BTN1,INPUT_PULLUP);
  pinMode(BTN2,INPUT_PULLUP);
  pinMode(BTN3,INPUT_PULLUP);

  Serial.println("Buttons Initialized.");
  delay(100);
  //u8g.firstPage();
}

void buttonAction(int button, bool held){
  //Button 1
  if (button == 0)
  {
    if (held == true)
    {
      //Serial.println("Button 1 Held Action");
      disp.wipeDisplay();
      pageInit = false;
      screen = screen-1;
      if (screen < 0)
      {screen = 3;}
    }
    else
    {
     // Serial.println("Button 1 Release Action");
    }
  }
  //Button 2
  if (button == 1)
    {
      //Serial.println("Button 2 Release Action");

    }
  //Button 3
    if (button == 2)
    {
      if (held == true)
      {
       // Serial.println("Button 3 Held Action");
        disp.wipeDisplay();
        pageInit = false;
        screen = screen+1;
        if (screen > 3)
        {screen = 0;}
      }
      else
      {
       // Serial.println("Button 3 Release Action");
      }
  }
  
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
        TCCR3B |= (1<<CS32)|(1<<CS30);
        btnPressed = 1;
        //Serial.println("Button 1 has been pressed");
      }
    if (btnState[0] == 0)
      {
       //Serial.println("Button 1 has been released");
       if (buttonHeld[0] == 1)
          {
          //Serial.println("Button 1 has been held");
          buttonAction(0,true);
          buttonHeld[0] = 0;
          }
          else
          {
            buttonAction(0,false);
          }
          TCCR3B |= (0<<CS32)|(0<<CS30);
          btnPressed = 0;
      }
      

  }

  if (btnState[1] != pastBtnState[1])
  {
    if (btnState[1] == 1)
      {
       // Serial.println("Button 2 has been pressed");
      }
    if (btnState[1] == 0)
      {
       // Serial.println("Button 2 has been released");
       buttonAction(1,false);
      }
  }


  if (btnState[2] != pastBtnState[2])
  {
    if (btnState[2] == 1)
      {
        TCCR3B |= (1<<CS32)|(1<<CS30);
        btnPressed = 3;
        //Serial.println("Button 3 has been pressed");
      }
    if (btnState[2] == 0)
      {
       //Serial.println("Button 3 has been released");
       if (buttonHeld[2] == 1)
          {
          //Serial.println("Button 3 has been held");
          buttonAction(2,true);
          buttonHeld[2] = 0;
          }
          else
          {
            buttonAction(2,false);
          }
          TCCR3B |= (0<<CS32)|(0<<CS30);
          btnPressed = 0;
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


//////////////////////////
//Connection Screen
//////////////////////////
void connectInit(blue bt){
  disp.wipeDisplay();
  char error[] = "Please Connect via Bluetooth.";
  disp.drawText(140,8,sheldon_alph,error,sizeof(error),0x00);  

  bt.csLow();
  bt.setupBluetooth();
  bt.csHigh();
  
}

void connectUpdate(){

}
//////////////////////////
//Home Screen
//////////////////////////
void homeInit(char* timeStr, char* ampm, char* dateStr) {
  disp.wipeDisplay();

  //char timeStr[] = "11:50";
  //char ampm[] = "am";
 // char dateStr[] = "5/3/2020";

  disp.drawText(140,8,sheldon_alph,timeStr,sizeof(timeStr),0x00);
  disp.drawText(172,8,sheldon_alph,ampm,sizeof(ampm),0x00);
  disp.drawText(140,8,sheldon_alph,dateStr,sizeof(dateStr),0x00);
  pageInit = true;
  //Serial.println("Initialized Home.");
}

void homeUpdate(char* timeStr, char* ampm. char* dateStr){
  //Need to update time text once a minute
  disp.drawText(140,8,sheldon_alph,timeStr,sizeof(timeStr),0x00);
  disp.drawText(172,8,sheldon_alph,ampm,sizeof(ampm),0x00);
  disp.drawText(140,8,sheldon_alph,dateStr,sizeof(dateStr),0x00);
  pageUpdated = true;
}
//////////////////////////
//Music Screen
//////////////////////////
void musicInit(char* trackStr, char* artistStr, char* positionStr, char* trackLengthStr) {
  disp.wipeDisplay();
    // Music Char Arrays
  //char trackStr[] = "Gainz";
  //char artistStr[] = "Deadlyft";
  //char positionStr[] = "00:00";
  //char trackLengthStr[] = "03:53";


  disp.drawText(180,0,sheldon_alph,trackStr,sizeof(trackStr),0x00);
 // delay(10);
  disp.drawText(180,16,sheldon_alph,artistStr,sizeof(artistStr),0x00);
  //delay(10);
  disp.drawText(180,45,sheldon_alph,positionStr,sizeof(positionStr),0x00);
  //delay(10);
  disp.drawText(36,45,sheldon_alph,trackLengthStr,sizeof(trackLengthStr),0x00);
  //delay(10);
  
  disp.drawRect(0,55,180,4,true,(uint8_t)0x0);
  //delay(10);
  //disp.drawBitmap(106,35,16,16,shuffle_bits,sizeof(shuffle_bits));
 // delay(10);
  disp.drawBitmap(70,35,16,16,repeat_bits,sizeof(repeat_bits));
  //delay(10);
  //disp.drawBitmap(192,0,64,64,deadlyft_bit,sizeof(deadlyft_bits));
  disp.drawRect(192,0,64,64,true,(uint8_t)0x0);
  disp.drawBitmap(196,24,54,18,music_bits,sizeof(music_bits)-1);
 // delay(10);

pageInit = true;
  //Serial.println("Initialized Music.");
  
}

void musicUpdate(char* trackStr, char* artistStr, char* positionStr, char* trackLengthStr){

    // Music Char Arrays
  //char trackStr[] = "Gainz";
  //char artistStr[] = "Deadlyft";
  //char positionStr[] = "00:00";
  //char trackLengthStr[] = "03:53";
  
  //Update position string and track length
  
  disp.drawText(180,45,sheldon_alph,positionStr,sizeof(positionStr),0x00);
 // delay(10);
  disp.drawText(36,45,sheldon_alph,trackLengthStr,sizeof(trackLengthStr),0x00);
  //delay(10);
 /* 
  disp.drawRect(0,55,180,4,true,(uint8_t)0x0);
  for (int w=180; w>0; w--)
  {
    disp.drawRect(0,55,w,4,true,(uint8_t)0x0);
    disp.drawRect(w,55,180-(w-1),4,true,(uint8_t)0xF);
    delay(100);
  }
  */
  pageUpdated = true;
  
}
//////////////////////////
//Navigation Screen
//////////////////////////

void navInit(char* instrStr, char* instrDistStr, char* timeRemainingStr, char* distRemainingStr) {
  disp.wipeDisplay();
  // Navigation Char Arrays
  //char instrStr[] = "Left on Burlington St";
  //char instrDistStr[] = "800 ft";
  //char timeRemainingStr[] = "2:30 hr";
  //char distRemainingStr[] = "1.25 mi";

  const char routeSelectStr1[] = "Start a Route from the";
  const char routeSelectStr2[] = "Arrow App";

  disp.drawText(200,16,sheldon_alph,routeSelectStr1,sizeof(routeSelectStr1)-1,0x00);
  //delay(10);
  disp.drawText(160,32,sheldon_alph,routeSelectStr2,sizeof(routeSelectStr2)-1,0x00);
  //delay(10);

  pageInit = true;
  //Serial.println("Initialized Nav Start.");
  
}

void navUpdate(char* instrStr, char* instrDistStr, char* timeRemainingStr, char* distRemainingStr) {
  disp.wipeDisplay();
   // Navigation Char Arrays
 // char instrStr[] = "Left on Burlington St";
 // char instrDistStr[] = "800 ft";
  //char timeRemainingStr[] = "2:30 hr";
  //char distRemainingStr[] = "1.25 mi";

  const char arrival[] = "Arrival";

  // Instruction
  disp.drawText(240,0,sheldon_alph,instrDistStr,sizeof(instrDistStr)-1,0x00);
  //delay(10);
  disp.drawText(240,16,sheldon_alph,instrStr,sizeof(instrStr)-1,0x00);
 // delay(10);

  disp.drawRect(0,45,256,20,true,(uint8_t)0x0);
 // delay(10);

  // Arrival
  disp.drawText(240,48,sheldon_alph,arrival,sizeof(arrival)-1,0x00);
//delay(10);
  disp.drawText(160,48,sheldon_alph,timeRemainingStr,sizeof(timeRemainingStr)-1,0x00);
 // delay(10);
  disp.drawText(80,48,sheldon_alph,distRemainingStr,sizeof(distRemainingStr)-1,0x00);
 // delay(10);

  pageUpdated = false;
  //Serial.println("Initalize nav route.");
  
}


//////////////////////////
//Navigation Screen
//////////////////////////
void rideInit() {
  disp.wipeDisplay();
  // Ride Char Arrays

  const char rideSelectStr1[] = "Start a Ride";
  const char rideSelectStr2[] = "by pressing";
  const char rideSelectStr3[] = "the center";
  const char rideSelectStr4[] = "button";

  // Selection Message
  disp.drawText(100,0,sheldon_alph,rideSelectStr1,sizeof(rideSelectStr1)-1,0x00);
  delay(10);
  disp.drawText(100,16,sheldon_alph,rideSelectStr2,sizeof(rideSelectStr2)-1,0x00);
  delay(10);
  disp.drawText(100,32,sheldon_alph,rideSelectStr3,sizeof(rideSelectStr1)-1,0x00);
  delay(10);
  disp.drawText(100,48,sheldon_alph,rideSelectStr4,sizeof(rideSelectStr2)-1,0x00);
  delay(10);


pageInit = true;
 // Serial.println("Initalized ride Start.");
  
}

void rideUpdate(char* currentSpeedStr, char* avgSpeedStr, char* rideTimeStr, char* distTraveledStr) {

  // Ride Char Arrays
//char currentSpeedStr[] = "10.5 mph";
//char avgSpeedStr[] = "AVG: 5.5 mph";
//char rideTimeStr[] = "0:45:02";
//char distTraveledStr[] = "0.25 mi";

  disp.drawText(220,8,sheldon_alph,currentSpeedStr,sizeof(currentSpeedStr)-1,0x00);
  //delay(10);
 disp.drawText(240,40,sheldon_alph,avgSpeedStr,sizeof(avgSpeedStr)-1,0x00);
  //delay(10);
 disp.drawText(80,8,sheldon_alph,rideTimeStr,sizeof(rideTimeStr)-1,0x00);
 // delay(10);
 disp.drawText(80,40,sheldon_alph,distTraveledStr,sizeof(distTraveledStr)-1,0x00);
  //delay(10);


pageUpdated = true;
  //Serial.println("Initalized Ride Screen.");
  
}
