//DC Motor Define 
#define dcMotorA1 9
#define dcMotorA2 10
#define dcMotorB1 3
#define dcMotorB2 5

void setup() {
  pinMode(dcMotorA1, OUTPUT);
  pinMode(dcMotorA2, OUTPUT);
  pinMode(dcMotorB1, OUTPUT);
  pinMode(dcMotorB2, OUTPUT);

}

void loop() {
  analogWrite(dcMotorA1, 100);
  analogWrite(dcMotorA2, LOW);
  analogWrite(dcMotorB1, 100);
  analogWrite(dcMotorB2, LOW);
  delay(2000);
  analogWrite(dcMotorA1, LOW);
  analogWrite(dcMotorA2, LOW);
  analogWrite(dcMotorB1, LOW);
  analogWrite(dcMotorB2, LOW);
  delay(3000);
}
