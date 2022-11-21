// This code is writen to read data from Benewake mini-S LiDAR through UART physical interface.
// The code is tested with Teensy 4.1. It can also be used with ESP8266 by making slight modifications
// by Ibrahim Technical Support Engineer: ibrahim@benewake.com

// This code reads serial data from 7 different Benewake mini-S LiDAR sensors

int DEBUG = 1;
double deltaAverageThresholdTriggerIn = 1.0; // IMPORTANT
double deltaAverageThresholdControlOut = 1.0; // IMPORTANT
double triggerDist = 250.0; // IMPORTANT
const double smoothSamples = 1.0 ;
int ctrlStartMs = 20;

int notes1[6] = {48, 50, 52, 55, 57, 59};
int notes2[6] = {48, 50, 52, 55, 57, 59};
int notes3[6] = {48, 50, 52, 55, 57, 59};
int notes4[6] = {48, 50, 52, 55, 57, 59};
int notes5[6] = {48, 50, 52, 55, 57, 59};
int notes6[6] = {48, 50, 52, 55, 57, 59};
int notes7[6] = {48, 50, 52, 55, 57, 59};

// length of notes arrays
// int numNotes = sizeof(notes1) -1;
int numNotes = 5;

int ctrlMapPos[7] = {94, 94, 94, 94, 94, 94, 94};
int ctrlMapNeg[7] = {93, 93, 93, 93, 93, 93, 93};

// int ctrlMapPos[7] = {0, 0, 0, 0, 0, 0, 0};
// int ctrlMapNeg[7] = {0, 0, 0, 0, 0, 0, 0};
int ctrlChannelMapPos[7] = {1, 2, 3, 4, 5, 6, 7};
int ctrlChannelMapNeg[7] = {1, 2, 3, 4, 5, 6, 7};

int inputMapRange[2] = {50, 250};

const int num_channels = 7; // number of analog channels to iterate over
int *notes[7] = {notes1, notes2, notes3, notes4, notes5, notes6, notes7};
int noteLock[7] = {0,0,0,0,0,0,0};

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

// bool first_iteration = true;
// const int sampleRate = 1000;
// const int analogMemSec = 3;
// const int analogMemSamples = analogMemSec * sampleRate;
// double analogMemory [num_channels][analogMemSamples];
// int analogMemCounter[num_channels] = {0, 0, 0, 0, 0, 0, 0};

// double triggerSampleCount[num_channels] = {0, 0, 0, 0, 0, 0, 0};

bool first_sample[num_channels] = {true, true, true, true, true, true, true} ;
double scalar1 = 1.0/smoothSamples;
double scalar2 = 1.0 - scalar1;

// TODO if smoothing is not needed, make this int 
double average[num_channels]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} ;
double lastAverage[num_channels]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} ;
double deltaAverage[num_channels]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} ;


// declare list for measured distances
int dists[7];
int lastDists[7] = {0, 0, 0, 0, 0, 0, 0};
int dist_temp;

unsigned char uart[7][5];  /*----save data measured by LiDAR-------------*/
const int HEADER=0x59; /*----frame header of data package------------*/
int rec_debug_state[7] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}; /*----receive state for frame----------------*/

int noteOut[7] = {0, 0, 0, 0, 0, 0, 0};
int noteIndex[7] = {-1, -1, -1, -1, -1, -1, -1};
int enterDist[7] = {999, 999, 999, 999, 999, 999, 999};
int distDiff[7] = {0, 0, 0, 0, 0, 0, 0};
int ctrlChange[7] = {0, 0, 0, 0, 0, 0, 0};
int triggerTick[7] = {0, 0, 0, 0, 0, 0, 0};


// USB MIDI receive functions
// contributed by Jan Zuiderveld

// laser_control_pins: 12 - 9, 6 - 3
int laser_control_pins[7] = {24, 12, 11, 10, 9, 6, 5};
int extra_control_pin = 25;

void OnNoteOn(byte channel, byte note, byte velocity) {
  digitalWrite(laser_control_pins[note], HIGH);
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  digitalWrite(laser_control_pins[note], LOW);  // Any Note-Off turns off LED
}

///////////////////////// SETUP ///////////////////////////// 

void setup() {
  auto start = millis();
  
  if (DEBUG) {
    Serial.begin(115200);
    Serial.println("DEBUG MODE");
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

}

////////////////////////////////// main loop //////////////////////////////////
void loop() {
  while (usbMIDI.read()) { // read all incoming messages
  }

  // read data from channels 
  for (int ch = 0; ch < num_channels; ch++) {
    while(Serials[ch].available()){
      Get_Lidar_data(Serials[ch], ch);
    }

    // if dists[ch] has not changed, continue
    if (dists[ch] == lastDists[ch]) {
        continue;
    }
  
    if (smoothSamples == 1.0) {
        average[ch] = dists[ch];
    } else {
        if (first_sample[ch]) {
            average[ch] = dists[ch];
            first_sample[ch] = false;
        }
        else {
            average[ch] = (dists[ch]*scalar1) + (average[ch]*scalar2);
        } 
    }

    // distance to MIDI translation
    // This section sends NoteOn messages to the MIDI channel when appropriate
    if (average[ch] != 0 && average[ch] < triggerDist && noteLock[ch] == 0) {

        deltaAverage[ch] = (lastAverage[ch] - average[ch]); // positive if lower, negative if higher away
        if (deltaAverage[ch] <= deltaAverageThresholdTriggerIn) {
        
            // map to note
            noteLock[ch] = 1;
            noteIndex[ch] = map(average[ch], inputMapRange[0], inputMapRange[1], 0, numNotes);

            // cut off index at 0 and 5
            if (noteIndex[ch] < 0) {
                noteIndex[ch] = 0;
            }
            if (noteIndex[ch] > 5) {
                noteIndex[ch] = 5;
            }
            noteOut[ch] = notes[ch][noteIndex[ch]];

            usbMIDI.sendNoteOn(noteOut[ch], 127, ch+1);
            usbMIDI.send_now();

            triggerTick[ch] = millis();

            // print noteOut
            if (DEBUG) {
              Serial.print("Note out: ");
              Serial.println(noteOut[ch]);
              Serial.print("Channel: ");
              Serial.println(ch);
           }
        } 

    }

    // This section sends ctrlChange messages to the MIDI channel when appropriate (distance change while noteLock is on)
    if (average[ch] != 0 && average[ch] < triggerDist && noteLock[ch] == 1) {
        if (triggerTick[ch] < (millis() + ctrlStartMs)) {
          
            if (enterDist[ch] == 0) {
                enterDist[ch] = average[ch];
            }
          
            // get difference between enterDist and average
            distDiff[ch] = average[ch] - enterDist[ch];
            
            // map to ctrlChange
            if (distDiff[ch] > 0) {
                ctrlChange[ch] = map(distDiff[ch], 0, 127, 0, 127);

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
                ctrlChange[ch] = map(distDiff[ch], 0, -127, 0, 127);
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
    if (noteLock[ch] == 1  && (average[ch] == 0 || average[ch] > triggerDist)) {

        // turn off note
        usbMIDI.sendNoteOn(noteOut[ch], 0, ch+1);
        usbMIDI.send_now();

        // reset control change
        usbMIDI.sendControlChange(ctrlMapPos[ch], 0, ctrlChannelMapPos[ch]);
        usbMIDI.sendControlChange(ctrlMapNeg[ch], 0, ctrlChannelMapNeg[ch]);
        usbMIDI.send_now();

        // reset noteLock
        noteLock[ch] = 0;

        // reset triggerSampleCount
        enterDist[ch] = 0;

        // print 
        if (DEBUG) {
            Serial.print("Note off: ");
            Serial.println(noteOut[ch]);
            Serial.print("Channel: ");
            Serial.println(ch);
            Serial.print("Distance: ");
            Serial.println(average[ch]);

        }

    }

    lastAverage[ch] = average[ch];
    lastDists[ch] = dists[ch];

  } // end of loop over channels

    // every 100ms, print the distances
    if (millis() % 100 == 0 && DEBUG && noteLock == 0) {
        for (int i = 0; i < 7; i++) {
            Serial.print(dists[i]);
            Serial.print(" ");
        }
        Serial.println();

    }


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
            while(serialX.available()){serialX.read();} // This part is added becuase some previous packets are there in the buffer so to clear serial buffer and get fresh data.
            
            dist_temp = uart[i][2] + uart[i][3]*256;//the distance

            // Serial.println(dist_temp);

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
