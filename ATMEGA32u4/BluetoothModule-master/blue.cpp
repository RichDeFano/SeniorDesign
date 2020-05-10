
#include "blue.h"


#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

const int numChars = 256; // an array to store the received data
int brackets = 0;
char receivedChars[numChars];
static byte ndx = 0;
char endMarker = '}';



Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
char * time = "12:00";

void blue::error(const __FlashStringHelper* err) {
  Serial.println(err);
  while (1);
}

void blue::setupBluetooth() {
  if ( !ble.begin(VERBOSE_MODE) )
  {
    //error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  //Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    //Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      //error(F("Couldn't factory reset"));
    }
  }

  ble.echo(false);
 // Serial.println("Requesting Bluefruit info:");
  ble.info();

  ble.verbose(false);  // debug info is a little annoying after this point!
  //Serial.print("Connecting");
  while (! ble.isConnected()) {
      //delay(500);
      //Serial.print(".");
  }
  //Serial.println("");

  //Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    //Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
 // Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);
  }
}


void blue::updateData(Music* music, Navigation* nav, RideTracking* ride) {
if (getDataUpdated() == true) {
  StaticJsonDocument<256> doc;

  DeserializationError error = deserializeJson(doc, receivedChars);

  if(error) {
    Serial.print("Error: ");
    Serial.println(error.c_str());
  }
  
  else{
    char s1[55];
    char s2[55];
    char s3[15];
    char s4[15];
    char s5[15];

  if(doc["music"]) {

    strcpy(s1, doc["music"]["track"]);
    music->setTrack(s1);

    strcpy(s2, doc["music"]["artist"]);
    music->setArtist(s2);

    strcpy(s3, doc["music"]["track_length"]);
    music->setLength(s3);

    strcpy(s4, doc["music"]["position"]);
    music->setPosition(s4);

    music->setProgressBar(doc["music"]["progressBar"]);

    music->setPlaying(doc["music"]["playing"]);
  } 

  if(doc["navigation"]["next_direction"]) {

    strcpy(s1, doc["navigation"]["next_direction"]);
    nav->setNextDir(s1);

    strcpy(s2, doc["navigation"]["next_road"]);
    nav->setNextRoad(s2);

    strcpy(s3, doc["navigation"]["next_step_distance"]);
    nav->setNextDistance(s3);

    strcpy(s4, doc["navigation"]["distance_remaining"]);
    nav->setDistance(s4);

    strcpy(s5, doc["navigation"]["time_remaining"]);
    nav->setTime(s5);

    nav->setRunning(true);
  }

  if(doc["navigation"]["running"]) {
    nav->setRunning(false);
  }

  if(doc["ride_tracking"]["distance"]) {
    strcpy(s1, doc["ride_tracking"]["distance"]);
    ride->setDistance(s1);

    strcpy(s2, doc["ride_tracking"]["time_ride"]);
    ride->setTime(s2);

    strcpy(s3, doc["ride_tracking"]["speed"]);
    ride->setSpeed(s3);

    strcpy(s4, doc["ride_tracking"]["avg_speed"]);
    ride->setAvg(s4);

    strcpy(s5, doc["ride_tracking"]["gain"]);
    ride->setGain(s5);


    ride->setRunning(true);
  }
  
 }
}
}

void blue::recvWithEndMarker(blue* bt, Music* mu, Navigation* nav, RideTracking* ride) {
  char rc;
  while (ble.available() > 0 && getDataUpdated() == false) {
    rc = ble.read();
    if (rc != endMarker) 
    {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) 
        {
          ndx = numChars - 1;
        }
    }
    else 
    {
      brackets++;
      if(brackets != 2)
        {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) 
          {
            ndx = numChars - 1;
          }
        }
      else
        {
          receivedChars[ndx] = rc;
          receivedChars[ndx+1] = '\0';
          ndx = 0;
          brackets = 0;
          setDataUpdated(true);
        }
    }
  }
  bt->updateData(mu,nav,ride);
}


void blue::sendData(){
    char n, inputs[256+1];

  if (Serial.available())
  {
    n = Serial.readBytes(inputs, BUFSIZE);
    inputs[n] = 0;
    // Send characters to Bluefruit
    //Serial.print("Sending: ");
    //Serial.println(inputs);

    // Send input data to host via Bluefruit
    ble.print(inputs);
  }
}




