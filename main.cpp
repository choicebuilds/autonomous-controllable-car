#include "mbed.h"
#include "rtos.h"
#include "Motor.h"
#include "Speaker.h"
#include "SDFileSystem.h"
#include "wave_player.h"

RawSerial  bluemod(p13, p14);
Motor lm(p23, p6, p5); // pwm, fwd, rev
Motor rm(p24, p8, p7); // pwm, fwd, rev
DigitalOut led(LED1); // on = manual; off = automatic;
DigitalIn echo(p20);
DigitalOut trigger(p19);
Speaker mySpeaker(p21);
//Analog Out Jack
//On Board Speaker
volatile bool mode = 1; //1 = manual; 0 = automatic
volatile int distance = 0;
volatile int correction = 0;
Timer sonar;

void blue(void const *args) {
    char bnum = 0;
    char bhit = 0;
    while(1) {
        if (bluemod.getc()=='!') {
            if (bluemod.getc()=='B') { //button data packet
                bnum = bluemod.getc(); //button number
                bhit = bluemod.getc(); //button hit
                if (bluemod.getc()==char(~('!' + 'B' + bnum + bhit))) { //checksum OK?
                    switch (bnum) {
                        case '1': //set mode
                            if (bhit=='1') {
                                mode = !mode;
                                led = !led;
                                mySpeaker.PlayNote(969.0, 0.5, 1.0);
                                mySpeaker.PlayNote(800.0, 0.5, 1.0);
                            } else {
                                mode = mode;
                            }
                            break;
                        case '5': //up dpad
                            if (mode) {
                                if (bhit=='1') {
                                    lm.speed(1.0);
                                    rm.speed(1.0);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                            }
                            break;
                        case '6': //down dpad
                            if (mode) {
                                if (bhit=='1') {
                                    lm.speed(-1.0);
                                    rm.speed(-1.0);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                            }
                            break;
                        case '7': //left dpad
                            if (mode) {
                                if (bhit=='1') {
                                    lm.speed(-0.5);
                                    rm.speed(0.5);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                            }
                            break;
                        case '8': //right dpad
                            if (mode) {
                                if (bhit=='1') {
                                    lm.speed(0.5);
                                    rm.speed(-0.5);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        Thread::wait(200);
    }
}

void sonarThread(void const *args) {
    sonar.reset();
    sonar.start();
    while(echo == 2) {};
    sonar.stop();
    correction = sonar.read_us();
    while(1) {
        trigger = 1;
        sonar.reset();
        wait_us(10.0);
        trigger = 0;
        while(echo == 0) {};
        sonar.start();
        while(echo == 1) {};
        sonar.stop();
        distance = (sonar.read_us() - correction) / 58.0;
//        uLCD.printf("Distance: %5.0d", distance);
        Thread::wait(200);
    }
}

int main() {
    led = 1;
    Thread thread2(blue);
    Thread thread3(sonarThread);
    while(1) {
//        led = 1;
//        Thread::wait(200);
//        led = 0;
        Thread::wait(200);
    }
}