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
  Serial1,
  Serial2,
  Serial3,
  Serial4,
  Serial5,
  Serial6,
  Serial7
};


int dist; /*----actual distance measurements of LiDAR---*/
int strength; /*----signal strength of LiDAR----------------*/
float temprature;
unsigned char check;        /*----save check value------------------------*/
int i;
unsigned char uart[9];  /*----save data measured by LiDAR-------------*/
const int HEADER=0x59; /*----frame header of data package------------*/
int rec_debug_state = 0x01;//receive state for frame


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
    Get_Lidar_data(Serials[i]);
  }
}

void Get_Lidar_data(HardwareSerial serialX, int dists[]){
if (serialX.available()) //check if serial port has data input
    {
    if(rec_debug_state == 0x01)
        {  //the first byte
          uart[0]=serialX.read();
          if(uart[0] == 0x59)
              {
                // check = uart[0];
                rec_debug_state = 0x02;
              }
        }
else if(rec_debug_state == 0x02)
     {//the second byte
      uart[1]=serialX.read();
      if(uart[1] == 0x59)
          {
            // check += uart[1];
            rec_debug_state = 0x03;
          }
      else{
            rec_debug_state = 0x01;
          }
      }

else if(rec_debug_state == 0x03)
        {
          uart[2]=serialX.read();
          // check += uart[2];
          rec_debug_state = 0x04;
        }
else if(rec_debug_state == 0x04)
        {
          uart[3]=serialX.read();
          // check += uart[3];
          rec_debug_state = 0x05;
        }
// else if(rec_debug_state == 0x05)
//         {
//           uart[4]=serialX.read();
//           check += uart[4];
//           rec_debug_state = 0x06;
//         }
// else if(rec_debug_state == 0x06)
//         {
//           uart[5]=serialX.read();
//           check += uart[5];
//           rec_debug_state = 0x07;
//         }
// else if(rec_debug_state == 0x07)
//         {
//           uart[6]=serialX.read();
//           check += uart[6];
//           rec_debug_state = 0x08;
//         }
// else if(rec_debug_state == 0x08)
//         {
//           uart[7]=serialX.read();
//           check += uart[7];
//           rec_debug_state = 0x09;
//         }

// else if(rec_debug_state == 0x09)
else if(rec_debug_state ==  0x05)
           {
            dist = uart[2] + uart[3]*256;//the distance
            Serial.print(dist); //output measure distance value of LiDAR
            while(serialX.available()){serialX.read();} // This part is added becuase some previous packets are there in the buffer so to clear serial buffer and get fresh data.
            rec_debug_state = 0x01;
           }
      
        // {
          // uart[8]=serialX.read();
          // if(uart[8] == check)
            // {
              
              // dist = uart[2] + uart[3]*256;//the distance
              // strength = uart[4] + uart[5]*256;//the strength
              // temprature = uart[6] + uart[7] *256;//calculate chip temprature
              // temprature = temprature/8 - 256;                              

              // Serial.print(dist); //output measure distance value of LiDAR
              // Serial.print('\n');
              // Serial.print("strength = ");
              // Serial.print(strength); //output signal strength value
              // Serial.print('\n');
              // Serial.print("\t Chip Temprature = ");
              // Serial.print(temprature);
              // Serial.println(" celcius degree"); //output chip temperature of Lidar                  

              // while(serialX.available()){serialX.read();} // This part is added becuase some previous packets are there in the buffer so to clear serial buffer and get fresh data.
              
            //  }
          // rec_debug_state = 0x01;

          
        }
    }
}
