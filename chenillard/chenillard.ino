void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
}

int state = 1;
void loop() {
  for (int i = 0; i < 5; ++i)
  digitalWrite(9 + i, state & (1 << i) ? HIGH : LOW);
  state = state << 1;
  if (state & (1 << 5))
    state = 1;
   int v = analogRead(A0);
  delay((v * 2) + 200);               
}
