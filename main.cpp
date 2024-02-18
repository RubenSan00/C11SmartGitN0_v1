/*!
******************************************************************************
Copyright (c) 2022 Analog Devices, Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc. and its
licensors. By using this software you agree to the terms of the associated
Analog Devices License Agreement.
******************************************************************************
 * @file    main.cpp
 * @brief   This file contains start code for EVAL-ADBMS6948 program
 * @version $Revision$
 * @date    $Date$
 *
 * Developed by: ADI BMS Applications Team, Bangalore, India
*****************************************************************************/

#include "common.h"
#include "print_result.h"
#include "Adbms6948_Applications.h"

// LED Blinking rate in milliseconds (Note: need to define the unit of a time duration i.e. seconds(s) or milliseconds(ms))
#define SLEEP_TIME                  500ms

// Initialise the digital pin that controls LED1
DigitalOut led(LED1);
// Initialise the serial object with TX and RX pins
BufferedSerial  serial_port(USBTX, USBRX);

DigitalOut chip_select(D10);                /* SPI chip select */
DigitalOut mosi(D11);                       /* MOSI */
DigitalIn miso(D12);                        /* MISO */
DigitalOut sclk(D13);                       /* SCK */

SPI spi(D11, D12, D13);                     /* SPI MOSI, MISO, SCK */

Timer timer;

Timeout timeout;

// The File handler is needed to allow printf commands to write to the terminal
FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}

// main() runs in its own thread in the OS
int main()
{
    // printing the actual Mbed OS version that this application is using to the console.
    printf(
        "Mbed OS version %d.%d.%d is what this applicaiton is currently using\r\n",
        MBED_MAJOR_VERSION,
        MBED_MINOR_VERSION,
        MBED_PATCH_VERSION
    );

    /* Initialize peripherals */
    chip_select = 1;
    spi.format(8,0);                /* 8bit data, CPOL-CPHA= 0*/
    spi.frequency(2000000);         /* SPI clock 2Mhz */
    serial_port.set_baud(115200);   /* UART baud rate is 115200 */

    PrintMenu();
    while (1) {
        int user_command = 0;

        /* Read from serial terminal */
        scanf("%d", &user_command);
        printf("Entered Command:%d\n", user_command);

        /* Execute user command*/
        ExecuteCommand(user_command);
    }
}
