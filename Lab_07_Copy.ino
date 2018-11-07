
// this sets all of the led pins
int led1 = 2;
int led2 = 4;
int led3 = 6;
int led4 = 5;

int leds[4] = {led1, led2, led3, led4}; // this is the array of leds

// this sets all of the button pins
int button1 = 34;
int button2 = 35;
int button3 = 36;
int button4 = 37;

int buttonPins[4] = {button4, button3, button2, button1}; // array of button pins


boolean lastButtonState[4] = {LOW, LOW, LOW, LOW}; // sets an array of last button states for all buttons
boolean buttonState[4] = {LOW, LOW, LOW, LOW}; // sets an array of button states for all buttons
boolean on[4] = {false, false, false, false}; // sets four on booleans to false

// sets switch one
int switch1 = 32;

// sets pot val for tempo
int tempo = 33;

// sets variables for current step and last step time
int currentStep = 0;
int lastStepTime = 0;

// sets variable for counter
int counter = 0;


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) { // this is a for loop running through all of the led pins and setting them as outputs
    pinMode(leds[i], OUTPUT);
  }
  pinMode(switch1, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
}


void sequenceNormal() { 
  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    //digitalWrite(leds[currentStep], LOW); //turn off the current led


    //turn stuff off for last step

    //increment
    currentStep = currentStep + 1;         //increment to the next step
    if (currentStep > 3) {
      currentStep = 0;
    }
    if (on[currentStep] == HIGH) { // if the current step reads in as high
      usbMIDI.sendNoteOff(60, 127, 1); // turn the last midi note off
      usbMIDI.sendNoteOn(60, 127, 1);  // turn a new midi note on


    }
    lastStepTime = millis();               //set lastStepTime to the current time
    // digitalWrite(leds[currentStep], HIGH); //turn on the new led



    //turn stuff on and set stuff for the current step
  }
}


void sequenceBack() { // this is the sequence back function ; it works the same as the other function
  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...

    currentStep = currentStep - 1;  // this is saying for every step it goes through, it should subtract one from the value
    if (currentStep < 0) { // if the value of currentStep is larger than 0
      currentStep = 3; // this is setting currentStep to three so it can count the leds back
    }
    
    if (on[currentStep] == HIGH) { // if current step reads high
      usbMIDI.sendNoteOff(60, 127, 1);  // turn off last midi note
      usbMIDI.sendNoteOn(60, 127, 1);  // turn on new midi note


    }

    //digitalWrite(leds[currentStep], LOW); //turn off the current led


    lastStepTime = millis();
    //digitalWrite(leds[currentStep], HIGH); //turn on the new led




  }
}
void sequence() { // this is a function determining that if a switch is flipped one way, the lights will blink forward and if the other way they'll blink back

  if (digitalRead(switch1) == HIGH) {
    sequenceNormal();
  }
  if (digitalRead(switch1) == LOW) {
    sequenceBack();
  }

}



void checkButtons() { // this is the function for checking if a button is pressed 
  for (int i = 0; i < 4; i++) { 
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPins[i]);
 
    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) { // if the button is pressed, sent on for the correct location in teh function as true 
      if (on[i] == false) {
        on[i] = true;
      } else if (on[i] == true) {
        on[i] = false;
      }
    }
  }
}
void setLeds() { // this is saying that is on is true, turn on the led in the same position of the array
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[currentStep], HIGH);

    if (on[i] == true) {
      digitalWrite(leds[i], HIGH);
    } else if (on[i] == false)
      digitalWrite(leds[i], LOW); {
    }

  }
}


void loop() {  // this just runs all of the functions
  tempo = analogRead(A14);
  sequence();
  checkButtons();
  setLeds();
}
