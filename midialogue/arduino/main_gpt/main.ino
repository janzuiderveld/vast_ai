// Configuration constants
const int DEBUG = 0;
const double deltaAverageThresholdTriggerIn = 1.0;
const int settleRequired = 4;
const double triggerDist = 250.0;
const double smoothSamples = 1.0;
const int num_channels = 7;

// Function declarations
void setupLaserControlPins();
void setupVolcaDrum();
void initControlChannels();
void readMidiMessages();
void processLidarData();
void handleRetrigger(int ch);
void updateAverage(int ch);
void handleNoteOn(int ch);
void handleCtrlChange(int ch);
void handleNoteOff(int ch);
void Get_Lidar_data(HardwareSerial &SerialX, int ch);

// Global variables
int DEBUG = 1; // TURN OFF for fixing laginess?
double deltaAverageThresholdTriggerIn = 1.0; // IMPORTANT
int settleRequired = 4;
double deltaAverageThresholdControlOut = 1.0; // IMPORTANT not yet
double triggerDist = 250.0; // IMPORTANT
const double smoothSamples = 1.0 ; 
int ctrlStartMs = 100; // Doesn't seem to be working, need to test./.

// lead 1
int notes1[7] = {57, 58, 60, 62, 64, 65, 67};
// lead 2
int notes2[6] = {65, 67, 69, 70, 72, 74};
// lead 3
int notes3[7] = {69, 70, 72, 74, 76, 77, 79};
// bass
int notes4[6] = {45, 46, 48, 50, 52, 53};
// drum
int notes5[6] = {60, 60, 62, 62, 62, 62};
int notes6[6] = {64, 64, 65, 65, 65, 65};
int notes7[6] = {67, 67, 69, 69, 69, 69};


int numNotes = 5; // cutoff for notes used 

int chMap[7] = {1, 1, 2, 3, 4, 4, 4}; 
int ctrlChannelMapPos[7] = {1, 1, 2, 3, 4, 4, 4};
int ctrlChannelMapNeg[7] = {1, 1, 2, 3, 4, 4, 4};

int ctrlMapPos[7] = {69, 94, 94, 94,    18, 50, 84};
int ctrlMapNeg[7] = {69, 93, 93, 93,     119, 119, 119};
int ctrlPosReset[7] = {40, 0, 0, 0,    0, 0, 0};
int ctrlNegReset[7] = {40, 0, 0, 0,    0, 0, 0};
int ctrlPosRange[7] = {127, 127, 127, 127,    127, 127, 127};
int ctrlNegRange[7] = {-127, 127, 127, 127,    127, 127, 127};

// not implemented yet
int ctrlMapPos2[7] = {0, 0, 0, 0,    0, 0, 0};
int ctrlMapNeg2[7] = {0, 0, 0, 0,    0, 0, 0};
int ctrlPosReset2[7] = {0, 0, 0, 0,    0, 0, 0};
int ctrlNegReset2[7] = {0, 0, 0, 0,    0, 0, 0};

// not implemented yet
int retriggerStart[7] = {0, 0, 0, 0, 500, 500, 500};
int retriggerMod[7] = {0, 0, 0, 0, 0, 0, 0};
int lastRetrigger[7] = {0, 0, 0, 0, 0, 0, 0};

int inputMapRange[2] = {50, 250};

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

int lastTick[7] = {0, 0, 0, 0, 0, 0, 0};
int currentMillis = 0;
int minTimeBetweenTicks = 1; // POSSIBLE FIX for lagginess; set to higher value

int lastTriggerDist[7] = {0, 0, 0, 0, 0, 0, 0};

// laser_control_pins: 12 - 9, 6 - 3
int laser_control_pins[7] = {24, 11, 12, 10, 9, 6, 5};
int extra_control_pin = 25;

void setup() {
  if (DEBUG) {
    Serial.begin(115200);
    Serial.println("Printing debug messages");
  }

  setupLaserControlPins();
  setupVolcaDrum();
  initControlChannels();

  // Initialize all 7 serial ports with baud rate 115200
  for (int i = 0; i < 7; i++) {
    Serials[i].begin(115200);
  }

  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);

  // Blink lasers to signal setup is done
  // ... (blink code, unchanged)
}

void loop() {
  readMidiMessages();

  if (sendMidi == 0) {
    return;
  }

  processLidarData();
}

void setupLaserControlPins() {
  // Set up laser control pins
  for (int i = 0; i < 7; i++) {
    pinMode(laser_control_pins[i], OUTPUT);
    digitalWrite(laser_control_pins[i], HIGH);
  }
}

void setupVolcaDrum() {
  // Setup Volca drum
  for (int wave_send_cc = 103; wave_send_cc < 109; wave_send_cc++) {
    usbMIDI.sendControlChange(wave_send_cc, 127, 4);
  }
}

void initControlChannels() {
  // Init control channels according to ctrlReset
  for (int i = 0; i < 4; i++) {
    usbMIDI.sendControlChange(ctrlChannelMapNeg[i], ctrlNegReset[i], i);
    usbMIDI.sendControlChange(ctrlChannelMapPos[i], ctrlPosReset[i], i);
  }
}

void readMidiMessages() {
  while (usbMIDI.read()) {
    // Read all incoming messages
  }
}

void processLidarData() {
  // Read data from channels
  for (int ch = 0; ch < num_channels; ch++) {
    while (Serials[ch].available()) {
      Get_Lidar_data(Serials[ch], ch);
    }

    if (dists[ch] == lastDists[ch] && millis() <= (lastTick[ch] + minTimeBetweenTicks)) {
      continue;
    }

    handleRetrigger(ch);
    updateAverage(ch);

    if (average[ch] != 0 && average[ch] < triggerDist) {
      if (noteLock[ch] == 0) {
        handleNoteOn(ch);
      } else {
        handleCtrlChange(ch);
      }
    } else {
      handleNoteOff(ch);
    }
  }
}

// ... (