//Review LED connection
//If wrong colour go back 5th digital sensor 

#include <Arduino.h>
//Define General 
int displayStatus =0;
unsigned long currentMillis =0;
unsigned long previousMillis =0;
int gameStatus =0;
int initialPin =0;


// Define pins for LEDs
const int redPin = 2;
const int orangePin = 3;
const int greenPin = 4;
const int yellowPin = 5;


// Define pins for sensors
const int redSensorPin = 8;
const int orangeSensorPin = 9;
const int greenSensorPin = 10;
const int yellowSensorPin = 11;
//#define initialPin 7 // random please change 

// Define the sequence of colors in the game
const int gamePattern[] = {redPin, yellowPin, orangePin}; // Example pattern: Red -> Yellow -> Orange
const int sensorPattern[] = {redSensorPin, yellowSensorPin, orangeSensorPin};

// Define the time delay for each step and the time a player should stay at a color
const int stepDelay = 1000; // Time between colors
const int colorTimeout = 3000; // Time a player must stay at a color

// Define the time delay for the light show after winning
const int winLightShowDelay = 3000; // 3 seconds

// Variables to track game state
int currentStep = 0; // Current step in the pattern
unsigned long stepStartTime = 0; // Time when the step started
bool gameStarted = false;
bool playerWins = false;
bool patternShown = false; // To track whether the pattern has been shown

void setup() {
  // Initialize LED pins as outputs and turn them off
  pinMode(redPin, OUTPUT);
  digitalWrite(redPin, HIGH);
  pinMode(orangePin, OUTPUT);
  digitalWrite(orangePin, HIGH);
  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, HIGH);
  pinMode(yellowPin, OUTPUT);
  digitalWrite(yellowPin, HIGH);

  // Initialize sensor pins as inputs
  pinMode(redSensorPin, INPUT);
  pinMode(orangeSensorPin, INPUT);
  pinMode(greenSensorPin, INPUT);
  pinMode(yellowSensorPin, INPUT);

  // Other setup code
  Serial.begin(9600);
}

void loop() {
  //currentMillis = millis();
  Serial.println(currentMillis);
  if (initialPin == 0 && displayStatus ==0 ) {
    // Wait for the player to start the game (e.g., push a button or perform an action)
      //currentMillis = millis();
      //displayColorPattern();
      displayStatus =1;
      for (int i=0; i<3; i++){
        if(i==0){
          while(1){         
            if (digitalRead(sensorPattern[i]) == LOW){
              currentMillis = millis();        
              Serial.println(currentMillis);
              //Serial.println("i =0 true" + String(currentMillis));
              previousMillis = currentMillis;
              if (currentMillis - previousMillis > colorTimeout) continue;
            }
            else if (digitalRead(sensorPattern[i+1]) == LOW || digitalRead(sensorPattern[i+2]) == LOW){
              Serial.println("i =0 false" + String(currentMillis) + String(previousMillis));
              previousMillis = currentMillis;
              if (currentMillis - previousMillis > colorTimeout){
                gameStatus =-1; 
                break; 
              }
            }
          }
        }
        else if(i==1){
          if (digitalRead(sensorPattern[i]) == LOW){
            Serial.println("i =1 true");
            previousMillis = currentMillis;
            if (currentMillis - previousMillis > colorTimeout) continue;
            break;
          }
          else if (digitalRead(sensorPattern[i-1]) == LOW || digitalRead(sensorPattern[i+1]) == LOW){
            Serial.println("i =1 false");
            previousMillis = currentMillis;
            if (currentMillis - previousMillis > colorTimeout) break;
            
          }
        }
        else if(i==2){
          if (digitalRead(sensorPattern[i]) == LOW){
            Serial.println("i =2 true");
            previousMillis = currentMillis;
            if (currentMillis - previousMillis > colorTimeout) continue;
          }
          else if (digitalRead(sensorPattern[i-1]) == LOW || digitalRead(sensorPattern[i-2])== LOW){
            Serial.println("i =2 false");
            previousMillis = currentMillis;
            if (currentMillis - previousMillis > colorTimeout){
              gameStatus =-1;
              displayStatus =0; 
              break;
            }
          }
        }
        else if(gameStatus ==-1){
          gameStatus =0;
          displayStatus =0;
          break;
        }
         
      }
     
  }
  turnOffAllLeds();
}

void displayColorPattern() {
  for (int i = 0; i < sizeof(gamePattern) / sizeof(gamePattern[0]); i++) {
    int colorToDisplay = gamePattern[i];
    digitalWrite(colorToDisplay, LOW); // Turn on LED
    delay(stepDelay);
    digitalWrite(colorToDisplay, HIGH); // Turn off LED
    delay(stepDelay);
  }
}

void turnOffAllLeds() {
  digitalWrite(redPin, HIGH); // Turn off LED
  digitalWrite(orangePin, HIGH); // Turn off LED
  digitalWrite(greenPin, HIGH); // Turn off LED
  digitalWrite(yellowPin, HIGH); // Turn off LED
}

/*void celebrateWin() {
  // A simple light show to celebrate the win
  int iterations = 3; // Blink three times
  for (int i = 0; i < iterations; i++) {
    digitalWrite(redPin, LOW); // Turn on LED
    digitalWrite(orangePin, LOW); // Turn on LED
    digitalWrite(greenPin, LOW); // Turn on LED
    digitalWrite(yellowPin, LOW); // Turn on LED
    delay(500); // On for 500ms
    digitalWrite(redPin, HIGH); // Turn off LED
    digitalWrite(orangePin, HIGH); // Turn off LED
    digitalWrite(greenPin, HIGH); // Turn off LED
    digitalWrite(yellowPin, HIGH); // Turn off LED
    delay(500); // Off for 500ms
  }
  delay(winLightShowDelay);
  // You can add your own more complex light show patterns here
}*/
