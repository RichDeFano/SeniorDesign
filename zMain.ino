
#include <SPI.h>
#include "blue.h"
#include "buttonsAndPages.h"


ISR(TIMER3_COMPA_vect){
    if (btnPressed == 1)
    {buttonHeld[0] = 1;}
    if (btnPressed == 3)
    {buttonHeld[2] = 1;}
}


Music music("","yeet", "", "", false,0);
RideTracking ride("","","","","", false);
Navigation nav("","","","","", false);

blue bt(&music,&nav,&ride);


void setup() {

  
  delay(1000);
  Serial.begin(9600);//115200
  disp.setupDisplay();
  disp.pullCSHigh();
  
  delay(100);
  setupButtons();
  fnt_controller.createDictionary(sheldon_alph,6,8);
  delay(100);
  disp.pullCSLow();
  delay(1);
  connectInit(bt);
  delay(1);
  disp.pullCSHigh();
  delay(1000);
  

}

void loop() {
updateDataOverBluetooth();
delay(1000);
Serial.println("Fetching Data from Bluetooth..");
Music *btUpdate = bt.getMusic();
char* temp = btUpdate->getArtist();
Serial.println(temp);


/*
checkButtonStatus(buttonState);

delay(100);

disp.pullCSLow();
switch (screen) {

case 0:
  if (pageInit == false) {
    homeInit(timeStr, ampm, dateStr);
  }
  if (pageUpdate == false) {
    homeUpdate(timeStr, ampm, dateStr);
  }
  break;
case 1:
  if (pageInit == false) {
    musicInit(trackStr, artistStr, positionStr, trackLengthStr);
  }
  if (pageUpdate == false) {
    musicUpdate(trackStr, artistStr, positionStr, trackLengthStr);
  }
  break;
case 2:
  if (pageInit == false) {
    navInit();
  }
  if (pageUpdate == false) {
    navUpdate(instrStr, instrDistStr, timeRemainingStr, distRemainingStr);
  }
  break;
case 3:
  if (pageInit == false) {
    rideInit();
  }
  if (pageUpdate == false) {
    rideUpdate(currentSpeedStr, avgSpeedStr, rideTimeStr, distTraveledStr);
  }
  break;
default:
  if (pageInit == false) {
    homeInit(timeStr, ampm, dateStr);
  }
  if (pageUpdate == false) {
    homeUpdate(timeStr, ampm, dateStr);
  }
  break;

}
disp.pullCSHigh();
delay(10);
*/
}


void updateDataOverBluetooth(){
    
bt.csLow();
bt.recvWithEndMarker();
bt.updateData();
bt.csHigh();

}
  
