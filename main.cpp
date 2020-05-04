#include <Arduino.h>
#include "ArduinoJson.h"
#include "../include/Music.h"
#include "../include/RideTracking.h"
#include "../include/Navigation.h"

#define BUFSIZE                        256

Music music("Track","Artist", "01:00", "00:00", false,0);
RideTracking ride("","","","","", false);
Navigation nav("","","","","", false);

const int numChars = 200;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;
int brackets = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '}';
  char rc;
 
 // if (Serial.available() > 0) {
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

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
        receivedChars[ndx] = '}';
        receivedChars[ndx+1] = '\0'; // terminate the string
        ndx = 0;
        brackets = 0;
        newData = true;
      }
    }
  }
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

  if(doc["music"]) {
    music.setTrack(doc["music"]["track"]);
    music.setArtist(doc["music"]["artist"]);
    music.setLength(doc["music"]["track_length"]);
    music.setPlaying(doc["music"]["playing"]);
    music.setPosition(doc["music"]["position"]);
    music.setProgressBar(doc["music"]["progressBar"]);
    char* str = music.getTrack();
    Serial.println(str);
  } 

  if(doc["navigation"]["next_direction"]) {
    nav.setNextDir(doc["navigation"]["next_direction"]);
    nav.setNextRoad(doc["navigation"]["next_road"]);
    nav.setNextDistance(doc["navigation"]["next_step_distance"]);
    nav.setDistance(doc["navigation"]["distance_remaining"]);
    nav.setTime(doc["navigation"]["time_remaining"]);
    nav.setRunning(true);
  }

  if(doc["navigation"]["running"]) {
    nav.setRunning(false);
  }

  if(doc["ride_tracking"]["distance"]) {
    ride.setDistance(doc["ride_tracking"]["distance"]);
    ride.setTime(doc["ride_tracking"]["time_ride"]);
    ride.setSpeed(doc["ride_tracking"]["speed"]);
    ride.setAvg(doc["ride_tracking"]["avg_speed"]);
    ride.setGain(doc["ride_tracking"]["gain"]);
    ride.setRunning(true);
  }
  newData = false;
 }
}

void loop() {
  recvWithEndMarker();
  updateData();
}
