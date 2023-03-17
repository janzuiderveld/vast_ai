// This code is writen to read data from Benewake mini-S LiDAR through UART physical interface.
// The code is tested with Teensy 4.1. It can also be used with ESP8266 by making slight modifications

// This code reads serial data from 7 different Benewake mini-S LiDAR sensors

int DEBUG = 1; // TURN OFF for fixing laginess?

double deltaAverageThresholdTriggerIn = 10.0; // IMPORTANT
int settleRequired = 2;

double deltaAverageThresholdControlOut = 1.0; // IMPORTANT not yet

double triggerDist[7] = {999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0};

int ctrlStartMs = 200; 

// lead 1
int notes1[7] = {57, 58, 60, 62, 64, 64, 64};
// lead 2
int notes2[7] = {67, 69, 70, 72, 74, 74, 74};
// lead 3
int notes3[7] = {69, 70, 72, 74, 76, 77, 79};
// bass
int notes4[6] = {45, 46, 48, 50, 52, 53};
// bass 2
int notes5[6] = {55, 57, 58, 60, 62, 64};
// drum
int notes6[6] = {60, 60, 60, 60, 62, 62};
int notes7[6] = {67, 67, 67, 67, 69, 69};

int numNotes = 5; // cutoff for notes used 

int chMap[7] = {1, 1, 2, 3, 3, 4, 4}; 
int ctrlChannelMapPos[7] = {1, 1, 2, 3, 3, 4, 4};
int ctrlChannelMapNeg[7] = {1, 1, 2, 3, 3, 4, 4};

int ctrlMapPos[7] = {75, 16, 93, 69, 69,        18, 84};
int ctrlMapNeg[7] = {70, 16, 69, 24, 70,       119, 119};
int ctrlPosReset[7] = {40, 51, 0, 50, 40,           0, 0};
int ctrlNegReset[7] = {40, 51, 64, 20, 40,         0, 0};
int ctrlPosRange[7] = {127, 64, 127, 100, 127,     127, 127};
int ctrlNegRange[7] = {-127, -64, 127, 100, 30,    127, 127};

// not implemented yet
int ctrlMapPos2[7] = {0, 0, 0, 0,    0, 0, 0};
int ctrlMapNeg2[7] = {0, 0, 0, 0,    0, 0, 0};
int ctrlPosReset2[7] = {0, 0, 0, 0,    0, 0, 0};
int ctrlNegReset2[7] = {0, 0, 0, 0,    0, 0, 0};

int retriggerStart[7] = {500, 0, 0, 0, 0, 500, 500};
int retriggerMod[7] = {500, 0, 0, 0, 0, 500, 500};

int lastRetrigger[7] = {0, 0, 0, 0, 0, 0, 0};

int inputMapRange[2] = {80, 280};
// create another list 

const int num_channels = 7; // number of analog channels to iterate over
int *notes[7] = {notes1, notes2, notes3, notes4, notes5, notes6, notes7};
int noteLock[7] = {0,0,0,0,0,0,0};

int settleCount[7] = {0,0,0,0,0,0,0};

int sendMidi = 1;

// Ordered Serial list
HardwareSerial Serials[7] {
  Serial7,
  Serial8,
  Serial3,
  Serial4,
  Serial5,
  Serial1,
  Serial2
};

// TODO if smoothing is not needed, make this int 
int dists[num_channels]= {0, 0, 0, 0, 0, 0, 0} ;
int strengths[num_channels]= {0, 0, 0, 0, 0, 0, 0} ;
int lastAverage[num_channels]= {0, 0, 0, 0, 0, 0, 0} ;
int deltaAverage[num_channels]= {0, 0, 0, 0, 0, 0, 0} ;

// declare list for measured distances
int lastDists[7] = {0, 0, 0, 0, 0, 0, 0};
int dist_temp = 0;

unsigned char uart[7][6];  /*----save data measured by LiDAR-------------*/
const int HEADER=0x59; /*----frame header of data package------------*/
int rec_debug_state[7] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}; /*----receive state for frame----------------*/

int noteOut[7] = {0, 0, 0, 0, 0, 0, 0};
int noteIndex[7] = {-1, -1, -1, -1, -1, -1, -1};
int enterDist[7] = {999, 999, 999, 999, 999, 999, 999};
int distDiff[7] = {0, 0, 0, 0, 0, 0, 0};
int ctrlChange[7] = {0, 0, 0, 0, 0, 0, 0};
int triggerTick[7] = {0, 0, 0, 0, 0, 0, 0};

int lastTick[7] = {0, 0, 0, 0, 0, 0, 0};
int currentMillis = 0;
int minTimeBetweenTicks = 1; // POSSIBLE FIX for lagginess; set to higher value

int lastTriggerDist[7] = {0, 0, 0, 0, 0, 0, 0};

// laser_control_pins: 12 - 9, 6 - 3
int laser_control_pins[7] = {24, 11, 12, 10, 9, 6, 5};
int extra_control_pin = 25;


// define calbirate function
void calibrate(int ch) {
  // turn off laser
  pinMode(laser_control_pins[ch], INPUT);

  // sum 100 samples
  for (int j = 0; j < 100; j++) {
    
    Get_Lidar_data(Serials[ch], ch);
    // check if reading is not 0
    while (dists[ch] == 0) {
      Get_Lidar_data(Serials[ch], ch);
    }

    // add to dists
    triggerDist[ch] += dists[ch];

    // delay 1 ms
    delay(1);
  }

  triggerDist[ch] = triggerDist[ch] / 100;
  triggerDist[ch] = triggerDist[ch] - 30;

  // print triggerDist
  if (DEBUG) {
    Serial.print("Trigger Distance for ch ");
    Serial.print(ch);
    Serial.print(" is ");
    Serial.println(triggerDist[ch]);
  }

  // turn on laser
  pinMode(laser_control_pins[ch], OUTPUT);
}

void reportDistances() {
    for (int i = 0; i < 7; i++) {
        if (dists[i] > 0) {
            // send first 2 digits
            usbMIDI.sendControlChange(1, int(dists[i] / 10), 10);
            // send last digit
            usbMIDI.sendControlChange(1, dists[i] % 10, 11);
        }
    }
    usbMIDI.send_now();
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  // if note is between 0 and 6, turn on laser
  if (note < 7) {
    digitalWrite(laser_control_pins[note], HIGH);
  }

  // if note is 7, turn on extra control pin
  if (note == 7) {
    // Serial.println("Turning on extra control pin");
    digitalWrite(extra_control_pin, HIGH);
  }

  // if note == 10, set sendMidi to true
  if (note == 10) {
    sendMidi = 1;
  }

  // if note is between 20 and 26, calibrate channel
  if (note > 19 && note < 27) {
    calibrate(note - 20);
  }

  // if note is 30 report distances
  if (note == 30) {
    reportDistances();
  }

}


void OnNoteOff(byte channel, byte note, byte velocity) {
  // if note is between 0 and 6, turn ff laser
  if (note < 7) {
    digitalWrite(laser_control_pins[note], LOW);
    // exit function
    return;
  }

  // else if note is 7, turn off extra control pin
  if (note == 7) {
    digitalWrite(extra_control_pin, LOW);
    // exit function
    return;
  }

  // if note == 10, set sendMidi to false
  if (note == 10) {
    sendMidi = 0;
  }

}

///////////////////////// SETUP ///////////////////////////// 

void setup() {
  auto start = millis();
  
  if (DEBUG) {
    Serial.begin(115200);
    Serial.println("DEBUGUG MODE");
  }

  // begin all 7 serial ports with baud rate 115200
  for (int i = 0; i < 7; i++) {
    Serials[i].begin(115200);
  }

  // set up laser control pins
  for (int i = 0; i < 7; i++) {
    pinMode(laser_control_pins[i], OUTPUT);
    digitalWrite(laser_control_pins[i], HIGH);
  }

  // set up extra control pin
  pinMode(extra_control_pin, OUTPUT);

  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn) ;

  if (DEBUG) {
    // print the time that has elapsed
    auto end = millis();
    auto duration = (end - start);
    // print duration
    Serial.print("Setup Duration (ms): ");
    Serial.println(duration);
  }

  // turn off all lasers
  for (int i = 0; i < 7; i++) {
    // digitalWrite(laser_control_pins[i], LOW);
    pinMode(laser_control_pins[i], INPUT);
  }

  // Calibrate the sensors 
  for (int ch = 0; ch < 7; ch++) {
    // turn on laser
    pinMode(laser_control_pins[ch], OUTPUT);

    // read 1000 samples
    for (int j = 0; j < 1000; j++) {
      
      // check if reading is not 0
      while (dists[ch] == 0) {
        // read sensor
        Get_Lidar_data(Serials[ch], ch);
      }

      // read sensor
      Get_Lidar_data(Serials[ch], ch);
      // add to dists
      triggerDist[ch] += dists[ch];
      // delay 1 ms
      delay(1);
    }

    // divide by 1000
    triggerDist[ch] /= 1000;

    // subtract 10
    triggerDist[ch] -= 30;

    Serial.print(ch);
    Serial.print(" ");
    Serial.println(triggerDist[ch]);


    // turn off laser
    pinMode(laser_control_pins[ch], INPUT);
  }

  // blink lasers to signal setup is done
  for (int i = 0; i < 7; i++) {
    pinMode(laser_control_pins[i], OUTPUT);
  }
  delay(500);
  for (int i = 0; i < 7; i++) {
    // digitalWrite(laser_control_pins[i], LOW);
    pinMode(laser_control_pins[i], INPUT);
  }
  delay(500);
    for (int i = 0; i < 7; i++) {
    pinMode(laser_control_pins[i], OUTPUT);
    // digitalWrite(laser_control_pins[i], HIGH);
  }
  delay(500);
  for (int i = 0; i < 7; i++) {
    // digitalWrite(laser_control_pins[i], LOW);
    pinMode(laser_control_pins[i], INPUT);
  }
  delay(500);
    for (int i = 0; i < 7; i++) {
    // digitalWrite(laser_control_pins[i], HIGH);
    pinMode(laser_control_pins[i], OUTPUT);
  }

  //  setup Volca drum 
  // control channel 103 - 108: send 127
  for (int wave_send_cc = 103; wave_send_cc < 109; wave_send_cc++) {
    usbMIDI.sendControlChange(wave_send_cc, 127, 4);
  }

  // init ctrl according to ctrlReset
  for (int i = 0; i < 4; i++) {
    usbMIDI.sendControlChange(ctrlChannelMapNeg[i], ctrlNegReset[i], i);
    usbMIDI.sendControlChange(ctrlChannelMapPos[i], ctrlPosReset[i], i);
  }

  for (int ch = 0; ch < 7; ch++) {
    lastTick[ch] = millis();
  }

}
////////////////////////////////// main loop //////////////////////////////////
void loop() {
  while (usbMIDI.read()) { // read all incoming messages
  }

  if (sendMidi == 0) {
      return;
  }

  // read data from channels 
  for (int ch = 0; ch < num_channels; ch++) {

    while(Serials[ch].available()){
          Get_Lidar_data(Serials[ch], ch);
    }

    if (millis() <= (lastTick[ch] + minTimeBetweenTicks)) {
      continue;
    } 

    lastTick[ch] = millis();

    // // retrigger
    // check if channel is in retrigger mode
    if (retriggerStart[ch] != 0) {
      // check if channel is active
        if (dists[ch] != 0 && dists[ch] < triggerDist[ch] && noteLock[ch] == 1) {
        // check if last retrigger was long enough ago
        if (millis() > retriggerMod[ch] + lastRetrigger[ch]) {

          // turn off note
          usbMIDI.sendNoteOn(noteOut[ch], 0, chMap[ch]);

          // Send NoteOn message to MIDI channel
          usbMIDI.sendNoteOn(noteOut[ch], 120 + chMap[ch], chMap[ch]);
          // update lastRetrigger
          lastRetrigger[ch] = millis();
        }
      }
    }

    // check if channel is inactive, but dists is within triggerDist
    if (dists[ch] != 0 && dists[ch] < triggerDist[ch] && noteLock[ch] == 0) {
      // absoulute of dists
        deltaAverage[ch] = abs(lastAverage[ch] - dists[ch]); // absoulute of dists
        if (deltaAverage[ch] <= deltaAverageThresholdTriggerIn) { 
            settleCount[ch] += 1;
        } else {
          settleCount[ch] = 0;
        }
    }

    if (dists[ch] == lastDists[ch]) {
      continue;
    }

    // distance to MIDI translation
    // This section sends NoteOn messages to the MIDI channel when appropriate
    if (dists[ch] != 0 && dists[ch] < triggerDist[ch] && noteLock[ch] == 0) {

        // // TODO this should be moved out of this loop, where it only counts on change of dist
        // deltaAverage[ch] = (lastAverage[ch] - dists[ch]); // positive if lower, negative if higher away
        // if (deltaAverage[ch] <= deltaAverageThresholdTriggerIn) { // Use absolute diff here?
        //     settleCount[ch] += 1;
        //     // 

            if (settleCount[ch] >= settleRequired) {
              lastTriggerDist[ch] = dists[ch];

              // map to note
              noteLock[ch] = 1;
              noteIndex[ch] = map(dists[ch], inputMapRange[0], inputMapRange[1], 0, numNotes);

              // cut off index at 0 and 5
              if (noteIndex[ch] < 0) {
                  noteIndex[ch] = 0;
              }
              if (noteIndex[ch] > 5) {
                  noteIndex[ch] = 5;
              }
              noteOut[ch] = notes[ch][noteIndex[ch]];

              usbMIDI.sendNoteOn(noteOut[ch], 120 + chMap[ch], chMap[ch]);
              usbMIDI.send_now();

              triggerTick[ch] = millis();

              lastRetrigger[ch] = millis();

              // print noteOut
              if (DEBUG) {
                Serial.print("Note out: ");
                Serial.println(noteOut[ch]);
                Serial.print("Channel: ");
                Serial.println(ch);
            }
          }
        
        // //
        // } else {
        //   settleCount[ch] = 0;
        // }
        // //

    }

    // This section sends ctrlChange messages to the MIDI channel when appropriate (distance change while noteLock is on)
    // check if laser is still active
    if (dists[ch] != 0 && dists[ch] < triggerDist[ch] && noteLock[ch] == 1) {
        currentMillis = millis();
        if (currentMillis > (triggerTick[ch] + ctrlStartMs)) {
            // lastTick[ch] = currentMillis;

            if (enterDist[ch] == 0) {
                enterDist[ch] = dists[ch];
            }
          
            // get difference between enterDist and dists
            distDiff[ch] = dists[ch] - enterDist[ch];
            
            // map to ctrlChange
            if (distDiff[ch] > 0) {
                ctrlChange[ch] = map(distDiff[ch], 0, 127, 0, ctrlPosRange[ch]) + ctrlPosReset[ch];

                // cutoff 
                if (ctrlChange[ch] > 127) {
                    ctrlChange[ch] = 127;
                }
                if (ctrlChange[ch] < 1) {
                    ctrlChange[ch] = 1;
                }

                usbMIDI.sendControlChange(ctrlMapPos[ch], ctrlChange[ch], ctrlChannelMapPos[ch]);

            }

            if (distDiff[ch] < 0) {
                ctrlChange[ch] = map(distDiff[ch], 0, -127, 0, ctrlNegRange[ch]) + ctrlNegReset[ch];
                
                // cutoff 
                if (ctrlChange[ch] > 127) {
                    ctrlChange[ch] = 127;
                }
                if (ctrlChange[ch] < 1) {
                    ctrlChange[ch] = 1;
                } 

                usbMIDI.sendControlChange(ctrlMapNeg[ch], ctrlChange[ch], ctrlChannelMapNeg[ch]);
            }

            usbMIDI.send_now();

            // if retriggerStart, edit retriggerMod[ch]
            if (retriggerStart[ch] > 0) {
                retriggerMod[ch] = retriggerStart[ch] + map(distDiff[ch], 0, 100, 0, 500);
                if (retriggerMod[ch] > 2000) {
                    retriggerMod[ch] = 2000;
                }
                if (retriggerMod[ch] < 10) {
                    retriggerMod[ch] = 10;
                }
            }

            // print ctrlChange
            if (DEBUG) {
                Serial.print("CtrlChange: ");
                Serial.println(ctrlChange[ch]);
                Serial.print("Channel: ");
                Serial.println(ch);
                Serial.print("Dist ");
                Serial.println(dists[ch]);
            }

        }

    }

    // This section sends NoteOff messages to the MIDI channel when appropriate
    if (noteLock[ch] == 1  && (dists[ch] == 0 || dists[ch] > triggerDist[ch])) {

        // turn off note
        usbMIDI.sendNoteOn(noteOut[ch], 0, chMap[ch]);
        usbMIDI.send_now();

        // reset control change
        usbMIDI.sendControlChange(ctrlMapPos[ch], ctrlPosReset[ch], ctrlChannelMapPos[ch]);
        usbMIDI.sendControlChange(ctrlMapNeg[ch], ctrlNegReset[ch], ctrlChannelMapNeg[ch]);
        usbMIDI.send_now();

        // reset noteLock
        noteLock[ch] = 0;

        // reset triggerSampleCount
        enterDist[ch] = 0;

        // reset settleCount
        settleCount[ch] = 0;

        // print 
        if (DEBUG) {
            Serial.print("Note off: ");
            Serial.println(noteOut[ch]);
            Serial.print("Channel: ");
            Serial.println(ch);
            Serial.print("Distance: ");
            Serial.println(dists[ch]);
            // print lastTriggerDist
            Serial.print("Last trigger dist: ");
            Serial.println(lastTriggerDist[ch]);
            // print triggerTick
            Serial.print("Trigger tick: ");
            Serial.println(triggerTick[ch]);
            // print currentMillis
            Serial.print("Current millis: ");
            Serial.println(currentMillis);

        }

    }

    lastAverage[ch] = dists[ch];
    lastDists[ch] = dists[ch];

  } // end of loop over channels


    // every 100ms, print the distances
    // first check if DEBUG is on
    // if (DEBUG) {
    //   if (millis() % 100 == 0) {
          // for (int i = 0; i < 7; i++) {
          //     Serial.print(dists[i]);
          //     Serial.print(" ");
          // }
          
          // Serial.println(dists[0]);
          // Serial.println(strengths[0]);
          // Serial.println(" ");
      // }
    // }

}

/// @brief 
/// @param serialX 
/// @param i 
void Get_Lidar_data(HardwareSerial serialX, int i){
if (serialX.available()) //check if serial port has data input
    {
    if(rec_debug_state[i] == 0x01)
        {  //the first byte
          //Serial.println("first byte");
          uart[i][0]=serialX.read();
          //Serial.println(uart[i][0]);
          if(uart[i][0] == 0x59)
              {
                // check = uart[i][0];
                rec_debug_state[i] = 0x02;
              }
        }
        
else if(rec_debug_state[i] == 0x02)
     {//the second byte
      //Serial.println("2nd byte");
      uart[i][1]=serialX.read();
      if(uart[i][1] == 0x59)
          {
            rec_debug_state[i] = 0x03;
          }
      else{
            rec_debug_state[i] = 0x01;
          }
      }

else if(rec_debug_state[i] == 0x03)
        {
          uart[i][2]=serialX.read();
          rec_debug_state[i] = 0x04;
        }
else if(rec_debug_state[i] == 0x04)
        {
          uart[i][3]=serialX.read();
          rec_debug_state[i] = 0x05;
        }

else if(rec_debug_state[i] ==  0x05)
           {
            
            uart[i][4]=serialX.read();
            uart[i][5]=serialX.read();
            strengths[i] = uart[i][4] + (uart[i][5]*256);
            
            while(serialX.available()){serialX.read();} // This part is added becuase some previous packets are there in the buffer so to clear serial buffer and get fresh data.

            dist_temp = uart[i][2] + uart[i][3]*256;//the distance
            // Do not update bad data
            if (dist_temp <= 0 || dist_temp > 1200) {
              rec_debug_state[i] = 0x01;
              // dists[i] = 1200;
              return;
            }

            dists[i] = dist_temp;

            while(serialX.available()){serialX.read();} // This part is added becuase some previous packets are there in the buffer so to clear serial buffer and get fresh data.
            rec_debug_state[i] = 0x01;
            
            // Serial.println(dists[i]); //output measure distance value of LiDAR
            
           }
    
        }
}
