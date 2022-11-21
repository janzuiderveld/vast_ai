// declare digital in array (2, 3, 18, 19, 20, 21)
int digitalIn[] = {4, 3, 2, 18, 19, 20, 21};

// declare digital out pin array
int digitalOutPin[] = {5, 6, 7, 8, 9, 10, 11};

// set output array init values
int outputArray[] = {0, 0, 0, 0, 0, 0, 0};

void setup() {
  // set all digital out pins to output
    for (int i = 0; i < 7; i++) {
        pinMode(digitalOutPin[i], OUTPUT);
    }

  // set all digital pins LOW
    for (int i = 0; i < 7; i++) {
        digitalWrite(digitalOutPin[i], LOW);
    }

  // set all digital in pins to input
    for (int i = 0; i < 7; i++) {
        pinMode(digitalIn[i], INPUT);
    }

  // attach interrupt to digital in pins
//   attachInterrupt(digitalPinToInterrupt(digitalIn[0]), digitalIn_ISR0, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(digitalIn[1]), digitalIn_ISR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(digitalIn[2]), digitalIn_ISR2, CHANGE);
    attachInterrupt(digitalPinToInterrupt(digitalIn[3]), digitalIn_ISR3, CHANGE);
    attachInterrupt(digitalPinToInterrupt(digitalIn[4]), digitalIn_ISR4, CHANGE);
    attachInterrupt(digitalPinToInterrupt(digitalIn[5]), digitalIn_ISR5, CHANGE);
    attachInterrupt(digitalPinToInterrupt(digitalIn[6]), digitalIn_ISR6, CHANGE);

    Serial.begin(9600);
    Serial.println("Setup complete");

}

void loop(){
    // check digitalin[0]
    if (digitalRead(digitalIn[0]) == HIGH) {
        digitalWrite(digitalOutPin[0], LOW);
    } else {
        digitalWrite(digitalOutPin[0], HIGH);
    }
}

void digitalIn_ISR1() {
    // set the digital out pin to the value of the digital in pin
    if (outputArray[1] == 0) {
        digitalWrite(digitalOutPin[1], HIGH);
        outputArray[1] = 1;
    } else {
        digitalWrite(digitalOutPin[1], LOW);
        outputArray[1] = 0;
    }

    
    Serial.println("digitalIn_ISR1");
}

void digitalIn_ISR2() {
    // set the digital out pin to the value of the digital in pin
    if (outputArray[2] == 0) {
        digitalWrite(digitalOutPin[2], HIGH);
        outputArray[2] = 1;
    } else {
        digitalWrite(digitalOutPin[2], LOW);
        outputArray[2] = 0;
    }

    Serial.println("digitalIn_ISR2");
}

void digitalIn_ISR3() {
    // set the digital out pin to the value of the digital in pin
    if (outputArray[3] == 0) {
        digitalWrite(digitalOutPin[3], HIGH);
        outputArray[3] = 1;
    } else {
        digitalWrite(digitalOutPin[3], LOW);
        outputArray[3] = 0;
    }
}

void digitalIn_ISR4() {
    // set the digital out pin to the value of the digital in pin
    if (outputArray[4] == 0) {
        digitalWrite(digitalOutPin[4], HIGH);
        outputArray[4] = 1;
    } else {
        digitalWrite(digitalOutPin[4], LOW);
        outputArray[4] = 0;
    }
}

void digitalIn_ISR5() {
    // set the digital out pin to the value of the digital in pin
    if (outputArray[5] == 0) {
        digitalWrite(digitalOutPin[5], HIGH);
        outputArray[5] = 1;
    } else {
        digitalWrite(digitalOutPin[5], LOW);
        outputArray[5] = 0;
    }
}

void digitalIn_ISR6() {
    // set the digital out pin to the value of the digital in pin
    if (outputArray[6] == 0) {
        digitalWrite(digitalOutPin[6], HIGH);
        outputArray[6] = 1;
    } else {
        digitalWrite(digitalOutPin[6], LOW);
        outputArray[6] = 0;
    }
}


