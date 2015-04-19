/**
 * @addtogroup I2C
 * @{
 * @file      Panel_i2c.h
 * @author    Iowa State University uCube team
 * @author    Andy Snawerdt
 * @author    Jeramie Vens
 * @author    Kris Burney
 * @date      Mar 28, 2015
 * @brief     Prototype for I2C functions
 * @copyright Copyright (C) 2015 ISU uCube team
 * @details   This file contains all the prototypes for I2C functions
 *            such as intialization, sending data, and recieving data
 *            for the MSP430 controlling the display on the panels.
 */

/*
    Panel-display
    Copyright (C) 2015  Iowa State University uCube team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PANEL_I2C_H_
#define PANEL_I2C_H_

#include <stdint.h>

void I2C_init();
unsigned char volatile * I2C_receive(uint8_t slave_addr, char *array, uint16_t latch);

#endif /* PANEL_I2C_H_ */

/// @}

