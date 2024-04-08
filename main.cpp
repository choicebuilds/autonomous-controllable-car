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
volatile float speed = 0.7;
volatile double sound = 440;
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
                            } else {
                                mode = mode;
                            }
                            break;
                        case '3': //decrease speed
                            if (bhit=='1') {
                                if (speed < 0.3) {
                                    speed = speed;
                                    sound = sound;
                                } else {
                                    speed -= 0.1;
                                    sound -= 30;
                                }
                                mySpeaker.PlayNote(sound, 0.5, 1.0);
                            } else {
                                speed = speed;
                            }
                            break;
                        case '4': //increase speed
                            if (bhit=='1') {
                                if (speed > 1.0) {
                                    speed = speed;
                                    sound = sound;
                                } else {
                                    speed += 0.1;
                                    sound += 30;
                                }
                                mySpeaker.PlayNote(sound, 0.5, 1.0);
                            } else {
                                speed = speed;
                            }
                            break;
                        case '5': //up dpad
                            if (mode) {
                                if (bhit=='1') {
                                    lm.speed(speed);
                                    rm.speed(speed);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                            }
                            break;
                        case '6': //down dpad
                            if (mode) {
                                if (bhit=='1') {
                                    lm.speed(-speed);
                                    rm.speed(-speed);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                            }
                            break;
                        case '7': //left dpad
                            if (mode) {
                                if (bhit=='1') {
                                    lm.speed(-speed);
                                    rm.speed(speed);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                            }
                            break;
                        case '8': //right dpad
                            if (mode) {
                                if (bhit=='1') {
                                    lm.speed(speed);
                                    rm.speed(-speed);
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