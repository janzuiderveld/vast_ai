// This code is wrriten to read data from Benewake mini-S LiDAR through UART physical interface.
// The code is tested with Teensy 4.1. It can also be used with ESP8266 by making slight modifications
// by Ibrahim Technical Support Engineer: ibrahim@benewake.com

// This code reads serial data from 7 different Benewake mini-S LiDAR sensors

// Ant 1: C3, D3, E3, G3, A3 Ant 2: C4, D4, E4, G4, A4 Ant 3: C5, D5, E5, G5, A5
// Ant 1: 48, 50, 52, 55, 57 Ant 2:  Ant 3: 

int noteLock[7] = {0,0,0,0,0,0,0};

int notes1[5] = {48, 50, 52, 55, 57};
int notes2[5] = {48, 50, 52, 55, 57};
int notes3[5] = {48, 50, 52, 55, 57};
int notes4[5] = {48, 50, 52, 55, 57};
int notes5[5] = {48, 50, 52, 55, 57};
int notes6[5] = {48, 50, 52, 55, 57};
int notes7[5] = {48, 50, 52, 55, 57};

// list of notes
int *notes[7] = {notes1, notes2, notes3, notes4, notes5, notes6, notes7};

int triggerSampleThreshold = 5;

double triggerDist = 250.0;

int lastDist = 0;

// Serial 2 (sensor 1)
#define RXD2 7
#define TXD2 8

// Serial 1 (sensor 2)
#define RXD1 0
#define TXD1 1

// Serial 5 (sensor 3)
#define RXD5 21
#define TXD5 20

// Serial 4 (sensor 4)
#define RXD4 16
#define TXD4 17

// Serial 3 (sensor 5)
#define RXD3 15
#define TXD3 14

// Serial 8 (sensor 6)
#define RXD8 34
#define TXD8 35

// Serial 7 (sensor 7)
#define RXD7 28
#define TXD7 29

// Ordered Serial list
HardwareSerial Serials[7] {
  Serial2,
  Serial1,
  Serial5,
  Serial4,
  Serial3,
  Serial8,
  Serial7
};

const int gAnalogChannelNum = 7; // number of analog channels to iterate over

// needed?
bool first_sample[gAnalogChannelNum] = {true, true, true, true, true, true, true} ;
bool first_iteration = true;
///////////////////////////////////////

double analogInMax[gAnalogChannelNum]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} ;
double analogInMin[gAnalogChannelNum]= {9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 9999.0, 9999.0, } ;

double runningAverage;


float lastTrigger[gAnalogChannelNum];
const int sampleRate = 1000;
const int analogMemSec = 3;
const int analogMemSamples = analogMemSec * sampleRate;

double analogMemory [gAnalogChannelNum][analogMemSamples];
int analogMemCounter[gAnalogChannelNum] = {0, 0, 0, 0, 0, 0, 0};

double triggerSampleCount[gAnalogChannelNum] = {0, 0, 0, 0, 0, 0, 0};
double thresh [gAnalogChannelNum];
double outputScaling [gAnalogChannelNum];

double thresh_error = 0.02;
const double smoothSamples = 1.0 ;
double scalar1 = 1.0/smoothSamples;
double scalar2 = 1.0 - scalar1;

// TODO if smoothing is not needed, make this int 
double average[gAnalogChannelNum]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} ;
double lastAverage[gAnalogChannelNum]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} ;
double deltaAverage[gAnalogChannelNum]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} ;
double scaled[gAnalogChannelNum]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} ;

int counter = 0;

//declare list for measured distances
int dists[7];
int dist_temp;

int i;
unsigned char uart[7][5];  /*----save data measured by LiDAR-------------*/
const int HEADER=0x59; /*----frame header of data package------------*/
int rec_debug_state[7] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}; /*----receive state for frame----------------*/

int noteOut[7] = {0, 0, 0, 0, 0, 0, 0};
int noteIndex[7] = {-1, -1, -1, -1, -1, -1, -1};
int enterDist[7] = {999, 999, 999, 999, 999, 999, 999};
int distDiff[7] = {0, 0, 0, 0, 0, 0, 0};
int ctrlChange[7] = {0, 0, 0, 0, 0, 0, 0};
int ctrlMapPos[7] = {0, 0, 0, 0, 0, 0, 0};
int ctrlMapNeg[7] = {0, 0, 0, 0, 0, 0, 0};
int ctrlChannelMapPos[7] = {1, 2, 3, 4, 5, 6, 7};
int ctrlChannelMapNeg[7] = {1, 2, 3, 4, 5, 6, 7};

// USB MIDI receive functions
// contributed by Jan Zuiderveld

// laser_control_pins: 12 - 9, 6 - 3
int laser_control_pins[7] = {25, 24, 11, 9, 6, 5, 3};

void OnNoteOn(byte channel, byte note, byte velocity) {
  digitalWrite(laser_control_pins[note+1], HIGH);
  
  // OR if this does not work:
  // pinMode(laser_control_pins[i], OUTPUT);
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  digitalWrite(laser_control_pins[note+1], LOW);  // Any Note-Off turns off LED

  // OR if this does not work:
  // pinMode(laser_control_pins[i], INPUT);
}

///////////////////////// SETUP ///////////////////////////// 

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("\n STARTING \n");
  auto start = millis();

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

  // blink lasers during setup
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

  // // loop until analogMem is filled
  // Serial.println("Filling memory");
  // while (analogMemCounter[6] < analogMemSamples) {
  //     for(unsigned int ch = 0; ch < gAnalogChannelNum; ch++) {
          
  //         // save current rec_debug_state
  //         int rec_debug_state_last = rec_debug_state[ch];
          
  //         while(Serials[ch].available()){
  //             Get_Lidar_data(Serials[ch], ch);
  //         }

  //         if (rec_debug_state[ch] == 0x01) {
  //             if (rec_debug_state_last == 0x05) {
  //                 // save to memory
  //                 analogMemory[ch][analogMemCounter[ch]] = dists[ch];
  //                 analogMemCounter[ch]++;

  //                 if (dists[ch] > analogInMax[ch]) {
  //                     // print to the console the channel, the min and max
  //                     // std::cout << "Channel " << ch << ": " << analogInMin[ch] << " to " << analogInMax[ch] << std::endl;
  //                     analogInMax[ch] = dists[ch];
  //                 }

  //                 if (dists[ch] < analogInMin[ch]) {
  //                     // filter our zero readings
  //                         // print to the console the channel, the min and max
  //                         // std::cout << "Channel " << ch << ": " << analogInMin[ch] << " to " << analogInMax[ch] << std::endl;
  //                     analogInMin[ch] = dists[ch];
  //                 }
  //             }
  //         } 
  //     }
    
  // // print counters
  // for (int i = 0; i < 7; i++) {
  //   Serial.print(analogMemCounter[i]);
  //   Serial.print(" ");
  // }
  // Serial.println("");

  // }

  // Serial.println("");

  // // loop over channels
  // for(unsigned int ch = 0; ch < gAnalogChannelNum; ch++) {

  //     //print AnalogInMin and AnalogInMax
  //     Serial.print("Channel ");
  //     Serial.print(ch);
  //     Serial.print(": ");
  //     Serial.print(analogInMin[ch]);
  //     Serial.print(" to ");
  //     Serial.println(analogInMax[ch]);

  //     // print memory counter
  //     Serial.print("Memory counter: ");
  //     Serial.println(analogMemCounter[ch]);

  //     }

  first_iteration = false;

  // print the time that has elapsed
  auto end = millis();
  auto duration = (end - start);
  //print duration
  Serial.print("Duration (ms): ");
  Serial.println(duration);

  delay(2000);
}


////////////////////////////////// main loop //////////////////////////////////
void loop() {
  usbMIDI.read();

  // TODO If performance is an issue, move everything to this first loop over channels
  // read data from channels 
  for (int i = 0; i < 7; i++) {
    while(Serials[i].available()){
      Get_Lidar_data(Serials[i], i);
    }
  }

  // iterate over channels for processing
  // get the average according to the smoothing factor
  for(unsigned int ch = 0; ch < gAnalogChannelNum; ch++) {
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

    // if noteLock is true, then the note will not change until the noteLock is released
    // if noteLock is false, then the note will change to corresponding note when the distance has settled

    // This section sends NoteOn messages to the MIDI channel when appropriate
    if (average[ch] != 0 && average[ch] < triggerDist && noteLock[ch] == 0) {
        

        // TODO add measures to check if average is stable?
        triggerSampleCount[ch]++;

        // like this:
        // calculate absolute distance from last measurement
        // deltaAverage = abs(average[ch] - lastAverage[ch]);
        // if (deltaAverage > deltaAverageThreshold) {
        
        
        // print triggerSampleCount
        // Serial.print("Trigger sample count: ");
        // Serial.println(triggerSampleCount[ch]);

        // if last triggerSampleThreshold measurements < triggerDist: map to note
        if (triggerSampleCount[ch] > triggerSampleThreshold) {
            // map to note
            noteLock[ch] = 1;
            noteIndex[ch] = map(average[ch], 30, 180, 0, 5);
            // cut off index at 0 and 5
            if (noteIndex[ch] < 0) {
                noteIndex[ch] = 0;
            }
            if (noteIndex[ch] > 5) {
                noteIndex[ch] = 5;
            }
            noteOut[ch] = notes[ch][noteIndex[ch]];

            usbMIDI.sendNoteOn(noteOut[ch], 127, ch+1);

            enterDist[ch] = average[ch];
        } 

    } else {
        // reset triggerSampleCount
        triggerSampleCount[ch] = 0;
    }

    // This section sends ctrlChange messages to the MIDI channel when appropriate (distance change while noteLock is on)
    // usbMIDI.sendControlChange(control, value, channel);
    if (average[ch] != 0 && average[ch] < triggerDist && noteLock[ch] == 1) {
        // get difference between enterDist and average
        distDiff[ch] = average[ch] - enterDist[ch];
        
        // map to ctrlChange
        if (distDiff[ch] > 0) {
            ctrlChange[ch] = map(distDiff[ch], 0, 127, 0, 127);
            usbMIDI.sendControlChange(ctrlMapPos[ch], ctrlChange[ch], ctrlChannelMapPos[ch]);
        }

        if (distDiff[ch] < 0) {
            ctrlChange[ch] = map(distDiff[ch], -127, 0, 0, 127);
            usbMIDI.sendControlChange(ctrlMapNeg[ch], ctrlChange[ch], ctrlChannelMapNeg[ch]);
        }
    }


    // This section sends NoteOff messages to the MIDI channel when appropriate
    if (average[ch] == 0 || average[ch] > triggerDist) {
        // turn off note
        usbMIDI.sendNoteOff(noteOut[ch], 0, ch+1);
        // reset control change
        usbMIDI.sendControlChange(ctrlMapPos[ch], 0, ctrlChannelMapPos[ch]);
        usbMIDI.sendControlChange(ctrlMapNeg[ch], 0, ctrlChannelMapNeg[ch]);
        // reset noteLock
        noteLock[ch] = 0;
    }

    lastAverage[ch] = average[ch];







            // scale the analog input
            // scaled[ch] = (average[ch] - analogInMin[ch]) * outputScaling[ch];
            // scaled[ch] = (average[ch] - analogInMin[ch]);
            // if (ch == 6) {
            //     // scaled[ch] = scaled[ch] - 0.15;
            //     scaled[ch] = scaled[ch] - 0.0;
            // }
           
            // scaled[ch] = (average[ch] - analogInMax[ch]) * 1.5;
            
            // scaled[ch] = (average[ch] - analogInMax[ch]) * outputScaling[ch];


  //     note[i] = 60;
  //     usbMIDI.sendNoteOn(60, 99, i+1);  // 60 = C4

  //     Serial.println(i);
  //     Serial.println("note on");

  //   // if dists[i] > 100 and note is not 0, send midi note off
  //   } else if (dists[i] > 100 && note[i] != 0) {
  //     note[i] = 0;
  //     usbMIDI.sendNoteOff(60, 0, i+1);  // 60 = C4
  //     Serial.println(i);
  //     Serial.println("note off");
  //   }
  // }


  // for (int i = 0; i < 7; i++) {
  //   // if dists[i] < 100 and note is not 60, send midi note on
  //   if (dists[i] < 100 && note[i] != 60) {
  //     note[i] = 60;
  //     usbMIDI.sendNoteOn(60, 99, i+1);  // 60 = C4

  //     Serial.println(i);
  //     Serial.println("note on");

  //   // if dists[i] > 100 and note is not 0, send midi note off
  //   } else if (dists[i] > 100 && note[i] != 0) {
  //     note[i] = 0;
  //     usbMIDI.sendNoteOff(60, 0, i+1);  // 60 = C4
  //     Serial.println(i);
  //     Serial.println("note off");
  //   }
  // }


// Notes:
//   Channel 6: 1
// Channel 6: 0
// Channel 6: 1
// Channel 6: 0
// Channel 6: 1
// Channel 6: 0
// Channel 6: 1
// Channel 6: 0
// Channel 6: 1
// Channel 6: 0
// Channel 6: 1
// Channel 6: 0

  // every 100ms, print the distances
  // if (millis() % 100 == 0) {
    // for (int i = 0; i < 7; i++) {
    //   Serial.print(note[i]);
    //   Serial.print(" ");
    // }
    // Serial.println();


    // print ch 6 debug info
    // Serial.print("NoteIndex: ");
    // Serial.println(noteIndex[6]);
    // Serial.print("Note: ");
    // Serial.println(note[6]);
    // Serial.print("Average: ");
    // Serial.println(average[6]);
    // Serial.print("Dist: ");
    // Serial.println(dists[6]);


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
              dists[i] = 1200;
              return;
            }

            dists[i] = dist_temp;

            while(serialX.available()){serialX.read();} // This part is added becuase some previous packets are there in the buffer so to clear serial buffer and get fresh data.
            rec_debug_state[i] = 0x01;
            
            // Serial.println(dists[i]); //output measure distance value of LiDAR
            
           }
    
        }
}