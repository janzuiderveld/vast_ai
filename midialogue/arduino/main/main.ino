// This code is wrriten to read data from Benewake mini-S LiDAR through UART physical interface.
// The code is tested with Teensy 4.1. It can also be used with ESP8266 by making slight modifications
// by Ibrahim Technical Support Engineer: ibrahim@benewake.com

// This code reads serial data from 7 different Benewake mini-S LiDAR sensors

// Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
//  Serial2.begin(115200);
//  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

// #TODO 
// handle -1, 0 and large output
// https://github.com/budryerson/TFMini-Plus - set framerate on sensors


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

//declare list for measured distances
int dists[7];
int dist_temp;

int i;
unsigned char uart[7][5];  /*----save data measured by LiDAR-------------*/
const int HEADER=0x59; /*----frame header of data package------------*/
int rec_debug_state[7] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}; /*----receive state for frame----------------*/

int note[7] = {0, 0, 0, 0, 0, 0, 0};

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("\nBenewake TFmini-S UART LiDAR Program");

  // begin all 7 serial ports with baud rate 115200
  for (int i = 0; i < 7; i++) {
    Serials[i].begin(115200);
  }


}

void loop() {
  // read from all 7 serial ports
  for (int i = 0; i < 7; i++) {
    while(Serials[i].available()){
      Get_Lidar_data(Serials[i], i);
    }

  for (int i = 0; i < 7; i++) {
    // if dists[i] < 100 and note is not 60, send midi note on
    if (dists[i] < 100 && note[i] != 60) {
      note[i] = 60;
      usbMIDI.sendNoteOn(60, 99, i);  // 60 = C4
      // delay(200);


      Serial.println(i);
      Serial.println("note on");

    // if dists[i] > 100 and note is not 0, send midi note off
    } else if (dists[i] > 100 && note[i] != 0) {
      note[i] = 0;
      usbMIDI.sendNoteOff(60, 0, i);  // 60 = C4
      Serial.println(i);
      Serial.println("note off");

    }

  }
}
}

void Get_Lidar_data(HardwareSerial serialX, int i){
if (serialX.available()) //check if serial port has data input
    {
    if(rec_debug_state[i] == 0x01)
        
        {  //the first byte
          uart[i][0]=serialX.read();
          if(uart[i][0] == 0x59)
              {
                // check = uart[i][0];
                rec_debug_state[i] = 0x02;
              }
        }
else if(rec_debug_state[i] == 0x02)
     {//the second byte
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

            dist_temp = uart[i][2] + uart[i][3]*256;//the distance

            // Serial.println(dist_temp);

            // Do not update bad data
            if (dist_temp < 0 || dist_temp > 1200) {
              while(serialX.available()){serialX.read();} // This part is added becuase some previous packets are there in the buffer so to clear serial buffer and get fresh data.
              rec_debug_state[i] = 0x01;
              return;
            }

            dists[i] = dist_temp;

            while(serialX.available()){serialX.read();} // This part is added becuase some previous packets are there in the buffer so to clear serial buffer and get fresh data.
            rec_debug_state[i] = 0x01;
            
            // Serial.println(dists[i]); //output measure distance value of LiDAR
            
           }
    
        }
}