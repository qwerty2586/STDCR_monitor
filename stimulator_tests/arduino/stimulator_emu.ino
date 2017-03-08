#include <Arduino.h>

#define N 4
#define NO_OUTPUT -1
#define MIN 0
#define MAX 7

int led_pins[N] = {10,11,12,13};
int raspi_pins[N] = {4,5,6,7};

int current_output = NO_OUTPUT;

void pin_init() {
    for (int i=0;i<N;++i) {
        pinMode(led_pins[i],OUTPUT);
        pinMode(raspi_pins[i],OUTPUT);
    }
};

void set_pin(int pin,int state) {
    digitalWrite(led_pins[pin],state);
    digitalWrite(raspi_pins[pin],state);
    Serial.println(String(pin)+": "+String(state));

}

void show_output (int output) {
    if (output == NO_OUTPUT) {
        for (int i=0;i<N;++i) {
            set_pin(i,LOW);
            delay(1);
        }
    }
    else {
        set_pin(1,output % 2);
        set_pin(2,(output>>1) % 2);
        set_pin(3,(output>>2) % 2);
        delay(1);
        set_pin(0,HIGH);
    }
}


void setup() {
    Serial.begin(9600);
    pin_init();
    randomSeed(analogRead(0)*analogRead(1));
}

void loop() {
    show_output(random(MIN,MAX+1));
    delay(random(5,15)*100);
    show_output(NO_OUTPUT);
    delay(random(5,10)*100);
}