#include <Arduino.h>
#include "ArduinoJson.h"
#include <SPI.h>
#include <string.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include "../include/Music.h"
#include "../include/RideTracking.h"
#include "../include/Navigation.h"
#include "../include/BluefruitConfig.h"

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


Music music("Track","Artist", "01:00", "00:00", false,0);
RideTracking ride("","","","","", false);
Navigation nav("","","","","", false);

const int numChars = 200; // an array to store the received data

boolean newData = false;
int brackets = 0;
char receivedChars[numChars];
static byte ndx = 0;
char endMarker = '}';


void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  ble.verbose(false);  // debug info is a little annoying after this point!
  Serial.print("Connecting");
  while (! ble.isConnected()) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set module to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
}

void updateData() {
 if (newData == true) {
  StaticJsonDocument<256> doc;

  // Deserialize
  DeserializationError error = deserializeJson(doc, receivedChars);
  serializeJson(doc, Serial);
  Serial.println();

  if(error) {
    Serial.print("Error: ");
    Serial.println(error.c_str());
  }
  else{
  if(doc["music"]) {
    char t[100];
    strcpy(t, doc["music"]["track"]);
    music.setTrack(t);
    Serial.println(music.getTrack());
    strcpy(t, doc["music"]["artist"]);
    music.setArtist(t);
    Serial.println(music.getArtist());
    strcpy(t, doc["music"]["track_length"]);
    music.setLength(t);
    Serial.println(music.getLength());
    music.setPlaying(doc["music"]["playing"]);
    Serial.println(music.getPlaying());
    strcpy(t, doc["music"]["position"]);
    music.setPosition(t);
    Serial.println(music.getPosition());
    music.setProgressBar(doc["music"]["progressBar"]);
    Serial.println(music.getProgressBar());
  } 

  if(doc["navigation"]["next_direction"]) {
    char t[100];
    strcpy(t, doc["naviagtion"]["next_direction"]);
    nav.setNextDir(t);
    strcpy(t, doc["naviagtion"]["next_road"]);
    nav.setNextRoad(t);
    strcpy(t, doc["naviagtion"]["next_step_distance"]);
    nav.setNextDistance(t);
    strcpy(t, doc["naviagtion"]["distance_remaining"]);
    nav.setDistance(t);
    strcpy(t, doc["naviagtion"]["time_remaining"]);
    nav.setTime(t);
    nav.setRunning(true);
  }

  if(doc["navigation"]["running"]) {
    nav.setRunning(false);
  }

  if(doc["ride_tracking"]["distance"]) {
    char t[100];
    strcpy(t, doc["ride_tracking"]["distance"]);
    ride.setDistance(t);
    strcpy(t, doc["ride_tracking"]["time_ride"]);
    ride.setTime(t);
    strcpy(t, doc["ride_tracking"]["speed"]);
    ride.setSpeed(t);
    strcpy(t, doc["ride_tracking"]["avg_speed"]);
    ride.setAvg(t);
    strcpy(t, doc["ride_tracking"]["gain"]);
    ride.setGain(t);
    ride.setRunning(true);
  }
 }
 newData=false;
 }
}

void recvWithEndMarker() {
  char rc;
    
 // if (Serial.available() > 0) {
  while (ble.available() > 0 && newData == false) {
    rc = ble.read();
    //Serial.println(rc);
    
    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      brackets++;
      if(brackets != 2){
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }else{
        receivedChars[ndx] = rc;
        receivedChars[ndx+1] = '\0'; // terminate the string
        ndx = 0;
        brackets = 0;
        newData = true;
      }
    }
  }

}


void loop() {
  recvWithEndMarker();
  updateData();
}
