/*
 * Copyright 2015 Google Inc.
 * Portions Copyright (C) 2014 NVIDIA Corporation
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __DRIVERS_POWER_MAX77620_H__
#define __DRIVERS_POWER_MAX77620_H__

#include <stdint.h>

#include "drivers/bus/i2c/i2c.h"
#include "drivers/power/power.h"

typedef struct Max77620Pmic
{
	PowerOps ops;
	int (*set_reg)(struct Max77620Pmic *me, uint8_t reg, uint8_t value);
	I2cOps *bus;
	uint8_t chip;
} Max77620Pmic;

Max77620Pmic *new_max77620_pmic(I2cOps *bus, uint8_t chip);

#endif /* __DRIVERS_POWER_MAX77620_H__ */
