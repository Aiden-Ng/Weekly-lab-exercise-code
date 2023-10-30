//Keypad Define Guess from 0-50 (size = 51)
#define chosenArraySize 3  //Is actually 4
#define randomArraySize 50 
#define KA 10
#define KB 11
#define KC 12
#define KD 13
#define KAvail A5 

bool checkStatus = false; 

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
int intChosen =0;

void setup() {
  Serial.begin(9600);
  //Keypad 
  pinMode(KA, INPUT);
  pinMode(KB, INPUT);
  pinMode(KC, INPUT);
  pinMode(KD, INPUT);
  pinMode(KAvail, INPUT);

    //Keypad
  for (int i=0; i<chosenArraySize; i++) chosenArray[i]=' '; // Initializing the chosenArraySize into _ _ _ 0
  chosenArray[chosenArraySize] = 0;  //because this means nothing in ASCII

  //for (int i=0; i<(randomArraySize+1); i++) randomArray[i] = i; // Making randomArray into list of 0-50 

}

void loop() {
  if (digitalRead(KAvail)){
    keypadRead();
    Serial.println(intChosen);  
    delay(300);
  }
  if (digitalRead(KAvail)){
        keypadRead();
        delay(300);
      }
      if (0 <= intChosen <= 50 && checkStatus == true){
        indexChosen = findIndex(intChosen, randomArray,randomArraySize);
        if ( indexChosen == indexRandom ){
          Serial.println("You pass");
          checkStatus = false;
          break; //break from while loop 
          
        }
        else if( indexChosen > indexRandom ){
          indexLast = indexChosen;
          Serial.println("Overestimated"); 
          checkStatus = false;
        }
        else if ( indexChosen < indexRandom ){
          indexFirst = indexChosen;
          Serial.println("Underestimated"); 
          checkStatus = false;       
        }
        else {
          Serial.println("Error in finding index");
          checkStatus = false;
        }
      }
      else if (checkStatus == true){
        Serial.println("Value of out range please input again");
        checkStatus = false; 
      }
    
}



void keypadRead() {
  int k=0, ka=0, kb=0, kc=0, kd=0;
  ka = digitalRead(KA);
  kb = digitalRead(KB);
  kc = digitalRead(KC);
  kd = digitalRead(KD);

  k= ka + kb*2 + kc*4 + kd*8;
  if (k!= 3){ //F key 
    for (int i =0; i<(chosenArraySize - 1); i++)chosenArray[i]=chosenArray[i+1];
    chosenArray[chosenArraySize-1]=keyboardChar[k];
    chosenArray[chosenArraySize]=0;
  }
  else {
    conversionCharToInt();
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
