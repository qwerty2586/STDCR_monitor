const byte interruptPin1 = 2;
const byte interruptPin2 = 3;
unsigned long time;

void setup() {
    Serial.begin(9600);Serial.println("boot...");
    time = 0;
    pinMode(interruptPin1, INPUT);
    pinMode(interruptPin2, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin1), reset, RISING);
    attachInterrupt(digitalPinToInterrupt(interruptPin2), move, RISING);
}

void loop() {

}

void reset() {
    time = micros();
    Serial.print("event1: ");
    Serial.println(micros());
}

void move() {
    unsigned long time2 = micros();
    Serial.print("event2: ");
    Serial.println(micros());

}