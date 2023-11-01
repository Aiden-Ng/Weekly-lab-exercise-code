  /*
General question 
Will printing random(0,300) give me the same value everytime? If I did not do randomSeed 
if I do randomSeed(1) and randomSeed(2) and then I print, would it be almost similar
*/

//General define 
int duration =0; //used as secods 
bool checkStatus = false; 

//Global Variable 
int intChosen =0;
int intRandom =0;

//PiezoBuzzer Define 
#define piezoBuzzer 8

//DC Motor Define 
#define dcMotorA1 11
#define dcMotorA2 6
#define dcMotorB1 3
#define dcMotorB2 5

//Keypad Define Guess from 0-50 (size = 51)
#define chosenArraySize 3  //Is actually 4
#define randomArraySize 50 
#define KA 4
#define KB 7
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
  TCNT1 = 3036; //To reduce the offset
  duration++;
}


void loop() {
  intRandom = random(0,51); //selects random number 0 <=indexRandom <51
  indexRandom = findIndex(intRandom, randomArray, (randomArraySize +1));
  /*analogWrite(dcMotorA1, 150);
  analogWrite(dcMotorA2, LOW);
  analogWrite(dcMotorB1, 150);
  analogWrite(dcMotorB2, LOW);
  delay(1000); //10 second before it restarts. 
  analogWrite(dcMotorA1, LOW);
  analogWrite(dcMotorA2, LOW);
  analogWrite(dcMotorB1, LOW);
  analogWrite(dcMotorB2, LOW);  
  delay(2000);
  analogWrite(dcMotorA1, LOW);
  analogWrite(dcMotorA2, 150);
  analogWrite(dcMotorB1, LOW);
  analogWrite(dcMotorB2, 150);
  delay(1000);
  analogWrite(dcMotorA1, LOW);
  analogWrite(dcMotorA2, LOW);
  analogWrite(dcMotorB1, LOW);
  analogWrite(dcMotorB2, LOW);  
  delay(2000);*/
  //Serial.println(indexRandom);
  binarySearchTreeProcess();
}

void keypadRead() {
  int k=0, ka=0, kb=0, kc=0, kd=0;
  ka = digitalRead(KA);
  kb = digitalRead(KB);
  kc = digitalRead(KC);
  kd = digitalRead(KD);

  k= ka + kb*2 + kc*4 + kd*8;
  if (k!=3){
    for (int i =0; i<(chosenArraySize - 1); i++)chosenArray[i]=chosenArray[i+1];
    chosenArray[chosenArraySize-1]=keyboardChar[k];
    chosenArray[chosenArraySize]=0;
  }
  else {
    conversionCharToInt();
    checkStatus = true; 
    Serial.println(intChosen);
  }
}


void binarySearchTreeProcess() {
    indexFirst = 0;
    indexLast = sizeof(randomArray)/sizeof(randomArray[0]);
    //Timer should start
    while(duration <=30){      
      Serial.println(chosenArray);
      if (digitalRead(KAvail)){
        keypadRead();
        delay(300);
      }
      if (checkStatus == true){
        indexChosen = findIndex(intChosen, randomArray,(randomArraySize+1));
        if (indexChosen == -1) {
          Serial.println("Please choose range from 0-50");
          delay(2000);
        }
        else if ( indexChosen == indexRandom ){
          Serial.println("You pass");
          analogWrite(dcMotorB1, 150);
          analogWrite(dcMotorB2, LOW);
          delay(2000); //10 second before it restarts. 
          analogWrite(dcMotorB1, LOW);
          analogWrite(dcMotorB2, LOW);
          delay(10000);
          analogWrite(dcMotorB1, LOW);
          analogWrite(dcMoto4B2, 150);
          delay(2000); 
          analogWrite(dcMotorB1, LOW);
          analogWrite(dcMotorB2, LOW);  
          
          checkStatus = false;
          for (int i=0; i<chosenArraySize; i++) chosenArray[i]=' '; // Initializing the chosenArraySize into _ _ _ 0
          chosenArray[chosenArraySize] = 0;  //because this means nothing in ASCII
          break; //break from while loop 
          
        }
        else if( indexChosen > indexRandom ){
          if (indexChosen <= indexLast) {
            indexLast = indexChosen;
            Serial.println("Overestimated"); 
            for (int i=0; i<3; i++){
              tone(piezoBuzzer, 500, 100);
              delay(200);  
            }
          }
          else ;
            Serial.println("It's less than "  + String(indexLast));
            delay(2000);
        }
        else if ( indexChosen < indexRandom ){
          if (indexChosen >= indexFirst) {
            indexFirst = indexChosen;
            Serial.println("Underestimated"); 
            for (int i=0; i<2; i++){
              tone(piezoBuzzer, 100, 100);
              delay(200);  
            }
          }
          else ;  
            Serial.println("It's more than "  +  String(indexFirst));
            delay(2000);
        }

        checkStatus = false;
        for (int i=0; i<chosenArraySize; i++) chosenArray[i]=' '; // Initializing the chosenArraySize into _ _ _ 0
        chosenArray[chosenArraySize] = 0;  //because this means nothing in ASCII
      }
    }
    Serial.println("DIE");    
    analogWrite(dcMotorA1, 100);
    analogWrite(dcMotorA2, LOW);
    delay(1300);
    analogWrite(dcMotorA1, LOW);
    analogWrite(dcMotorA2, LOW);
    delay(1000);
    analogWrite(dcMotorA1; LOW);
    analogWrite(dcMotorA2; 100);
    delay(1300);
    analogWrite(dcMotorA1; LOW);
    analogWrite(dcMotorA2; LOW);
    
    //resetting the variables 
    checkStatus = false;
    duration =0;
    for (int i=0; i<chosenArraySize; i++) chosenArray[i]=' '; // Initializing the chosenArraySize into _ _ _ 0
    chosenArray[chosenArraySize] = 0;  //because this means nothing in ASCII
    
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
   return -1;
}
