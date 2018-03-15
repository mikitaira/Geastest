/*******************************************************************************
Copyright (c) 2017, Miki Taira

This file is part of Voxelyze.
Voxelyze is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Voxelyze is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
See <http://www.opensource.org/licenses/lgpl-3.0.html> for license details.
*******************************************************************************/

#ifndef VX_LOG_H
#define VX_LOG_H

#include "Voxelyze.h"
#include "VX_Voxel.h"
#include <iostream>
#include <fstream>
#include <string>


typedef unsigned int datObject;  
enum datComponent: unsigned int {
	POSITION = 1 << 0,
	CORNERPOSITION = 1 << 1,
	CORNEROFFSET = 1 << 2,
	VOXELSIZE = 1 << 3,
	BASESIZE = 1 << 4,
	DISPLACEMENT = 1 << 5,
	ORIENTATION = 1 << 6,
	FORCE = 1 << 7,
	MOMENT = 1 << 8,
	VELOCITY = 1 << 9,
	ANGULARVELOCITY = 1 << 10,
	PRESSURE = 1 << 11,
	KINETICENERGY = 1 << 12,
	VOLUMETRICSTRAIN = 1 << 13,
	TRANSVERSEAREA = 1 << 14,
	TRANSVERSESTRAINSUM = 1 << 15,
	DAMPINGMULTIPLIER = 1 << 16,
};

inline void datSet(datObject& obj, datComponent dat, bool set) { set ? obj |= dat : obj &= ~dat; }
inline bool datIsSet(datObject obj, datComponent dat) { return (dat&obj) ? true : false; }
inline datComponent operator++ (datComponent& dat) { return dat = (datComponent)(std::underlying_type<datComponent>::type(dat) << 1); }

struct newData
{
	double posX = 0;
	double posY = 0;
	double posZ = 0;
	double cornerPositionNNNX = 0;
	double cornerPositionNNNY = 0;
	double cornerPositionNNNZ = 0;
	double cornerPositionNNPX = 0;
	double cornerPositionNNPY = 0;
	double cornerPositionNNPZ = 0;
	double cornerPositionNPNX = 0;
	double cornerPositionNPNY = 0;
	double cornerPositionNPNZ = 0;
	double cornerPositionNPPX = 0;
	double cornerPositionNPPY = 0;
	double cornerPositionNPPZ = 0;
	double cornerPositionPNNX = 0;
	double cornerPositionPNNY = 0;
	double cornerPositionPNNZ = 0;
	double cornerPositionPNPX = 0;
	double cornerPositionPNPY = 0;
	double cornerPositionPNPZ = 0;
	double cornerPositionPPNX = 0;
	double cornerPositionPPNY = 0;
	double cornerPositionPPNZ = 0;
	double cornerPositionPPPX = 0;
	double cornerPositionPPPY = 0;
	double cornerPositionPPPZ = 0;
	double cornerOffsetNNNX = 0;
	double cornerOffsetNNNY = 0;
	double cornerOffsetNNNZ = 0;
	double cornerOffsetNNPX = 0;
	double cornerOffsetNNPY = 0;
	double cornerOffsetNNPZ = 0;
	double cornerOffsetNPNX = 0;
	double cornerOffsetNPNY = 0;
	double cornerOffsetNPNZ = 0;
	double cornerOffsetNPPX = 0;
	double cornerOffsetNPPY = 0;
	double cornerOffsetNPPZ = 0;
	double cornerOffsetPNNX = 0;
	double cornerOffsetPNNY = 0;
	double cornerOffsetPNNZ = 0;
	double cornerOffsetPNPX = 0;
	double cornerOffsetPNPY = 0;
	double cornerOffsetPNPZ = 0;
	double cornerOffsetPPNX = 0;
	double cornerOffsetPPNY = 0;
	double cornerOffsetPPNZ = 0;
	double cornerOffsetPPPX = 0;
	double cornerOffsetPPPY = 0;
	double cornerOffsetPPPZ = 0;
	double sizeX = 0;
	double sizeY = 0;
	double sizeZ = 0;
	double baseSizeX = 0;
	double baseSizeY = 0;
	double baseSizeZ = 0;
	double displacementX = 0;
	double displacementY = 0;
	double displacementZ = 0;
	double orientationAngle = 0;
	double orientationAxisX = 0;
	double orientationAxisY = 0;
	double orientationAxisZ = 0;
	Vec3D<double> vecOrientation = Vec3D<double>(0, 0, 0);
	double forceX = 0;
	double forceY = 0;
	double forceZ = 0;
	double momentX = 0;
	double momentY = 0;
	double momentZ = 0;
	double velocityX = 0;
	double velocityY = 0;
	double velocityZ = 0;
	double angularVelocityX = 0;
	double angularVelocityY = 0;
	double angularVelocityZ = 0;
	double pressure = 0;
	double kineticEnergy = 0;
	double volumetricStrain = 0;
	double transverseAreaX = 0;
	double transverseAreaY = 0;
	double transverseAreaZ = 0;
	double transverseStrainSumX = 0;
	double transverseStrainSumY = 0;
	double transverseStrainSumZ = 0;
	double dampingMultiplier = 0;
};

//! Voxelyze Logger
/*!

*/
class CVX_Logger
{
public:

	CVX_Logger();
	CVX_Logger(CVoxelyze* voxelyzeInstance); //!< Initializes this logger. @param[in] voxelyzeInstance The voxelyze instance to link this log object to.
	~CVX_Logger(); //!< Destructor

	void iniLogging();
	void doLogging(); //!< 

	void logTitleWriter(std::string datfile); //!< 
	void logWriter(std::string datfile, float currentSimTime, newData newLog); //!< 
	newData getNewData(CVX_Voxel* voxel);
	newData getAveNewData(int layer, CVoxelyze* vx);

	void setDirectory(const char* dirpath); //!< 

	void setOutAverage(bool onoff) { ave = onoff; }; //!< true:on,false:off.
	void setOutDetail(bool onoff) { detail = onoff; };  //!< true:on,false:off.
	void setDatObj(std::string onoff);  //!< 


private:
	CVoxelyze* vx;
	bool ave = false;
	bool detail = false;
	datObject datobj;

	std::string dirPath;

	friend class CVoxelyze; //give the main simulation class full access
	friend class CVX_Voxel; //give our voxel class direct access to all the members for quick access};

};

newData add(newData a, newData b);
newData div(newData a, int b);
#endif
