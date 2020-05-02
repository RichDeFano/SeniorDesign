
#include <SPI.h>

ISR(TIMER3_COMPA_vect){
    if (btnPressed == 1)
    {buttonHeld[0] = 1;}
    if (btnPressed == 3)
    {buttonHeld[2] = 1;}
}

void setup() {
  
  delay(1000);
  Serial.begin(115200); 
  disp.setupDisplay();
  delay(100);
  setupButtons();

  fnt_controller.createDictionary(zelda_alph,8,16);
  fnt_controller.createSmallDictionary(small_numbs,8,7);

  homeInit();
  delay(1000);
}

void loop() {
checkButtonStatus(buttonState);


switch(screen) {

  case 0:
    if (pageInit == false)
    {
      homeInit();
      pageInit == true;
    }
    break;
  case 1:
  if (pageInit == false)
    {
    musicInit();
    pageInit == true;
    }
    break;
  case 2:
  if (pageInit == false)
    {
    navStartInit();
    pageInit == true;
    }
    break;
  case 3:
  if (pageInit == false)
    {
    rideStartInit();
    pageInit == true;
    }
    break;
  default:
  if (pageInit == false)
    {
    homeInit();
    pageInit == true;
    }
    break;        
  
}
}
