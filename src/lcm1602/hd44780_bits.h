/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

// This file contains bit definitions for the HD44780 and compatible
// LCD controllers.  It is used primarily my the lcm1602 driver and
// it's derivatives.
//
// Those values (DATA, CMD) are specific to the implementation of the
// i2C expander in use, so may not be appropriate for inclusion into
// this file.  But for now, we will leave them here.

#include <stdint.h>

// commands
const uint8_t HD44780_CLEARDISPLAY = 0x01;
const uint8_t HD44780_RETURNHOME = 0x02;
const uint8_t HD44780_ENTRYMODESET = 0x04;
const uint8_t HD44780_DISPLAYCONTROL = 0x08;
const uint8_t HD44780_CURSORSHIFT = 0x10;
const uint8_t HD44780_FUNCTIONSET = 0x20;

// flags for display entry mode
const uint8_t HD44780_ENTRYRIGHT = 0x00;
const uint8_t HD44780_ENTRYLEFT = 0x02;
const uint8_t HD44780_ENTRYSHIFTINCREMENT = 0x01;
const uint8_t HD44780_ENTRYSHIFTDECREMENT = 0x00;

// flags for display on/off control
const uint8_t HD44780_DISPLAYON = 0x04;
const uint8_t HD44780_DISPLAYOFF = 0x00;
const uint8_t HD44780_CURSORON = 0x02;
const uint8_t HD44780_CURSOROFF = 0x00;
const uint8_t HD44780_BLINKON = 0x01;
const uint8_t HD44780_BLINKOFF = 0x00;

// flags for display/cursor shift
const uint8_t HD44780_DISPLAYMOVE = 0x08;
const uint8_t HD44780_MOVERIGHT = 0x04;
const uint8_t HD44780_MOVELEFT = 0x00;

// flags for function set
const uint8_t HD44780_8BITMODE = 0x10;
const uint8_t HD44780_4BITMODE = 0x00;
const uint8_t HD44780_2LINE = 0x08;
const uint8_t HD44780_1LINE = 0x00;
const uint8_t HD44780_5x10DOTS = 0x04;
const uint8_t HD44780_5x8DOTS = 0x00;

// flags for CGRAM
const uint8_t HD44780_SETCGRAMADDR = 0x40;

// may be implementation specific
const uint8_t HD44780_EN = 0x04; // Enable bit
const uint8_t HD44780_RW = 0x02; // Read/Write bit
const uint8_t HD44780_RS = 0x01; // Register select bit
const uint8_t HD44780_DATA = 0x40;
const uint8_t HD44780_CMD = 0x80;

const uint8_t HD44780_BACKLIGHT = 0x08;
const uint8_t HD44780_NOBACKLIGHT = 0x00;
