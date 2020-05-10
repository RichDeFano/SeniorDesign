#ifndef blue_h
#define blue_h

#include "Music.h"
#include "Navigation.h"
#include "RideTracking.h"

//#include <Arduino.h>
#include "ArduinoJson.h"
//#include <SPI.h>
#include <Adafruit_ATParser.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BLE.h>



#include "BluefruitConfig.h"

class Music;
class Navigation;
class RideTracking;

class blue
{
  public:


    blue(){
        newData = false;
    };
    ~blue(){};
    void error(const __FlashStringHelper* err);
    void setupBluetooth();
    void sendData();
    void updateData(Music* music,Navigation* nav,RideTracking* r);
    void csHigh();
    void csLow();
    void recvWithEndMarker(blue *bt,Music* mu,Navigation* nav,RideTracking* r);
    void flush();

    void setDataUpdated(bool f){newData = f;}
    bool getDataUpdated(){return newData;}
    void drawMusicScreen();
    
    /*
    Music* getMusic(){return music;}
    Navigation* getNavigation(){return nav;}
    RideTracking* getRideTracking(){return ride;}

    void setMusic(Music* m){music = m;}
    void setNavigation(Navigation n){nav = n;}
    void setRideTracking(RideTracking r){ride = r;}
    */

    bool blueCS(){
        int at = digitalRead(BLUEFRUIT_SPI_CS);
        if (at == HIGH)
            {return true;}
        else
            {return false;}
    }

    char* getRecievedChars(){return receivedChars;}

    
    
private:
    /*
    Music* music;
    Navigation* nav;
    RideTracking* ride;
    */
    bool newData;
    char receivedChars[256];
    
};
#endif

