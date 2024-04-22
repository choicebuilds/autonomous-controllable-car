#include "Mutex.h"
#include "mbed.h"
#include "rtos.h"
#include "Motor.h"
#include "Speaker.h"
#include "uLCD_4DGL.h"
#include <stdio.h>
#include <VL53L0X.h>
#include "Servo.h"


DevI2C          i2c(p28, p27);
DigitalOut      shdn_left(p26);
VL53L0X         l_left(&i2c, &shdn_left, NC);
uint32_t        distances[16];

Servo myservo(p22);

RawSerial  bluemod(p13, p14);
Motor lm(p23, p6, p5); // pwm, fwd, rev
Motor rm(p24, p8, p7); // pwm, fwd, rev
DigitalOut led(LED1); // on = manual; off = automatic;
DigitalIn echo(p20);
DigitalOut trigger(p19);
Speaker mySpeaker(p21);
// DigitalOut shdn(p26);
Serial pc(USBTX,USBRX);
uLCD_4DGL uLCD(p9,p10,p20); // serial tx, serial rx, reset pin;
Mutex mutex;
// Mutex speaker;
Mutex LCD;
volatile bool mode = 1; //1 = manual; 0 = automatic
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
                            // mutex.lock();
                            if (bhit=='1') {
                                mode = !mode;
                                led = !led;
                                mutex.lock();
                                lm.speed(0.0);
                                rm.speed(0.0);
                                mutex.unlock();
                            } else {
                                mode = mode;
                            }
                            // mutex.unlock();
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
                                //speaker.lock();
                                mySpeaker.PlayNote(sound, 0.5, 1.0);
                                //speaker.unlock();
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
                                //speaker.lock();
                                mySpeaker.PlayNote(sound, 0.5, 1.0);
                                //speaker.unlock();
                            } else {
                                speed = speed;
                            }
                            break;
                        case '5': //up dpad
                            if (mode) {
                                mutex.lock();
                                if (bhit=='1') {
                                    lm.speed(speed);
                                    rm.speed(speed);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                               mutex.unlock();
                            }
                            break;
                        case '6': //down dpad
                            if (mode) {
                                mutex.lock();
                                if (bhit=='1') {
                                    lm.speed(-speed);
                                    rm.speed(-speed);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                                mutex.unlock();
                            }
                            break;
                        case '7': //left dpad
                            if (mode) {
                                mutex.lock();
                                if (bhit=='1') {
                                    lm.speed(-speed);
                                    rm.speed(speed);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                                mutex.unlock();
                            }
                            break;
                        case '8': //right dpad
                            if (mode) {
                                mutex.lock();
                                if (bhit=='1') {
                                    lm.speed(speed);
                                    rm.speed(-speed);
                                } else {
                                    lm.speed(0.0);
                                    rm.speed(0.0);
                                }
                                mutex.unlock();
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        Thread::wait(300);
    }
}

void lidar(void const *args) {
    while (1) {
        l_left.get_distance(&distances[0]); 
        Thread::wait(200);
    }
}
void move(void const *args) {
    while(1) {
        if (!mode) {
            mutex.lock();
            lm.speed(0.5);
            rm.speed(0.5);
            mutex.unlock();
            //speaker.lock();
            if (((distances[0]) > 500)) {
                mySpeaker.PlayNote(sound, 0.5, 0.1);
            }
            else if (((distances[0]) < 500) && ((distances[0]) >= 400)) {
                mySpeaker.PlayNote(sound, 0.5, 0.3);
            }
            else if (((distances[0]) < 400) && ((distances[0]) >= 300)) {
                mySpeaker.PlayNote(sound, 0.5, 0.5);
            }
            else if (((distances[0]) < 300) && ((distances[0]) >= 200)) {
                mySpeaker.PlayNote(sound, 0.5, 1.0);
            }
            //speaker.unlock();
            while ((distances[0]) <= 150) {
                mutex.lock();
                lm.speed(-0.5);
                rm.speed(0.5);
                mutex.unlock();
                wait(0.5);
                // l_left.get_distance(&distances[0]);
            }
        }
        Thread::wait(500);
    }
}
void servo(void const *args) {
    while(1) {
        myservo = speed;
        Thread::wait(500);
    }
}


int main() {
    uLCD.cls();
    uLCD.color(RED);
    uLCD.filled_circle(64, 115, 10, BLUE);
    led = 1;

    l_left.VL53L0X_off();
    l_left.init_sensor(0x30);

    Thread thread2(blue);
    Thread thread3(lidar);
    Thread thread4(move);
    Thread thread5(servo);
    while(1) {
    
        if (((distances[0]) > 500)) {
            uLCD.filled_rectangle(32, 100, 96, 10, GREEN);
        }
        if (((distances[0]) >= 400)) {
            uLCD.filled_rectangle(32, 100, 96, 30, GREEN);
        } else {
            uLCD.filled_rectangle(32, 10, 96, 10, BLACK);
        }
        if (((distances[0]) >= 300)) {
            uLCD.filled_rectangle(32, 100, 96, 50, GREEN);
        } else {
            uLCD.filled_rectangle(32, 30, 96, 10, BLACK);
        }
        if (((distances[0]) >= 200)) {
            uLCD.filled_rectangle(32, 100, 96, 70, GREEN);
        } else {
            uLCD.filled_rectangle(32, 50, 96, 10, BLACK);
        }
        if (((distances[0]) >= 100)) {
            uLCD.filled_rectangle(32, 100, 96, 90, GREEN);
        } else {
            uLCD.filled_rectangle(32, 70, 96, 10, BLACK);
        }

        uLCD.locate(4,0);
        if (mode) {
            uLCD.printf("Manual   ");
        } else {
            uLCD.printf("Automatic");
        }

        Thread::wait(500);
    }
}