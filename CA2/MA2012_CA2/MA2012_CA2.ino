  /*
General question 
Will printing random(0,300) give me the same value everytime? If I did not do randomSeed 
if I do randomSeed(1) and randomSeed(2) and then I print, would it be almost similar
*/




//General define 

//Global Variable 
int intChosen =0;
int intRandom =0;

//PiezoBuzzer Define 
#define piezoBuzzer 8

//Solenoid Define  
#define solenoid 9

//DC Motor Define 
#define dcMotorA1 5
#define dcMotorA2 4
#define dcMotorB1 2
#define dcMotorB2 3

//Keypad Define Guess from 0-50 (size = 51)
#define chosenArraySize 3  //Is actually 4
#define randomArraySize 50 
#define KA 10
#define KB 11
#define KC 12
#define KD 13
#define KAvail A5 

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
//#include <LiquidCrystal.h> //For LCD

//LCD Define 
/*const int rs = 9, en = 10, d4 = 11, d5 = 12, d6 = 13, d7 = 14;
LiquidCrystal lcd (rs,en,d4,d5,d6,d7); //Creating object */


void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A4)); //Random empty AnalogInput
  

  //Piezobuzzer
  pinMode(piezoBuzzer,OUTPUT);
  
  //Solenoid
  pinMode(solenoid, OUTPUT);
  
  //DC Motor 
  pinMode(dcMotorA1, OUTPUT);
  pinMode(dcMotorA2, OUTPUT);
  pinMode(dcMotorB1, OUTPUT);
  pinMode(dcMotorB2, OUTPUT);

  //Keypad 
  pinMode(KA, INPUT);
  pinMode(KB, INPUT);
  pinMode(KC, INPUT);
  pinMode(KD, INPUT);
  pinMode(KAvail, INPUT);

  //Timer 
  TCCR1A = 0; //cuz its a 16 bit counter A = 8 bit 
  TCCR1B = 0; //cuz its a 16 bit counter B = 8 bit
  TCNT1 = 3036; //To reduce the offset 
  TCCR1B |= (1<<CS12);  //set prescalar to 256 
  TIMSK1 |= (1<<TOIE1); //enable overflow 

  //Keypad
  for (int i=0; i<chosenArraySize; i++) chosenArray[i]=' '; // Initializing the chosenArraySize into _ _ _ 0
  chosenArray[chosenArraySize] = 0;  //because this means nothing in ASCII

  for (int i=0; i<(randomArraySize+1); i++) randomArray[i] = i; // Making randomArray into list of 0-50 
}

ISR(TIMER1_OVF_vect) {
  Serial.println(millis());
  TCNT1 = 3036; //To reduce the offset 
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
    //Timer should start
    while(true){
      if (digitalRead(KAvail)){
        keypadRead();
      }
      if ( chosenArray[chosenArraySize] == 'F'){
        conversionCharToInt();
        indexChosen = findIndex(intChosen, randomArray,randomArraySize);
        if ( indexChosen == indexRandom ){
          Serial.println("You pass");
          digitalWrite(dcMotorA1, HIGH);
          digitalWrite(dcMotorA2, LOW);
          digitalWrite(dcMotorB1, HIGH);
          digitalWrite(dcMotorB2, LOW);
          delay(4000);
          digitalWrite(dcMotorA1, LOW);
          digitalWrite(dcMotorA2, LOW);
          digitalWrite(dcMotorB1, LOW);
          digitalWrite(dcMotorB2, LOW);          
          
          break; //break from while loop 
          
        }
        else if( indexChosen > indexRandom ){
          indexLast = indexChosen;
          Serial.println("Overestimated"); 
          for (int i=0; i<3; i++){
            tone(piezoBuzzer, 500, 100);
            delay(200);  
          }
        }
        else if ( indexChosen < indexRandom ){
          indexFirst = indexChosen;
          Serial.println("Underestimated"); 
          for (int i=0; i<2; i++){
            tone(piezoBuzzer, 100, 100);
            delay(200);  
          }  
        }
        else Serial.println("Error in finding index");
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
 intChosen = setPos;
 //Serial.println(intChosen);
}


int findIndex(int target, int array[], int length){
  for (int i =0; i < length; i++) {
    if (array[i] == target) return i; 
  }
  Serial.println("Target not inside the list");
  return -1;
}
