/* Copyright 2020 ILWrites
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/****************************/
/* VOLTAGE MEASUREMENT CODE */
/****************************/

#include "analog.h"

// Define number of samples to take voltage measurements
static int NUM_VSAMPLES = 10;

// Define voltage devision factor
static float VDF = 9.33;

// Define voltage reference
static float VREF = 5.00;

// Define ADC resolution
static int ADC_RES = 1024;

// Define values for voltage reading
static float VBAT;
static float RPIV;
