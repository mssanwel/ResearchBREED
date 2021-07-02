int a=8;
int b=12;
  
void setup() {
  // put your setup code here, to run once:
  pinMode(a, OUTPUT); //IN2
  pinMode(b, OUTPUT); //IN1
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
}
