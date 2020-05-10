#include <SPI.h>
#include "blue.h"
#include "buttonsAndPages.h"

ISR(TIMER3_COMPA_vect) {
  if (btnPressed == 1)
  {
    buttonHeld[0] = 1;
  }
  if (btnPressed == 3)
  {
    buttonHeld[2] = 1;
  }
}


Music music;
RideTracking ride;
Navigation nav;

blue bt;


void setup() {
  setupButtons();
  Serial.begin(115200);//115200
  disp.setupDisplay();
  connectInit(bt);
  disp.wipeDisplay();
}

void loop() {
if (bt.getDataUpdated() == false)
  {
    bt.recvWithEndMarker(&bt,&music,&nav,&ride);
  }
else
  {
  updateScreen();
  bt.setDataUpdated(false);
  }
}



void updateScreen() {
  switch (screen) {
    case 0:
      if (pageInit == false) {
      // homeInit(timeStr, ampm, dateStr);
      }
      
      if (pageUpdated == false) {
        //homeUpdate(timeStr, ampm, dateStr);
      }
      break;
      
    case 1:
      if (pageInit == false) {
       musicInit(music.getTrack(), music.getArtist(), music.getPosition(), music.getLength(),music.getProgressBar());
      }
      
      else{
        musicUpdate(music.getTrack(), music.getArtist(), music.getPosition(), music.getLength(),music.getProgressBar());
      }

      break;
      
    case 2:
      if (pageInit == false) {
       // navInit();
      }
      
        if (pageUpdated == false) {
        //navUpdate(instrStr, instrDistStr, timeRemainingStr, distRemainingStr,arrTime);
        }
      
      break;
      
    case 3:
      if (pageInit == false) {
      //  rideInit();
      }
      
        if (pageUpdated == false) {
       // rideUpdate(currentSpeedStr, avgSpeedStr, rideTimeStr, distTraveledStr);
        }
      
      break;
    default:
      if (pageInit == false) {
        //homeInit(timeStr, ampm, dateStr);
      }
      
        if (pageUpdated == false) {
        //homeUpdate(timeStr, ampm, dateStr);
        }
      
      break;

  }
}
