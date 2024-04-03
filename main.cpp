#include "mbed.h"
#include "rtos.h"
#include "Motor.h"
#include "uLCD_4DGL.h"
#include "SDFileSystem.h"
#include "wave_player.h"

RawSerial  bluemod(p28,p27);
Motor lm(p23, p6, p5); // pwm, fwd, rev
Motor rm(p24, p8, p7); //pwm, fwd, rev
DigitalOut led(LED1);
Mutex mutex;
uLCD_4DGL uLCD(p13,p14,p15); // serial tx, serial rx, reset pin;
DigitalIn echo(p20);
DigitalOut trigger(p19);
//Analog Out Jack
//On Board Speaker
volatile bool mode=1; //1 = manual; 0 = automatic
volatile int distance=0;
volatile int correction=0;
Timer sonar;

void blue(void const *args) {
    char bnum=0;
    char bhit=0;
    while(1) {
        mutex.lock();
        if (mode) {
            uLCD.locate(1,2);
            uLCD.printf("\n\nManual Mode   ");
        } else {
            uLCD.locate(1,2);
            uLCD.printf("\n\nAutomatic Mode");
        }
        mutex.unlock();
        if (bluemod.getc()=='!') {
            if (bluemod.getc()=='B') { //button data packet
                bnum = bluemod.getc(); //button number
                bhit = bluemod.getc(); //button hit
                if (bluemod.getc()==char(~('!' + 'B' + bnum + bhit))) { //checksum OK?
                    switch (bnum) {
                        case '1': //set mode
                            if (bhit=='1') {
                                mode=!mode;
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
    while(echo==2) {};
    sonar.stop();
    correction = sonar.read_us();
    while(1) {
        trigger=1;
        sonar.reset();
        wait_us(10.0);
        trigger=0;
        while(echo==0) {};
        sonar.start();
        while(echo==1) {};
        sonar.stop();
        distance = (sonar.read_us() - correction) / 58.0;
        mutex.lock();
        uLCD.locate(2,6);
        uLCD.printf("Distance: %5.0d", distance);
        mutex.unlock();
        Thread::wait(200);
    }
}

int main() {
    uLCD.cls();
    uLCD.printf("Design Project");
    Thread thread2(blue);
    Thread thread3(sonarThread);
    while(1) {
        led = 1;
        Thread::wait(200);
        led = 0;
        Thread::wait(200);
        // mutex.lock();
        // uLCD.locate(2, 6);
        // mutex.unlock();
    //SD card needed with image and video files for last two demos
        //uLCD.media_init();
        // Thread::wait(200);
        // uLCD.set_sector_address(0x0000, 0x0000);
        //uLCD.display_video(0,0);
        //Thread::wait(100);
        //ulcd_mutex.unlock();
    }
}