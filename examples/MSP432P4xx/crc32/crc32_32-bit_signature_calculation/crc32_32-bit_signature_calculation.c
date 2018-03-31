/*
 * -------------------------------------------
 *    MSP432 DriverLib - v3_10_00_09 
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 * MSP432 CRC32 - CRC32 Calculation
 *
 * Description: In this example, the CRC32 module is used to calculate a CRC32
 * checksum in CRC32 bit mode. This is a very simple example that just pipes
 * in the contents of an arbitrary array to the CRC32 module and the result
 * is placed into a local variable. The debugger is used by the user to observe
 * the result.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 * Author: Timothy Logan
 ******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>

#include <stdbool.h>

#define CRC32_POLY              0x10211222

/* Statics */
static const uint8_t myData[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

int main(void)
{
    volatile uint32_t hwCalculatedCRC;
    uint32_t ii;

    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Setting the polynomial and feeding in the data */
    MAP_CRC32_setSeed(CRC32_POLY, CRC32_MODE);

    for(ii=0;ii<15;ii++)
        MAP_CRC32_set8BitData(myData[ii], CRC32_MODE);

    /* Getting the result from the hardware module */
    hwCalculatedCRC = MAP_CRC32_getResult(CRC32_MODE);

    /* Pause for the debugger */
    __no_operation();
}
