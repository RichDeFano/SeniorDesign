

#ifndef buttonsAndPages_h
#define buttonsAndPages_h

//---Buttons
#define BTN1 5
#define BTN2 6
#define BTN3 9


#include "display.h"
#include "bitmaps.h"
#include "bitmapStorage.h"
#include "bitmapLetter.h"
#include "textStorage.h"
#include "blue.h"

Display_obj disp;


int buttonHeld[] = {0, 0, 0};
int buttonState[] = {0, 0, 0};
int btnPressed = 0;
int screen = 1;
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

  delay(100);
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
      //Serial.println("Button 1 Release Action");
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
        //Serial.println("Button 3 Held Action");
        disp.wipeDisplay();
        pageInit = false;
        screen = screen+1;
        if (screen > 3)
        {screen = 0;}
      }
      else
      {
       //Serial.println("Button 3 Release Action");
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
        ////Serial.println("Button 1 has been pressed");
      }
    if (btnState[0] == 0)
      {
       ////Serial.println("Button 1 has been released");
       if (buttonHeld[0] == 1)
          {
          ////Serial.println("Button 1 has been held");
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
       // //Serial.println("Button 2 has been pressed");
      }
    if (btnState[1] == 0)
      {
       // //Serial.println("Button 2 has been released");
       buttonAction(1,false);
      }
  }


  if (btnState[2] != pastBtnState[2])
  {
    if (btnState[2] == 1)
      {
        TCCR3B |= (1<<CS32)|(1<<CS30);
        btnPressed = 3;
        ////Serial.println("Button 3 has been pressed");
      }
    if (btnState[2] == 0)
      {
       ////Serial.println("Button 3 has been released");
       if (buttonHeld[2] == 1)
          {
          ////Serial.println("Button 3 has been held");
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

  char error[] = "Please Connect Via ";
  char error2[] = "Bluetooth";
  char connected[] = "Connected!";

  disp.wipeDisplay();

  disp.drawBitmap(200,16,14,19,bluetooth_bits, sizeof(bluetooth_bits));
  disp.drawText(140,8,error,sizeof(error),0x00);  
  disp.drawText(140,16,error2,sizeof(error2),0x00);  

  bt.setupBluetooth();

  disp.drawText(140,50,connected,sizeof(connected),0x00);
  delay(1000);
}

//////////////////////////
//Home Screen
//////////////////////////
void homeInit(char* timeStr, char* ampm, char* dateStr) 
{

  disp.drawText(140,8,timeStr,strlen(timeStr),0x00);
  disp.drawText(140,32,dateStr,strlen(dateStr),0x00);
  pageInit = true;
}

void homeUpdate(char* timeStr, char* ampm, char* dateStr)
{
  disp.drawText(140,8,timeStr,strlen(timeStr),0x00);
  disp.drawText(140,32,dateStr,strlen(dateStr),0x00);
}
//////////////////////////
//Music Screen
//////////////////////////
void musicInit(char* trStr, char* arStr, char* posStr, char* tLStr, int progress)
{
  char positionStr[strlen(posStr)+1];
  strlcpy(positionStr,posStr,sizeof(positionStr));
  disp.drawText(180,45,positionStr,sizeof(positionStr),0x00);


  char trackLengthStr[strlen(tLStr)+1];
  strlcpy(trackLengthStr,tLStr,sizeof(trackLengthStr));
  disp.drawText(36,45,trackLengthStr,sizeof(trackLengthStr),0x00);

  char artistStr[strlen(arStr)+1];
  strlcpy(artistStr,arStr,sizeof(artistStr));
  disp.drawText(180,16,artistStr,sizeof(artistStr),0x00);

  char trackStr[strlen(trStr)+1];
  strlcpy(trackStr,trStr,sizeof(trackStr));
  disp.drawText(180,0,trackStr,sizeof(trackStr),0x00);

  
  disp.drawRect(0,55,180,4,true,(uint8_t)0x0);
  disp.drawRect(192,0,64,64,true,(uint8_t)0x0);
  disp.drawBitmap(196,24,54,18,music_bits,sizeof(music_bits));

 pageInit = true;
}


void musicUpdate(char* trStr, char* arStr, char* posStr, char* tLStr, int progress)
{
  char positionStr[strlen(posStr)+1];
  strlcpy(positionStr,posStr,sizeof(positionStr));
  disp.drawText(180,45,positionStr,sizeof(positionStr),0x00);

  char trackLengthStr[strlen(tLStr)+1];
  strlcpy(trackLengthStr,tLStr,sizeof(trackLengthStr));
  disp.drawText(36,45,trackLengthStr,sizeof(trackLengthStr),0x00);

  char artistStr[strlen(arStr)+1];
  strlcpy(artistStr,arStr,sizeof(artistStr));
  disp.drawText(180,16,artistStr,sizeof(artistStr),0x00);

  char trackStr[strlen(trStr)+1];
  strlcpy(trackStr,trStr,sizeof(trackStr));
  disp.drawText(180,0,trackStr,sizeof(trackStr),0x00);

  disp.drawRect(0,55,progress,4,true,(uint8_t)0x0);
  disp.drawRect(progress,55,180-(progress-1),4,true,(uint8_t)0xF);
}

//////////////////////////
//Navigation Screen
//////////////////////////
void navInit() {
  char routeSelectStr1[] = "Start a Route from the";
  char routeSelectStr2[] = "Arrow App";

  disp.drawText(200,16,routeSelectStr1,sizeof(routeSelectStr1),0x00);
  disp.drawText(160,32,routeSelectStr2,sizeof(routeSelectStr2),0x00);

  pageInit = true;
  disp.wipeDisplay();
  disp.drawRect(0,45,256,20,true,(uint8_t)0x0);
}

void navUpdate(char* instrStr, char* instrDistStr, char* timeRemainingStr, char* distRemainingStr, char* arrTime) {
  char arrival[] = "Arr:";

  disp.drawText(240,0,instrDistStr,strlen(instrDistStr),0x00);
  disp.drawText(240,16,instrStr,strlen(instrStr),0x00);
  disp.drawText(244,48,arrival,sizeof(arrival),0x00);
  disp.drawText(208,48,arrTime,strlen(arrTime),0x00);
  disp.drawText(140,48,timeRemainingStr,strlen(timeRemainingStr),0x00);
  disp.drawText(60,48,distRemainingStr,strlen(distRemainingStr),0x00);
  disp.drawBitmap(5,0,28,24,fullLeft_bits,sizeof(fullLeft_bits));
}

//////////////////////////
//Navigation Screen
//////////////////////////
void rideInit() {
  char rideSelectStr1[] = "Start a Ride";
  char rideSelectStr2[] = "by pressing";
  char rideSelectStr3[] = "the center";
  char rideSelectStr4[] = "button";

  disp.drawText(100,0,rideSelectStr1,sizeof(rideSelectStr1),0x00);
  disp.drawText(100,16,rideSelectStr2,sizeof(rideSelectStr2),0x00);
  disp.drawText(100,32,rideSelectStr3,sizeof(rideSelectStr3),0x00);
  disp.drawText(100,48,rideSelectStr4,sizeof(rideSelectStr4),0x00);

  disp.wipeDisplay();

pageInit = true;
}

void rideUpdate(char* currentSpeedStr, char* avgSpeedStr, char* rideTimeStr, char* distTraveledStr) {
  disp.drawText(220,8,currentSpeedStr,strlen(currentSpeedStr),0x00);
  disp.drawText(220,40,avgSpeedStr,strlen(avgSpeedStr),0x00);
  disp.drawText(80,8,rideTimeStr,strlen(rideTimeStr),0x00);
  disp.drawText(80,40,distTraveledStr,strlen(distTraveledStr),0x00);

  disp.drawBitmap(230,0,28,27,speedometer_bits, sizeof(speedometer_bits));
  disp.drawBitmap(230,35,28,27,speedometer_bits, sizeof(speedometer_bits));
  disp.drawBitmap(90,0,28,31,clock_bits,sizeof(clock_bits));
  disp.drawBitmap(90,35,16,29,mapPin_bits, sizeof(mapPin_bits));
}
#endif