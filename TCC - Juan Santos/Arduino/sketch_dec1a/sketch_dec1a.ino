int motorP = 9;
int tacoP = A0;

void setup() {
  pinMode(motorP, OUTPUT);
  pinMode(tacoP, INPUT);

  Serial.begin(9600);

  Serial.print("porta do motor: ");
  Serial.println(motorP);
  Serial.print("porta do taco: ");
  Serial.println(tacoP);
}

void loop() {
  digitalWrite(motorP, 250);
  Serial.println("Motor ligado!");
  Serial.print("Motor taco: ");
  Serial.println(analogRead(tacoP));
  delay(5000);
  /*
  digitalWrite(motorP, 0);
  Serial.println("Motor desligado!");
  Serial.print("Motor taco: ");
  Serial.println(analogRead(tacoP));
  delay(5000);
  */
}