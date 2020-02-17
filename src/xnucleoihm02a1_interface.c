#include <stdio.h>

#include <pigpio.h>

#include "xnucleoihm02a1_interface.h"

int RPI_Board_Init(void) {

    int pigpioVer = gpioInitialise();

    if (pigpioVer < 0) {
        printf("Failed to initialize PIGPIO\n");
        return -1;
    } else {
        printf("Pigpio ver: %d\n", pigpioVer);
    }

    // 1MHz = 1us - 1 bit = 64 * 8 = 512us

    // 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
    // b  b  b  b  b  b  R  T  n  n  n  n  W  A u2 u1 u0 p2 p1 p0  m  m
    // 0  0  0  0  0  0  0  0  0  0  0  0  0  0  1  1  1  0  0  0   1  1 
    //                                           ux is 0 if the CEx GPIO is reserved for SPI (default) and 1 otherwise. 
    //                                                   px is 0 if CEx is active low (default) and 1 for active high. 

    // linux kernel:
    // #define BCM2835_SPI_CS_CSPOL		0x00000040
    // /* disable CSPOL which puts HW-CS into deselected state */
    // cs &= ~BCM2835_SPI_CS_CSPOL;
    // /* use the "undefined" chip-select as precaution */
    // #define BCM2835_SPI_CS_CS_10		0x00000002
    // #define BCM2835_SPI_CS_CS_01		0x00000001
    // cs |= BCM2835_SPI_CS_CS_10 | BCM2835_SPI_CS_CS_01;

    hspi = spiOpen(0, 32000, 0b11100011);

    if (hspi >= 0) {
        return 1;
    }
    
    printf("Failed to open SPI (%d)\n", hspi);

    return -1;
}

void RPI_Board_Close(void) {
    spiClose(hspi);
    gpioTerminate();
}
