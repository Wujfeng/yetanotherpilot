/*
  June 2012

  BaseFlightPlus Rev -

  An Open Source STM32 Based Multicopter

  Includes code and/or ideas from:

  1)AeroQuad
  2)BaseFlight
  3)CH Robotics
  4)MultiWii
  5)S.O.H. Madgwick

  Designed to run on Naze32 Flight Control Board

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date                 Author          Notes
// 29/09/2011   SOH Madgwick    Initial release
// 02/10/2011   SOH Madgwick    Optimised for reduced CPU load
//
//=====================================================================================================

//----------------------------------------------------------------------------------------------------
// Variable declaration

extern float q0, q1, q2, q3;    // quaternion of sensor frame relative to auxiliary frame
extern float gyro_bias[3];
extern float Rot_matrix[9];
extern float euler[3];
extern float NED2BODY[3][3];
extern float BODY2NED[3][3];
extern float acc_ned[3];

//---------------------------------------------------------------------------------------------------
// Function declarations
void NonlinearSO3AHRSinit(float ax, float ay, float az, float mx, float my, float mz, float gx, float gy, float gz);
void NonlinearSO3AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, float twoKp, float twoKi, float twoKpMag, float twoKiMag, float dt);


//=====================================================================================================
// End of file
//=====================================================================================================
