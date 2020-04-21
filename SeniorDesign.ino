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

char testStr[] = "Comfortably Numb";
char testStr2[] = "Pink Floyd";

char testStr3[] = "00:00";
char testStr4[] = "06:22";

void setup() {
  
  delay(1000);
  Serial.begin(115200); 
  disp.setupDisplay();
  delay(100);
  setupButtons();

  fnt_controller.createDictionary(zelda_alph,8,16);
  fnt_controller.createSmallDictionary(small_numbs,8,7);


  disp.drawText(180,0,zelda_alph,testStr,sizeof(testStr),0x00);
  delay(10);
  disp.drawText(180,16,zelda_alph,testStr2,sizeof(testStr2),0x00);
  delay(10);
  disp.drawText(180,45,small_numbs,testStr3,sizeof(testStr3),0x30);
  delay(10);
  disp.drawText(36,45,small_numbs,testStr4,sizeof(testStr4),0x30);
  delay(10);
  
  disp.drawRect(0,55,180,4,true,(uint8_t)0x0);
  delay(10);
  disp.drawBitmap(106,35,16,16,shuffle_bits,sizeof(shuffle_bits));
  delay(10);
  disp.drawBitmap(70,35,16,16,repeat_bits,sizeof(repeat_bits));
  delay(10);
  disp.drawBitmap(192,0,64,64,wall_bits1,sizeof(wall_bits1));
  delay(10);
  
  
  delay(1000);
}

void loop() {
checkButtonStatus(buttonState);
disp.drawRect(0,55,180,4,true,(uint8_t)0x0);
  for (int w=180; w>0; w--)
  {
    
    disp.drawRect(0,55,w,4,true,(uint8_t)0x0);
    disp.drawRect(w,55,180-(w-1),4,true,(uint8_t)0xF);
    //disp.drawBitmap(w-2,54,4,7,second_marker,sizeof(second_marker));
    
 
    delay(100);
  }
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
