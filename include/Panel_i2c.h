/**
 * @addtogroup I2C
 * @{
 * @file      Panel_i2c.h
 * @author    Iowa State University uCube team
 * @author    Andy Snawerdt
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

/**
 * @class     i2c_t
 * @brief     A struct holding the base address of the i2c channel
 * @details   This struct is a 32 bit value representing the base address of the
 * 	          i2c channel. This struct is acting as an object to be used later.
 *
 */
typedef struct I2C_STRUCT
{
	/// The base address of the i2c channel.
	uint32_t base_addr;
}i2c_t;

i2c_t* I2C_init(uint32_t base_addr);
void I2C_send(i2c_t* this, uint8_t slave_addr, uint8_t data, uint8_t register_addr);
uint8_t I2C_receive(i2c_t* this, uint8_t slave_addr, uint8_t reg);


#endif /* PANEL_I2C_H_ */

/// @}

