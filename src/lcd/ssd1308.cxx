/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdexcept>
#include <string>
#include <unistd.h>

#include "hd44780_bits.hpp"
#include "ssd1308.hpp"

using namespace upm;

SSD1308::SSD1308(int bus_in, int addr_in) : m_i2c_lcd_control(bus_in)
{
    m_lcd_control_address = addr_in;
    m_name = "SSD1308";

    mraa::Result error = m_i2c_lcd_control.address(m_lcd_control_address);
    if (error != mraa::SUCCESS) {
        throw std::invalid_argument(std::string(__FUNCTION__) +
                                    ": I2c.address() failed");
        return;
    }

    m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_OFF); // display off
    usleep(4500);
    m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_ON); // display on
    usleep(4500);
    setNormalDisplay(); // set to normal display '1' is ON

    clear();
    setAddressingMode(PAGE);
}

SSD1308::~SSD1308()
{
}

mraa::Result
SSD1308::draw(uint8_t* data, int bytes)
{
    mraa::Result error = mraa::SUCCESS;

    setAddressingMode(HORIZONTAL);
    for (int idx = 0; idx < bytes; idx++) {
        m_i2c_lcd_control.writeReg(LCD_DATA, data[idx]);
    }

    return error;
}

/*
 * **************
 *  virtual area
 * **************
 */
mraa::Result
SSD1308::write(std::string msg)
{
    setAddressingMode(PAGE);
    for (std::string::size_type i = 0; i < msg.size(); ++i) {
        writeChar(msg[i]);
    }

    return mraa::SUCCESS;
}

mraa::Result
SSD1308::setCursor(int row, int column)
{
    mraa::Result error = mraa::SUCCESS;

    error = m_i2c_lcd_control.writeReg(LCD_CMD, BASE_PAGE_START_ADDR + row); // set page address
    error = m_i2c_lcd_control.writeReg(LCD_CMD,
                                       BASE_LOW_COLUMN_ADDR + (8 * column & 0x0F)); // set column
                                                                                    // lower address
    error = m_i2c_lcd_control.writeReg(LCD_CMD,
                                       BASE_HIGH_COLUMN_ADDR +
                                       ((8 * column >> 4) & 0x0F)); // set column higher address

    return error;
}

mraa::Result
SSD1308::clear()
{
    uint8_t columnIdx, rowIdx;

    m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_OFF); // display off
    for (rowIdx = 0; rowIdx < 8; rowIdx++) {
        setCursor(rowIdx, 0);

        // clear all columns
        for (columnIdx = 0; columnIdx < 16; columnIdx++) {
            writeChar(' ');
        }
    }
    m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_ON); // display on
    home();

    return mraa::SUCCESS;
}

mraa::Result
SSD1308::home()
{
    return setCursor(0, 0);
}

/*
 * **************
 *  private area
 * **************
 */
mraa::Result
SSD1308::writeChar(uint8_t value)
{
    mraa::Result rv;
    if (value < 0x20 || value > 0x7F) {
        value = 0x20; // space
    }

    for (uint8_t idx = 0; idx < 8; idx++) {
        rv = m_i2c_lcd_control.writeReg(LCD_DATA, BasicFont[value - 32][idx]);
    }

    return rv;
}

mraa::Result
SSD1308::setNormalDisplay()
{
    return m_i2c_lcd_control.writeReg(LCD_CMD,
                                      DISPLAY_CMD_SET_NORMAL_1308); // set to normal display '1' is
                                                                    // ON
}

mraa::Result
SSD1308::setAddressingMode(displayAddressingMode mode)
{
    mraa::Result rv;
    rv =m_i2c_lcd_control.writeReg(LCD_CMD, DISPLAY_CMD_MEM_ADDR_MODE); // set addressing mode
    rv =m_i2c_lcd_control.writeReg(LCD_CMD, mode);                      // set page addressing mode
    return rv;
}
