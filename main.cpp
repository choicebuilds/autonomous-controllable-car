#include "Mutex.h"
#include "mbed.h"
#include "rtos.h"
#include "Motor.h"
#include "Speaker.h"
#include "SDFileSystem.h"
#include "wave_player.h"
#include "XNucleo53L0A1.h"
#include <stdio.h>

RawSerial  bluemod(p13, p14);
Motor lm(p23, p6, p5); // pwm, fwd, rev
Motor rm(p24, p8, p7); // pwm, fwd, rev
DigitalOut led(LED1); // on = manual; off = automatic;
DigitalIn echo(p20);
DigitalOut trigger(p19);
Speaker mySpeaker(p21);
DigitalOut shdn(p26);
Serial pc(USBTX,USBRX);
Mutex mutex;
Mutex speaker;

#define VL53L0_I2C_SDA   p28
#define VL53L0_I2C_SCL   p27

static XNucleo53L0A1 *board=NULL;
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
                                speaker.lock();
                                mySpeaker.PlayNote(sound, 0.5, 1.0);
                                speaker.unlock();
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
                                speaker.lock();
                                mySpeaker.PlayNote(sound, 0.5, 1.0);
                                speaker.unlock();
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
        Thread::wait(200);
    }
}

void lidar(void const *args) {
    int status;
    uint32_t distance;
    DevI2C *device_i2c = new DevI2C(VL53L0_I2C_SDA, VL53L0_I2C_SCL);
    /* creates the 53L0A1 expansion board singleton obj */
    board = XNucleo53L0A1::instance(device_i2c, A2, D8, D2);
    shdn = 0; //must reset sensor for an mbed reset to work
    wait(0.1);
    shdn = 1;
    wait(0.1);
    /* init the 53L0A1 board with default values */
    status = board->init_board();
    while (status) {
        pc.printf("Fail");
        status = board->init_board();
    }
    //loop taking and printing distance
    pc.printf("%d", mode);
    while (1) {
        if (!mode) {
            pc.printf("D=%ld mm\r\n", distance);
            mutex.lock();
            lm.speed(0.5);
            rm.speed(0.5);
            mutex.unlock();
            status = board->sensor_centre->get_distance(&distance);
            if ((distance > 500)) {
                speaker.lock();
                mySpeaker.PlayNote(sound, 0.5, 0.1);
                speaker.unlock();
            }
            if ((distance < 500) && (distance >= 400)) {
                speaker.lock();
                mySpeaker.PlayNote(sound, 0.5, 0.3);
                speaker.unlock();
            }
            if ((distance < 400) && (distance >= 300)) {
                speaker.lock();
                mySpeaker.PlayNote(sound, 0.5, 0.5);
                speaker.unlock();
            }
            if ((distance < 300) && (distance >= 200)) {
                speaker.lock();
                mySpeaker.PlayNote(sound, 0.5, 1.0);
                speaker.unlock();
            }
            while (distance <= 150) {
                pc.printf("D=%ld mm\r\n", distance);
                //speaker make noise !!!!
                mutex.lock();
                lm.speed(-0.5);
                rm.speed(0.5);
                mutex.unlock();
                wait(0.5);
                status = board->sensor_centre->get_distance(&distance);
            }
        }
        Thread::wait(200);
    }
}

int main() {
    led = 1;
    Thread thread2(blue);
    Thread thread3(lidar);
    while(1) {
//        led = 1;
//        Thread::wait(200);
//        led = 0;
        Thread::wait(200);
    }
}