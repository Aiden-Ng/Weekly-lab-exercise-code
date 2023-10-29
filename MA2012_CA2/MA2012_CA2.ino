/*
General question 
Will printing random(0,300) give me the same value everytime? If I did not do randomSeed 
if I do randomSeed(1) and randomSeed(2) and then I print, would it be almost similar
*/


//General define 



//Global Variable 
int intChosen =0;
int intRandom =0;

//Solenoid Define  
#define solenoid 1

//DC Motor Define 
#define dcMotor1 2
#define dcMotor2 3

//Keypad Define Guess from 0-50 (size = 51)
#define chosenArraySize 3  //Is actually 4
#define randomArraySize 50 
#define KA 4
#define KB 5
#define KC 6
#define KD 7
#define KAvail 8 

//Keypad Variable
char keyboardChar[] = {'1','2','3','F',
                       '4','5','6','E',
                       '7','8','9','D',
                       'A','0','B','C'};
char chosenArray[chosenArraySize+1];
int randomArray[randomArraySize+1]; 
int indexFirst =0;
int indexLast; //will be define in setup using sizeof(randomArray) / sizeof(randomArray[0]
int indexChosen =0;
int indexRandom =0;

//LCD Library 
#include <LiquidCrystal.h> //For LCD

//LCD Define 
const int rs = 9, en = 10, d4 = 11, d5 = 12, d6 = 13, d7 = 14;
LiquidCrystal lcd (rs,en,d4,d5,d6,d7); //Creating object 


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  

  
  //Solenoid
  pinMode(solenoid, OUTPUT);
  
  //DC Motor 
  pinMode(dcMotor1, OUTPUT);
  pinMode(dcMotor2, OUTPUT);

  //Keypad 
  pinMode(KA, INPUT);
  pinMode(KB, INPUT);
  pinMode(KC, INPUT);
  pinMode(KD, INPUT);
  pinMode(KAvail, INPUT);

  for (int i=0; i<chosenArraySize; i++) chosenArray[i]=' '; // Initializing the chosenArraySize into _ _ _ 0
  chosenArray[chosenArraySize] = 0;  //because this means nothing in ASCII

  for (int i=0; i<(randomArraySize+1); i++) randomArray[i] = i; // Making randomArray into list of 0-50 
}

void loop() {
  int intRandom = random(0,51); //selects random number 0 <=indexRandom <51
  binarySearchTreeProcess();
}

void keypadRead() {
  int k=0, ka=0, kb=0, kc=0, kd=0;
  ka = digitalRead(KA);
  kb = digitalRead(KB);
  kc = digitalRead(KC);
  kd = digitalRead(KD);

  k= ka + kb*2 + kc*4 + kd*8;
  for (int i =0; i<(chosenArraySize - 1); i++)chosenArray[i]=chosenArray[i+1];
  chosenArray[chosenArraySize-1]=keyboardChar[k];
  chosenArray[chosenArraySize]=0;
}


void binarySearchTreeProcess() {
    indexFirst = 0;
    indexLast = sizeof(randomArray)/sizeof(randomArray[0]);
    indexRandom = findIndex(intRandom, randomArray, randomArraySize);
    while(true){
      if (digitalRead(KAvail)){
        keypadRead();
      }
      if ( chosenArray[chosenArraySize] == 'F'){
        indexChosen = findIndex(intChosen, randomArray,randomArraySize);
        if ( indexChosen == indexRandom ){
          Serial.println("You pass");
          break;
          //Conditions
        }
        else if( indexChosen > indexRandom ){
          indexLast = indexChosen;
          Serial.println("Overestimated"); 
          for (int i=0; i<3; i++){
            //tone(piezoBuzzer, 500, 100);
            delay(200);  
          }
        }
        else if ( indexChosen < indexRandom ){
          indexFirst = indexChosen;
          Serial.println("Underestimated"); 
          for (int i=0; i<2; i++){
            //tone(piezoBuzzer, 100, 100);
            delay(200);  
          }  
        }
      }
    }
}


void conversionCharToInt() {
 volatile int setPos =0;
 for (int i; i<chosenArraySize; i++){
  if ((chosenArray[i] - 48) >=0 && (chosenArray[i] - 48) <= 9){
    setPos *= 10;
    setPos = setPos + (chosenArray[i] - 48);
  }
 }
 intChosen = setPos;x`
 //Serial.println(intChosen);
}


int findIndex(int target, int array[], int length){
  for (int i =0; i < length; i++) {
    if (array[i] == target) return i; 
  }
  Serial.println("Target not inside the list");
  return -1;
}
