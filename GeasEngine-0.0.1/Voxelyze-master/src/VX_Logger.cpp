/*******************************************************************************
Copyright (c) 2017, Miki Taira

This file is part of Voxelyze.
Voxelyze is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Voxelyze is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
See <http://www.opensource.org/licenses/lgpl-3.0.html> for license details.
*******************************************************************************/
#include "VX_Logger.h"


CVX_Logger::CVX_Logger(void) {

}

CVX_Logger::CVX_Logger(CVoxelyze* voxelyzeInstance)
{
	vx = voxelyzeInstance;
}

CVX_Logger::~CVX_Logger(void) {

}

void CVX_Logger::iniLogging() {
	if (ave) {
		for (int i = 0; i < vx->maxLayer(); i++) {

			std::string datfile;
			datfile = datfile + dirPath;
			datfile = datfile + "/";
			datfile = datfile + "m" + std::to_string(i);
			datfile = datfile + ".dat";

			logTitleWriter(datfile);

		}
	}

	if (detail) {
		for (int i = 0; i < vx->voxelCount(); i++) {

			int x = vx->voxel(i)->indexX();
			int y = vx->voxel(i)->indexY();
			int z = vx->voxel(i)->indexZ();
			int l = vx->voxel(i)->indLayer();

			std::string datfile;
			datfile = datfile + dirPath;
			datfile = datfile + "/";
			datfile = datfile + "m" + std::to_string(l);
			datfile = datfile + "id" + std::to_string(i);
			datfile = datfile + "x" + std::to_string(x);
			datfile = datfile + "y" + std::to_string(y);
			datfile = datfile + "z" + std::to_string(z);
			datfile = datfile + ".dat";

			logTitleWriter(datfile);

		}
	}
}

void CVX_Logger::doLogging()
{
	if (ave) {
		for (int i = 0; i < vx->maxLayer(); i++) {

			std::string datfile;
			datfile = datfile + dirPath;
			datfile = datfile + "/";
			datfile = datfile + "m" + std::to_string(i);
			datfile = datfile + ".dat";

			newData newdata = getAveNewData(i, vx);
			logWriter(datfile, vx->currentSimTime(), newdata);

		}
	}

	if (detail) {
		for (int i = 0; i < vx->voxelCount(); i++) {

			int x = vx->voxel(i)->indexX();
			int y = vx->voxel(i)->indexY();
			int z = vx->voxel(i)->indexZ();
			int l = vx->voxel(i)->indLayer();

			std::string datfile;
			datfile = datfile + dirPath;
			datfile = datfile + "/";
			datfile = datfile + "m" + std::to_string(l);
			datfile = datfile + "id" + std::to_string(i);
			datfile = datfile + "x" + std::to_string(x);
			datfile = datfile + "y" + std::to_string(y);
			datfile = datfile + "z" + std::to_string(z);
			datfile = datfile + ".dat";

			newData newdata = getNewData(vx->voxel(i));
			logWriter(datfile, vx->currentSimTime(), newdata);

		}
	}
}

void CVX_Logger::logTitleWriter(std::string datfile)
{
	// ファイル出力ストリームの初期化
	std::ofstream ofs(datfile.c_str());


	//---------------------------------
	//simulation time
	//---------------------------------
	ofs << "simulation time";

	//---------------------------------
	//position
	//---------------------------------
	if (datIsSet(datobj, POSITION)) {
		ofs
			<< " " << "posX"
			<< " " << "posY"
			<< " " << "posZ";
	}

	//---------------------------------
	//cornerPosition
	//---------------------------------
	if (datIsSet(datobj, CORNERPOSITION)) {
		ofs
			<< " " << "cornerPositionNNNX"
			<< " " << "cornerPositionNNNY"
			<< " " << "cornerPositionNNNZ"
			<< " " << "cornerPositionNNPX"
			<< " " << "cornerPositionNNPY"
			<< " " << "cornerPositionNNPZ"
			<< " " << "cornerPositionNPNX"
			<< " " << "cornerPositionNPNY"
			<< " " << "cornerPositionNPNZ"
			<< " " << "cornerPositionNPPX"
			<< " " << "cornerPositionNPPY"
			<< " " << "cornerPositionNPPZ"
			<< " " << "cornerPositionPNNX"
			<< " " << "cornerPositionPNNY"
			<< " " << "cornerPositionPNNZ"
			<< " " << "cornerPositionPNPX"
			<< " " << "cornerPositionPNPY"
			<< " " << "cornerPositionPNPZ"
			<< " " << "cornerPositionPPNX"
			<< " " << "cornerPositionPPNY"
			<< " " << "cornerPositionPPNZ"
			<< " " << "cornerPositionPPPX"
			<< " " << "cornerPositionPPPY"
			<< " " << "cornerPositionPPPZ";
	}

	//---------------------------------
	//cornerOffset
	//---------------------------------
	if (datIsSet(datobj, CORNEROFFSET)) {
		ofs
			<< " " << "cornerOffsetNNNX"
			<< " " << "cornerOffsetNNNY"
			<< " " << "cornerOffsetNNNZ"
			<< " " << "cornerOffsetNNPX"
			<< " " << "cornerOffsetNNPY"
			<< " " << "cornerOffsetNNPZ"
			<< " " << "cornerOffsetNPNX"
			<< " " << "cornerOffsetNPNY"
			<< " " << "cornerOffsetNPNZ"
			<< " " << "cornerOffsetNPPX"
			<< " " << "cornerOffsetNPPY"
			<< " " << "cornerOffsetNPPZ"
			<< " " << "cornerOffsetPNNX"
			<< " " << "cornerOffsetPNNY"
			<< " " << "cornerOffsetPNNZ"
			<< " " << "cornerOffsetPNPX"
			<< " " << "cornerOffsetPNPY"
			<< " " << "cornerOffsetPNPZ"
			<< " " << "cornerOffsetPPNX"
			<< " " << "cornerOffsetPPNY"
			<< " " << "cornerOffsetPPNZ"
			<< " " << "cornerOffsetPPPX"
			<< " " << "cornerOffsetPPPY"
			<< " " << "cornerOffsetPPPZ";
	}

	//---------------------------------
	//size
	//---------------------------------
	if (datIsSet(datobj, VOXELSIZE)) {
		ofs
			<< " " << "sizeX"
			<< " " << "sizeY"
			<< " " << "sizeZ";
	}

	//---------------------------------
	//baseSize
	//---------------------------------
	if (datIsSet(datobj, BASESIZE)) {
		ofs
			<< " " << "baseSizeX"
			<< " " << "baseSizeY"
			<< " " << "baseSizeZ";
	}

	//---------------------------------
	//displacement
	//---------------------------------
	if (datIsSet(datobj, DISPLACEMENT)) {
		ofs
			<< " " << "displacementX"
			<< " " << "displacementY"
			<< " " << "displacementZ";
	}

	//---------------------------------
	//orientation
	//---------------------------------
	if (datIsSet(datobj, ORIENTATION)) {
		ofs
			<< " " << "orientationAngle"
			<< " " << "orientationAxisX"
			<< " " << "orientationAxisY"
			<< " " << "orientationAxisZ";
	}

	//---------------------------------
	//force
	//---------------------------------
	if (datIsSet(datobj, FORCE)) {
		ofs
			<< " " << "forceX"
			<< " " << "forceY"
			<< " " << "forceZ";
	}

	//---------------------------------
	//moment
	//---------------------------------
	if (datIsSet(datobj, MOMENT)) {
		ofs
			<< " " << "momentX"
			<< " " << "momentY"
			<< " " << "momentZ";
	}

	//---------------------------------
	//velocity
	//---------------------------------
	if (datIsSet(datobj, VELOCITY)) {
		ofs
			<< " " << "velocityX"
			<< " " << "velocityY"
			<< " " << "velocityZ";
	}

	//---------------------------------
	//angularVelocity
	//---------------------------------
	if (datIsSet(datobj, ANGULARVELOCITY)) {
		ofs
			<< " " << "angularVelocityX"
			<< " " << "angularVelocityY"
			<< " " << "angularVelocityZ";
	}

	//---------------------------------
	//pressure
	//---------------------------------
	if (datIsSet(datobj, PRESSURE)) {
		ofs << " " << "pressure";
	}

	//---------------------------------
	//kineticEnergy
	//---------------------------------
	if (datIsSet(datobj, KINETICENERGY)) {
		ofs << " " << "kineticEnergy";
	}

	//---------------------------------
	//volumetricStrain
	//---------------------------------
	if (datIsSet(datobj, VOLUMETRICSTRAIN)) {
		ofs << " " << "volumetricStrain";
	}

	//---------------------------------
	//transverseArea
	//---------------------------------
	if (datIsSet(datobj, TRANSVERSEAREA)) {
		ofs
			<< " " << "transverseAreaX"
			<< " " << "transverseAreaY"
			<< " " << "transverseAreaZ";
	}

	//---------------------------------
	//transverseStrainSum
	//---------------------------------
	if (datIsSet(datobj, TRANSVERSESTRAINSUM)) {
		ofs
			<< " " << "transverseStrainSumX"
			<< " " << "transverseStrainSumY"
			<< " " << "transverseStrainSumZ";
	}

	//---------------------------------
	//dampingMultiplier
	//---------------------------------
	if (datIsSet(datobj, DAMPINGMULTIPLIER)) {
		ofs << " " << "dampingMultiplier";
	}

	ofs << std::endl;

	ofs.close();

	return;
}

void CVX_Logger::logWriter(std::string datfile, float currentTime, newData newdata)
{
	// ファイル出力ストリームの初期化
	std::ofstream ofs(datfile.c_str(), std::ios::app);

	//---------------------------------
	//simulation time
	//---------------------------------
	ofs << currentTime;

	//---------------------------------
	//position
	//---------------------------------
	if (datIsSet(datobj, POSITION)) {
		ofs
			<< " " << newdata.posX
			<< " " << newdata.posY
			<< " " << newdata.posZ;
	}

	//---------------------------------
	//cornerPosition
	//---------------------------------
	if (datIsSet(datobj, CORNERPOSITION)) {
		ofs
			<< " " << newdata.cornerPositionNNNX
			<< " " << newdata.cornerPositionNNNY
			<< " " << newdata.cornerPositionNNNZ
			<< " " << newdata.cornerPositionNNPX
			<< " " << newdata.cornerPositionNNPY
			<< " " << newdata.cornerPositionNNPZ
			<< " " << newdata.cornerPositionNPNX
			<< " " << newdata.cornerPositionNPNY
			<< " " << newdata.cornerPositionNPNZ
			<< " " << newdata.cornerPositionNPPX
			<< " " << newdata.cornerPositionNPPY
			<< " " << newdata.cornerPositionNPPZ
			<< " " << newdata.cornerPositionPNNX
			<< " " << newdata.cornerPositionPNNY
			<< " " << newdata.cornerPositionPNNZ
			<< " " << newdata.cornerPositionPNPX
			<< " " << newdata.cornerPositionPNPY
			<< " " << newdata.cornerPositionPNPZ
			<< " " << newdata.cornerPositionPPNX
			<< " " << newdata.cornerPositionPPNY
			<< " " << newdata.cornerPositionPPNZ
			<< " " << newdata.cornerPositionPPPX
			<< " " << newdata.cornerPositionPPPY
			<< " " << newdata.cornerPositionPPPZ;
	}

	//---------------------------------
	//cornerOffset
	//---------------------------------
	if (datIsSet(datobj, CORNEROFFSET)) {
		ofs
			<< " " << newdata.cornerOffsetNNNX
			<< " " << newdata.cornerOffsetNNNY
			<< " " << newdata.cornerOffsetNNNZ
			<< " " << newdata.cornerOffsetNNPX
			<< " " << newdata.cornerOffsetNNPY
			<< " " << newdata.cornerOffsetNNPZ
			<< " " << newdata.cornerOffsetNPNX
			<< " " << newdata.cornerOffsetNPNY
			<< " " << newdata.cornerOffsetNPNZ
			<< " " << newdata.cornerOffsetNPPX
			<< " " << newdata.cornerOffsetNPPY
			<< " " << newdata.cornerOffsetNPPZ
			<< " " << newdata.cornerOffsetPNNX
			<< " " << newdata.cornerOffsetPNNY
			<< " " << newdata.cornerOffsetPNNZ
			<< " " << newdata.cornerOffsetPNPX
			<< " " << newdata.cornerOffsetPNPY
			<< " " << newdata.cornerOffsetPNPZ
			<< " " << newdata.cornerOffsetPPNX
			<< " " << newdata.cornerOffsetPPNY
			<< " " << newdata.cornerOffsetPPNZ
			<< " " << newdata.cornerOffsetPPPX
			<< " " << newdata.cornerOffsetPPPY
			<< " " << newdata.cornerOffsetPPPZ;
	}

	//---------------------------------
	//size
	//---------------------------------
	if (datIsSet(datobj, VOXELSIZE)) {
		ofs
			<< " " << newdata.sizeX
			<< " " << newdata.sizeY
			<< " " << newdata.sizeZ;
	}

	//---------------------------------
	//baseSize
	//---------------------------------
	if (datIsSet(datobj, BASESIZE)) {
		ofs
			<< " " << newdata.baseSizeX
			<< " " << newdata.baseSizeY
			<< " " << newdata.baseSizeZ;
	}

	//---------------------------------
	//displacement
	//---------------------------------
	if (datIsSet(datobj, DISPLACEMENT)) {
		ofs
			<< " " << newdata.displacementX
			<< " " << newdata.displacementY
			<< " " << newdata.displacementZ;
	}

	//---------------------------------
	//orientation
	//---------------------------------
	if (datIsSet(datobj, ORIENTATION)) {
		ofs
			<< " " << newdata.orientationAngle
			<< " " << newdata.orientationAxisX
			<< " " << newdata.orientationAxisY
			<< " " << newdata.orientationAxisZ;
	}

	//---------------------------------
	//force
	//---------------------------------
	if (datIsSet(datobj, FORCE)) {
		ofs
			<< " " << newdata.forceX
			<< " " << newdata.forceY
			<< " " << newdata.forceZ;
	}

	//---------------------------------
	//moment
	//---------------------------------
	if (datIsSet(datobj, MOMENT)) {
		ofs
			<< " " << newdata.momentX
			<< " " << newdata.momentY
			<< " " << newdata.momentZ;
	}

	//---------------------------------
	//velocity
	//---------------------------------
	if (datIsSet(datobj, VELOCITY)) {
		ofs
			<< " " << newdata.velocityX
			<< " " << newdata.velocityY
			<< " " << newdata.velocityZ;
	}

	//---------------------------------
	//angularVelocity
	//---------------------------------
	if (datIsSet(datobj, ANGULARVELOCITY)) {
		ofs
			<< " " << newdata.angularVelocityX
			<< " " << newdata.angularVelocityY
			<< " " << newdata.angularVelocityZ;
	}

	//---------------------------------
	//pressure
	//---------------------------------
	if (datIsSet(datobj, PRESSURE)) {
		ofs << " " << newdata.pressure;
	}

	//---------------------------------
	//kineticEnergy
	//---------------------------------
	if (datIsSet(datobj, KINETICENERGY)) {
		ofs << " " << newdata.kineticEnergy;
	}

	//---------------------------------
	//volumetricStrain
	//---------------------------------
	if (datIsSet(datobj, VOLUMETRICSTRAIN)) {
		ofs << " " << newdata.volumetricStrain;
	}

	//---------------------------------
	//transverseArea
	//---------------------------------
	if (datIsSet(datobj, TRANSVERSEAREA)) {
		ofs
			<< " " << newdata.transverseAreaX
			<< " " << newdata.transverseAreaY
			<< " " << newdata.transverseAreaZ;
	}

	//---------------------------------
	//transverseStrainSum
	//---------------------------------
	if (datIsSet(datobj, TRANSVERSESTRAINSUM)) {
		ofs
			<< " " << newdata.transverseStrainSumX
			<< " " << newdata.transverseStrainSumY
			<< " " << newdata.transverseStrainSumZ;
	}

	//---------------------------------
	//dampingMultiplier
	//---------------------------------
	if (datIsSet(datobj, DAMPINGMULTIPLIER)) {
		ofs << " " << newdata.dampingMultiplier;
	}


	ofs << std::endl;

	ofs.close();

	return;
}


newData CVX_Logger::getNewData(CVX_Voxel* voxel)
{
	newData newdata;

	//---------------------------------
	// position
	//---------------------------------
	newdata.posX = voxel->position().x;
	newdata.posY = voxel->position().y;
	newdata.posZ = voxel->position().z;

	//---------------------------------
	// cornerPosition
	//---------------------------------
	newdata.cornerPositionNNNX = voxel->cornerPosition(CVX_Voxel::NNN).x;
	newdata.cornerPositionNNNY = voxel->cornerPosition(CVX_Voxel::NNN).y;
	newdata.cornerPositionNNNZ = voxel->cornerPosition(CVX_Voxel::NNN).z;
	newdata.cornerPositionNNPX = voxel->cornerPosition(CVX_Voxel::NNP).x;
	newdata.cornerPositionNNPY = voxel->cornerPosition(CVX_Voxel::NNP).y;
	newdata.cornerPositionNNPZ = voxel->cornerPosition(CVX_Voxel::NNP).z;
	newdata.cornerPositionNPNX = voxel->cornerPosition(CVX_Voxel::NPN).x;
	newdata.cornerPositionNPNY = voxel->cornerPosition(CVX_Voxel::NPN).y;
	newdata.cornerPositionNPNZ = voxel->cornerPosition(CVX_Voxel::NPN).z;
	newdata.cornerPositionNPPX = voxel->cornerPosition(CVX_Voxel::NPP).x;
	newdata.cornerPositionNPPY = voxel->cornerPosition(CVX_Voxel::NPP).y;
	newdata.cornerPositionNPPZ = voxel->cornerPosition(CVX_Voxel::NPP).z;
	newdata.cornerPositionPNNX = voxel->cornerPosition(CVX_Voxel::PNN).x;
	newdata.cornerPositionPNNY = voxel->cornerPosition(CVX_Voxel::PNN).y;
	newdata.cornerPositionPNNZ = voxel->cornerPosition(CVX_Voxel::PNN).z;
	newdata.cornerPositionPNPX = voxel->cornerPosition(CVX_Voxel::PNP).x;
	newdata.cornerPositionPNPY = voxel->cornerPosition(CVX_Voxel::PNP).y;
	newdata.cornerPositionPNPZ = voxel->cornerPosition(CVX_Voxel::PNP).z;
	newdata.cornerPositionPPNX = voxel->cornerPosition(CVX_Voxel::PPN).x;
	newdata.cornerPositionPPNY = voxel->cornerPosition(CVX_Voxel::PPN).y;
	newdata.cornerPositionPPNZ = voxel->cornerPosition(CVX_Voxel::PPN).z;
	newdata.cornerPositionPPPX = voxel->cornerPosition(CVX_Voxel::PPP).x;
	newdata.cornerPositionPPPY = voxel->cornerPosition(CVX_Voxel::PPP).y;
	newdata.cornerPositionPPPZ = voxel->cornerPosition(CVX_Voxel::PPP).z;

	//---------------------------------
	// cornerOffset
	//---------------------------------
	newdata.cornerOffsetNNNX = voxel->cornerOffset(CVX_Voxel::NNN).x;
	newdata.cornerOffsetNNNY = voxel->cornerOffset(CVX_Voxel::NNN).y;
	newdata.cornerOffsetNNNZ = voxel->cornerOffset(CVX_Voxel::NNN).z;
	newdata.cornerOffsetNNPX = voxel->cornerOffset(CVX_Voxel::NNP).x;
	newdata.cornerOffsetNNPY = voxel->cornerOffset(CVX_Voxel::NNP).y;
	newdata.cornerOffsetNNPZ = voxel->cornerOffset(CVX_Voxel::NNP).z;
	newdata.cornerOffsetNPNX = voxel->cornerOffset(CVX_Voxel::NPN).x;
	newdata.cornerOffsetNPNY = voxel->cornerOffset(CVX_Voxel::NPN).y;
	newdata.cornerOffsetNPNZ = voxel->cornerOffset(CVX_Voxel::NPN).z;
	newdata.cornerOffsetNPPX = voxel->cornerOffset(CVX_Voxel::NPP).x;
	newdata.cornerOffsetNPPY = voxel->cornerOffset(CVX_Voxel::NPP).y;
	newdata.cornerOffsetNPPZ = voxel->cornerOffset(CVX_Voxel::NPP).z;
	newdata.cornerOffsetPNNX = voxel->cornerOffset(CVX_Voxel::PNN).x;
	newdata.cornerOffsetPNNY = voxel->cornerOffset(CVX_Voxel::PNN).y;
	newdata.cornerOffsetPNNZ = voxel->cornerOffset(CVX_Voxel::PNN).z;
	newdata.cornerOffsetPNPX = voxel->cornerOffset(CVX_Voxel::PNP).x;
	newdata.cornerOffsetPNPY = voxel->cornerOffset(CVX_Voxel::PNP).y;
	newdata.cornerOffsetPNPZ = voxel->cornerOffset(CVX_Voxel::PNP).z;
	newdata.cornerOffsetPPNX = voxel->cornerOffset(CVX_Voxel::PPN).x;
	newdata.cornerOffsetPPNY = voxel->cornerOffset(CVX_Voxel::PPN).y;
	newdata.cornerOffsetPPNZ = voxel->cornerOffset(CVX_Voxel::PPN).z;
	newdata.cornerOffsetPPPX = voxel->cornerOffset(CVX_Voxel::PPP).x;
	newdata.cornerOffsetPPPY = voxel->cornerOffset(CVX_Voxel::PPP).y;
	newdata.cornerOffsetPPPZ = voxel->cornerOffset(CVX_Voxel::PPP).z;

	//---------------------------------
	// size
	//---------------------------------
	newdata.sizeX = voxel->size().x;
	newdata.sizeY = voxel->size().y;
	newdata.sizeZ = voxel->size().z;

	//---------------------------------
	// baseSize
	//---------------------------------
	newdata.baseSizeX = voxel->baseSize().x;
	newdata.baseSizeY = voxel->baseSize().y;
	newdata.baseSizeZ = voxel->baseSize().z;

	//---------------------------------
	// displacement
	//---------------------------------
	newdata.displacementX = voxel->displacement().x;
	newdata.displacementY = voxel->displacement().y;
	newdata.displacementZ = voxel->displacement().z;

	//---------------------------------
	// orientation
	//---------------------------------
	newdata.orientationAngle = voxel->orientationAngle();
	newdata.orientationAxisX = voxel->orientationAxis().x;
	newdata.orientationAxisY = voxel->orientationAxis().y;
	newdata.orientationAxisZ = voxel->orientationAxis().z;
	newdata.vecOrientation = voxel->orientation().ToRotationVector();

	//---------------------------------
	// force
	//---------------------------------
	newdata.forceX = voxel->force().x;
	newdata.forceY = voxel->force().y;
	newdata.forceZ = voxel->force().z;

	//---------------------------------
	// moment
	//---------------------------------
	newdata.momentX = voxel->moment().x;
	newdata.momentY = voxel->moment().y;
	newdata.momentZ = voxel->moment().z;

	//---------------------------------
	// velocity
	//---------------------------------
	newdata.velocityX = voxel->velocity().x;
	newdata.velocityY = voxel->velocity().y;
	newdata.velocityZ = voxel->velocity().z;

	//---------------------------------
	// angularVelocity
	//---------------------------------
	newdata.angularVelocityX = voxel->angularVelocity().x;
	newdata.angularVelocityY = voxel->angularVelocity().y;
	newdata.angularVelocityZ = voxel->angularVelocity().z;

	//---------------------------------
	// pressure
	//---------------------------------
	newdata.pressure = voxel->pressure();

	//---------------------------------
	// kineticEnergy
	//---------------------------------
	newdata.kineticEnergy = voxel->kineticEnergy();

	//---------------------------------
	// volumetricStrain
	//---------------------------------
	newdata.volumetricStrain = voxel->volumetricStrain();

	//---------------------------------
	// transverseArea
	//---------------------------------
	newdata.transverseAreaX = voxel->transverseArea(CVX_Link::X_AXIS);
	newdata.transverseAreaY = voxel->transverseArea(CVX_Link::Y_AXIS);
	newdata.transverseAreaZ = voxel->transverseArea(CVX_Link::Z_AXIS);

	//---------------------------------
	// transverseStrainSum
	//---------------------------------
	newdata.transverseStrainSumX = voxel->transverseStrainSum(CVX_Link::X_AXIS);
	newdata.transverseStrainSumY = voxel->transverseStrainSum(CVX_Link::Y_AXIS);
	newdata.transverseStrainSumZ = voxel->transverseStrainSum(CVX_Link::Z_AXIS);

	//---------------------------------
	// dampingMultiplier
	//---------------------------------
	newdata.dampingMultiplier = voxel->dampingMultiplier();

	return newdata;
}

newData CVX_Logger::getAveNewData(int layer, CVoxelyze* vx)
{
	newData newdata;
	int count = 0;

	for (int i = 0; i < vx->voxelCount(); i++) {

		newData b;
		int l = vx->voxel(i)->indLayer();
		if (layer == l) {
			b = getNewData(vx->voxel(i));
			newdata = add(newdata, b);
			count++;
		}
	}

	if (count == 0) {
		return newdata;
	}

	newdata = div(newdata, count);
	Quat3D<double> orient;
	orient.FromRotationVector(newdata.vecOrientation);
	newdata.orientationAngle = orient.Angle();
	newdata.orientationAxisX = orient.Axis().x;
	newdata.orientationAxisY = orient.Axis().y;
	newdata.orientationAxisZ = orient.Axis().z;

	return newdata;
}

void CVX_Logger::setDirectory(const char* dirpath)
{ 
	dirPath = dirpath;
}

void  CVX_Logger::setDatObj(std::string onoff) {

	datobj = 0;
	datComponent edat = datComponent::POSITION;

	if (onoff.size() != 17) return;

	for (char c : onoff) {
		datSet(datobj, edat, (c == '1') ? true : false);
		++edat;
	}

}

newData add(newData a, newData b) {

	newData newdata;

	//---------------------------------
	//position
	//---------------------------------
	newdata.posX = a.posX + b.posX;
	newdata.posY = a.posY + b.posY;
	newdata.posZ = a.posZ + b.posZ;

	//---------------------------------
	//cornerPosition
	//---------------------------------
	newdata.cornerPositionNNNX = a.cornerPositionNNNX + b.cornerPositionNNNX;
	newdata.cornerPositionNNNY = a.cornerPositionNNNY + b.cornerPositionNNNY;
	newdata.cornerPositionNNNZ = a.cornerPositionNNNZ + b.cornerPositionNNNZ;
	newdata.cornerPositionNNPX = a.cornerPositionNNPX + b.cornerPositionNNPX;
	newdata.cornerPositionNNPY = a.cornerPositionNNPY + b.cornerPositionNNPY;
	newdata.cornerPositionNNPZ = a.cornerPositionNNPZ + b.cornerPositionNNPZ;
	newdata.cornerPositionNPNX = a.cornerPositionNPNX + b.cornerPositionNPNX;
	newdata.cornerPositionNPNY = a.cornerPositionNPNY + b.cornerPositionNPNY;
	newdata.cornerPositionNPNZ = a.cornerPositionNPNZ + b.cornerPositionNPNZ;
	newdata.cornerPositionNPPX = a.cornerPositionNPPX + b.cornerPositionNPPX;
	newdata.cornerPositionNPPY = a.cornerPositionNPPY + b.cornerPositionNPPY;
	newdata.cornerPositionNPPZ = a.cornerPositionNPPZ + b.cornerPositionNPPZ;
	newdata.cornerPositionPNNX = a.cornerPositionPNNX + b.cornerPositionPNNX;
	newdata.cornerPositionPNNY = a.cornerPositionPNNY + b.cornerPositionPNNY;
	newdata.cornerPositionPNNZ = a.cornerPositionPNNZ + b.cornerPositionPNNZ;
	newdata.cornerPositionPNPX = a.cornerPositionPNPX + b.cornerPositionPNPX;
	newdata.cornerPositionPNPY = a.cornerPositionPNPY + b.cornerPositionPNPY;
	newdata.cornerPositionPNPZ = a.cornerPositionPNPZ + b.cornerPositionPNPZ;
	newdata.cornerPositionPPNX = a.cornerPositionPPNX + b.cornerPositionPPNX;
	newdata.cornerPositionPPNY = a.cornerPositionPPNY + b.cornerPositionPPNY;
	newdata.cornerPositionPPNZ = a.cornerPositionPPNZ + b.cornerPositionPPNZ;
	newdata.cornerPositionPPPX = a.cornerPositionPPPX + b.cornerPositionPPPX;
	newdata.cornerPositionPPPY = a.cornerPositionPPPY + b.cornerPositionPPPY;
	newdata.cornerPositionPPPZ = a.cornerPositionPPPZ + b.cornerPositionPPPZ;

	//---------------------------------
	//cornerOffset
	//---------------------------------
	newdata.cornerOffsetNNNX = a.cornerOffsetNNNX + b.cornerOffsetNNNX;
	newdata.cornerOffsetNNNY = a.cornerOffsetNNNY + b.cornerOffsetNNNY;
	newdata.cornerOffsetNNNZ = a.cornerOffsetNNNZ + b.cornerOffsetNNNZ;
	newdata.cornerOffsetNNPX = a.cornerOffsetNNPX + b.cornerOffsetNNPX;
	newdata.cornerOffsetNNPY = a.cornerOffsetNNPY + b.cornerOffsetNNPY;
	newdata.cornerOffsetNNPZ = a.cornerOffsetNNPZ + b.cornerOffsetNNPZ;
	newdata.cornerOffsetNPNX = a.cornerOffsetNPNX + b.cornerOffsetNPNX;
	newdata.cornerOffsetNPNY = a.cornerOffsetNPNY + b.cornerOffsetNPNY;
	newdata.cornerOffsetNPNZ = a.cornerOffsetNPNZ + b.cornerOffsetNPNZ;
	newdata.cornerOffsetNPPX = a.cornerOffsetNPPX + b.cornerOffsetNPPX;
	newdata.cornerOffsetNPPY = a.cornerOffsetNPPY + b.cornerOffsetNPPY;
	newdata.cornerOffsetNPPZ = a.cornerOffsetNPPZ + b.cornerOffsetNPPZ;
	newdata.cornerOffsetPNNX = a.cornerOffsetPNNX + b.cornerOffsetPNNX;
	newdata.cornerOffsetPNNY = a.cornerOffsetPNNY + b.cornerOffsetPNNY;
	newdata.cornerOffsetPNNZ = a.cornerOffsetPNNZ + b.cornerOffsetPNNZ;
	newdata.cornerOffsetPNPX = a.cornerOffsetPNPX + b.cornerOffsetPNPX;
	newdata.cornerOffsetPNPY = a.cornerOffsetPNPY + b.cornerOffsetPNPY;
	newdata.cornerOffsetPNPZ = a.cornerOffsetPNPZ + b.cornerOffsetPNPZ;
	newdata.cornerOffsetPPNX = a.cornerOffsetPPNX + b.cornerOffsetPPNX;
	newdata.cornerOffsetPPNY = a.cornerOffsetPPNY + b.cornerOffsetPPNY;
	newdata.cornerOffsetPPNZ = a.cornerOffsetPPNZ + b.cornerOffsetPPNZ;
	newdata.cornerOffsetPPPX = a.cornerOffsetPPPX + b.cornerOffsetPPPX;
	newdata.cornerOffsetPPPY = a.cornerOffsetPPPY + b.cornerOffsetPPPY;
	newdata.cornerOffsetPPPZ = a.cornerOffsetPPPZ + b.cornerOffsetPPPZ;

	//---------------------------------
	//size
	//---------------------------------
	newdata.sizeX = a.sizeX + b.sizeX;
	newdata.sizeY = a.sizeY + b.sizeY;
	newdata.sizeZ = a.sizeZ + b.sizeZ;

	//---------------------------------
	//baseSize
	//---------------------------------
	newdata.baseSizeX = a.baseSizeX + b.baseSizeX;
	newdata.baseSizeY = a.baseSizeY + b.baseSizeY;
	newdata.baseSizeZ = a.baseSizeZ + b.baseSizeZ;

	//---------------------------------
	//displacement
	//---------------------------------
	newdata.displacementX = a.displacementX + b.displacementX;
	newdata.displacementY = a.displacementY + b.displacementY;
	newdata.displacementZ = a.displacementZ + b.displacementZ;

	//---------------------------------
	//orientation
	//---------------------------------
	newdata.orientationAngle = a.orientationAngle + b.orientationAngle;
	newdata.orientationAxisX = a.orientationAxisX + b.orientationAxisX;
	newdata.orientationAxisY = a.orientationAxisY + b.orientationAxisY;
	newdata.orientationAxisZ = a.orientationAxisZ + b.orientationAxisZ;
	newdata.vecOrientation = a.vecOrientation + b.vecOrientation;

	//---------------------------------
	//force
	//---------------------------------
	newdata.forceX = a.forceX + b.forceX;
	newdata.forceY = a.forceY + b.forceY;
	newdata.forceZ = a.forceZ + b.forceZ;

	//---------------------------------
	//moment
	//---------------------------------
	newdata.momentX = a.momentX + b.momentX;
	newdata.momentY = a.momentY + b.momentY;
	newdata.momentZ = a.momentZ + b.momentZ;

	//---------------------------------
	//velocity
	//---------------------------------
	newdata.velocityX = a.velocityX + b.velocityX;
	newdata.velocityY = a.velocityY + b.velocityY;
	newdata.velocityZ = a.velocityZ + b.velocityZ;

	//---------------------------------
	//angularVelocity
	//---------------------------------
	newdata.angularVelocityX = a.angularVelocityX + b.angularVelocityX;
	newdata.angularVelocityY = a.angularVelocityY + b.angularVelocityY;
	newdata.angularVelocityZ = a.angularVelocityZ + b.angularVelocityZ;

	//---------------------------------
	//pressure
	//---------------------------------
	newdata.pressure = a.pressure + b.pressure;

	//---------------------------------
	//kineticEnergy
	//---------------------------------
	newdata.kineticEnergy = a.kineticEnergy + b.kineticEnergy;

	//---------------------------------
	//volumetricStrain
	//---------------------------------
	newdata.volumetricStrain = a.volumetricStrain + b.volumetricStrain;

	//---------------------------------
	//transverseArea
	//---------------------------------
	newdata.transverseAreaX = a.transverseAreaX + b.transverseAreaX;
	newdata.transverseAreaY = a.transverseAreaY + b.transverseAreaY;
	newdata.transverseAreaZ = a.transverseAreaZ + b.transverseAreaZ;

	//---------------------------------
	//transverseStrainSum
	//---------------------------------
	newdata.transverseStrainSumX = a.transverseStrainSumX + b.transverseStrainSumX;
	newdata.transverseStrainSumY = a.transverseStrainSumY + b.transverseStrainSumY;
	newdata.transverseStrainSumZ = a.transverseStrainSumZ + b.transverseStrainSumZ;

	//---------------------------------
	//dampingMultiplier
	//---------------------------------
	newdata.dampingMultiplier = a.dampingMultiplier + b.dampingMultiplier;

	return newdata;

}

newData div(newData a, int b) {

	newData newdata;

	//---------------------------------
	//position
	//---------------------------------
	newdata.posX = a.posX / b;
	newdata.posY = a.posY / b;
	newdata.posZ = a.posZ / b;

	//---------------------------------
	//cornerPosition
	//---------------------------------
	newdata.cornerPositionNNNX = a.cornerPositionNNNX / b;
	newdata.cornerPositionNNNY = a.cornerPositionNNNY / b;
	newdata.cornerPositionNNNZ = a.cornerPositionNNNZ / b;
	newdata.cornerPositionNNPX = a.cornerPositionNNPX / b;
	newdata.cornerPositionNNPY = a.cornerPositionNNPY / b;
	newdata.cornerPositionNNPZ = a.cornerPositionNNPZ / b;
	newdata.cornerPositionNPNX = a.cornerPositionNPNX / b;
	newdata.cornerPositionNPNY = a.cornerPositionNPNY / b;
	newdata.cornerPositionNPNZ = a.cornerPositionNPNZ / b;
	newdata.cornerPositionNPPX = a.cornerPositionNPPX / b;
	newdata.cornerPositionNPPY = a.cornerPositionNPPY / b;
	newdata.cornerPositionNPPZ = a.cornerPositionNPPZ / b;
	newdata.cornerPositionPNNX = a.cornerPositionPNNX / b;
	newdata.cornerPositionPNNY = a.cornerPositionPNNY / b;
	newdata.cornerPositionPNNZ = a.cornerPositionPNNZ / b;
	newdata.cornerPositionPNPX = a.cornerPositionPNPX / b;
	newdata.cornerPositionPNPY = a.cornerPositionPNPY / b;
	newdata.cornerPositionPNPZ = a.cornerPositionPNPZ / b;
	newdata.cornerPositionPPNX = a.cornerPositionPPNX / b;
	newdata.cornerPositionPPNY = a.cornerPositionPPNY / b;
	newdata.cornerPositionPPNZ = a.cornerPositionPPNZ / b;
	newdata.cornerPositionPPPX = a.cornerPositionPPPX / b;
	newdata.cornerPositionPPPY = a.cornerPositionPPPY / b;
	newdata.cornerPositionPPPZ = a.cornerPositionPPPZ / b;

	//---------------------------------
	//cornerOffset
	//---------------------------------
	newdata.cornerOffsetNNNX = a.cornerOffsetNNNX / b;
	newdata.cornerOffsetNNNY = a.cornerOffsetNNNY / b;
	newdata.cornerOffsetNNNZ = a.cornerOffsetNNNZ / b;
	newdata.cornerOffsetNNPX = a.cornerOffsetNNPX / b;
	newdata.cornerOffsetNNPY = a.cornerOffsetNNPY / b;
	newdata.cornerOffsetNNPZ = a.cornerOffsetNNPZ / b;
	newdata.cornerOffsetNPNX = a.cornerOffsetNPNX / b;
	newdata.cornerOffsetNPNY = a.cornerOffsetNPNY / b;
	newdata.cornerOffsetNPNZ = a.cornerOffsetNPNZ / b;
	newdata.cornerOffsetNPPX = a.cornerOffsetNPPX / b;
	newdata.cornerOffsetNPPY = a.cornerOffsetNPPY / b;
	newdata.cornerOffsetNPPZ = a.cornerOffsetNPPZ / b;
	newdata.cornerOffsetPNNX = a.cornerOffsetPNNX / b;
	newdata.cornerOffsetPNNY = a.cornerOffsetPNNY / b;
	newdata.cornerOffsetPNNZ = a.cornerOffsetPNNZ / b;
	newdata.cornerOffsetPNPX = a.cornerOffsetPNPX / b;
	newdata.cornerOffsetPNPY = a.cornerOffsetPNPY / b;
	newdata.cornerOffsetPNPZ = a.cornerOffsetPNPZ / b;
	newdata.cornerOffsetPPNX = a.cornerOffsetPPNX / b;
	newdata.cornerOffsetPPNY = a.cornerOffsetPPNY / b;
	newdata.cornerOffsetPPNZ = a.cornerOffsetPPNZ / b;
	newdata.cornerOffsetPPPX = a.cornerOffsetPPPX / b;
	newdata.cornerOffsetPPPY = a.cornerOffsetPPPY / b;
	newdata.cornerOffsetPPPZ = a.cornerOffsetPPPZ / b;

	//---------------------------------
	//size
	//---------------------------------
	newdata.sizeX = a.sizeX / b;
	newdata.sizeY = a.sizeY / b;
	newdata.sizeZ = a.sizeZ / b;

	//---------------------------------
	//baseSize
	//---------------------------------
	newdata.baseSizeX = a.baseSizeX / b;
	newdata.baseSizeY = a.baseSizeY / b;
	newdata.baseSizeZ = a.baseSizeZ / b;

	//---------------------------------
	//displacement
	//---------------------------------
	newdata.displacementX = a.displacementX / b;
	newdata.displacementY = a.displacementY / b;
	newdata.displacementZ = a.displacementZ / b;

	//---------------------------------
	//orientation
	//---------------------------------
	newdata.orientationAngle = a.orientationAngle / b;
	newdata.orientationAxisX = a.orientationAxisX / b;
	newdata.orientationAxisY = a.orientationAxisY / b;
	newdata.orientationAxisZ = a.orientationAxisZ / b;
	newdata.vecOrientation = a.vecOrientation / b;

	//---------------------------------
	//force
	//---------------------------------
	newdata.forceX = a.forceX / b;
	newdata.forceY = a.forceY / b;
	newdata.forceZ = a.forceZ / b;

	//---------------------------------
	//moment
	//---------------------------------
	newdata.momentX = a.momentX / b;
	newdata.momentY = a.momentY / b;
	newdata.momentZ = a.momentZ / b;

	//---------------------------------
	//velocity
	//---------------------------------
	newdata.velocityX = a.velocityX / b;
	newdata.velocityY = a.velocityY / b;
	newdata.velocityZ = a.velocityZ / b;

	//---------------------------------
	//angularVelocity
	//---------------------------------
	newdata.angularVelocityX = a.angularVelocityX / b;
	newdata.angularVelocityY = a.angularVelocityY / b;
	newdata.angularVelocityZ = a.angularVelocityZ / b;

	//---------------------------------
	//pressure
	//---------------------------------
	newdata.pressure = a.pressure / b;

	//---------------------------------
	//kineticEnergy
	//---------------------------------
	newdata.kineticEnergy = a.kineticEnergy / b;

	//---------------------------------
	//volumetricStrain
	//---------------------------------
	newdata.volumetricStrain = a.volumetricStrain / b;

	//---------------------------------
	//transverseArea
	//---------------------------------
	newdata.transverseAreaX = a.transverseAreaX / b;
	newdata.transverseAreaY = a.transverseAreaY / b;
	newdata.transverseAreaZ = a.transverseAreaZ / b;

	//---------------------------------
	//transverseStrainSum
	//---------------------------------
	newdata.transverseStrainSumX = a.transverseStrainSumX / b;
	newdata.transverseStrainSumY = a.transverseStrainSumY / b;
	newdata.transverseStrainSumZ = a.transverseStrainSumZ / b;

	//---------------------------------
	//dampingMultiplier
	//---------------------------------
	newdata.dampingMultiplier = a.dampingMultiplier / b;

	return newdata;
}




