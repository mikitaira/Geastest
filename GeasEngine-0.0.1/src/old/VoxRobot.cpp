#include "VoxRobot.h"
#define USE_OPEN_GL

using namespace std;

extern string strUserTimeStep;
float userTimeStep;

extern ofstream os;
extern bool savelog;
//extern bool savejson;
extern char inputJson[100];
extern char dirname[100];

int timeSpan = 5000;
int startTime;
bool appForce;
bool flag;

VoxRobot::VoxRobot()
{
	int startTime = 0;
	bool appForce = true;

	flag = true;
	firstStage = 10000000;
	secondStage = 10000000;
}

VoxRobot::~VoxRobot()
{
    //cout << "Called destructor :: VoxRobot()" << endl;

}


void VoxRobot::initVoxRobot()
{
	//cout << "Called constructor :: VoxRobot()" << endl;
	timeStep = 0;
	initVoxelyze();
	initVoxels();
	if(savelog){
		logOutPut();
	}

	float time;
	if(strUserTimeStep == ""){
		time = Vx.recommendedTimeStep();
	}
	else {
		time = stof(strUserTimeStep);
		userTimeStep = time;
	}
	setgSkipStep(time);
}


void VoxRobot::initVoxelyze()
{
    Vx.clear();
	Vx = CVoxelyze();
}


void VoxRobot::initVoxels()
{
	Vx.loadJSON(inputJson);

	createActVoxelList();
}

void VoxRobot::createActVoxelList() {

	// リストのメモリ確保
	vector<vector<ActVoxel>>().swap(actVoxelList);
	actVoxelList.resize(Vx.materialCount());
	for (int i = 0; i < Vx.materialCount(); i++) {
		actVoxelList[i].reserve(Vx.voxelCount());
	}

	// マテリアルマップのメモリ確保
	map<CVX_Material*, int> materialMap;
	for (int i = 0; i < Vx.materialCount(); i++) {
		materialMap[Vx.material(i)] = i;
	}


	// ActVoxelリスト（ボクセル,インデックス）を作成
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> bin(0, (int)(1.0 / userTimeStep));

	for (int i = 0; i < Vx.voxelCount(); i++) {
		CVX_Voxel* n = Vx.voxel(i);
		ActVoxel actVoxel;
		actVoxel.voxel = n;
		actVoxel.voxIndices = i;
		// 完全同期
		actVoxel.time = 0;
		//actVoxel.time = bin(mt);

		// 該当のマテリアルとActVoxelを関連付け
		actVoxelList[materialMap.find(n->material())->second].push_back(actVoxel);
	}

	vector<vector<ActVoxel>>(actVoxelList).swap(actVoxelList);

}

void VoxRobot::alterVoxels(int alterCount) {

	// Voxel削除＋シミュレーションリセット
	Vx.clearAllVoxels();

	// レイヤーの最大値を設定
	Vx.setMaxLayer(2);

	// Voxel追加
	const int intX = 5;
	const int intY = 5;
	const int intZ = 1;
	const int intSteps = 5;
	int posX = 0;
	int posY = 5 / 2 * (-1);

	int x;

	for (int k = 0; k < intSteps; k++) {
		x = posX + k*2;
		if (x > intX + posX) {
			break;
		}

		for (int l = k*intZ; l < (k + 1)*intZ; l++) {
			for (int i = x; i < intX + posX; i++) {
				for (int j = posY; j < intY + posY; j++) {
					Vx.setVoxel(Vx.material(0), i, j, l, 0);

				}
			}
		}
	}


	Vx.setVoxel(Vx.material(1), 3, 0, 3, 1);
	CVX_Voxel* pv = Vx.setVoxel(Vx.material(1), 4, 0, 3, 1);
	pv->external()->setForce(-1.0, 0, 0);


	// actVoxel作成
	createActVoxelList();

	////Jsonファイル出力
	//char outJsonFile[100];
	//if (savejson) {
	//	strcpy(outJsonFile, dirname);
	//	strcat(outJsonFile, "/sim");
	//	strcat(outJsonFile, to_string(alterCount).c_str());
	//	strcat(outJsonFile, "/output.xvl.json");
	//	Vx.saveJSON(outJsonFile);
	//}
}


#define SIMTIME 30
# define RULE 23
#define Nout 10
#define Nin Nout-2
#define Nout2 10*10
#define Nin2 (Nout-2) * (Nout-2) 
# define CN 100
# define noise 0.0/20.0
# define SAMPLING 1e-0
# define TMAX 10.0
int CA_timestep=0;
int CA_array[CN][CN];
double R;
double R_real;	// 蔵元モデルの秩序パラメータ用
double R_imagine;
int rule(int num, int l, int m, int r) {
	int a = l << 2, b = m << 1, c = r << 0;
	switch (a + b + c) {
	case 0:
		//cout << "value " << a + b + c << endl;
		//cout << "case 0 return " << ((num&(1 << 0)) >> 0) << endl;
		return (((num&(1 << 0)) >> 0) ? 1 : 0);
		break;
	case 1:
		return (((num&(1 << 1)) >> 1) ? 1 : 0);
		break;
	case 2:
		return (((num&(1 << 2)) >> 2) ? 1 : 0);
		break;
	case 3:
		return (((num&(1 << 3)) >> 3) ? 1 : 0);
		break;
	case 4:
		return (((num&(1 << 4)) >> 4) ? 1 : 0);
		break;
	case 5:
		return (((num&(1 << 5)) >> 5) ? 1 : 0);
		break;
	case 6:
		return (((num&(1 << 6)) >> 6) ? 1 : 0);
		break;
	case 7:
		return (((num&(1 << 7)) >> 7) ? 1 : 0);
		break;
	default:
		break;
	}
}

void initCA() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> bin(0, 1);
	for (int i = 0; i < CN; i++) {
		for (int j = 0; j < CN; j++) {
			CA_array[i][j] = 0;
			if (j == 0) CA_array[i][j] = bin(mt);
			//if (i == (int)(NN / 2) && j==0) CA_array[i][j] = 1;
		}
	}
}
void CA(int t)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> real(0.0, 1.0);

	if (t == 0) initCA();
	else {
		int n = t;
		if (t >= CN) n = CN - 1;
		for (int i = 0; i < CN; i++) {
			for (int j = n; j > 0; j--) {
				CA_array[i][j] = CA_array[i][j - 1];
			}
		}
		
		for (int i = 0; i < CN; i++) {
			if (i == 0) {
				CA_array[i][0] = rule(RULE, CA_array[CN - 1][1], CA_array[i][1], CA_array[i + 1][1]);
				
			}
			else if (i == CN - 1) {
				CA_array[i][0] = rule(RULE, CA_array[i - 1][1], CA_array[i][1], CA_array[0][1]);
				
			}
			else {
				CA_array[i][0] = rule(RULE, CA_array[i - 1][1], CA_array[i][1], CA_array[i + 1][1]);
				
			}
		}
		
	}

	/*
	for (int j = 0; j < NN; j++) {
	for (int i = 0; i < NN; i++) {
	cout << CA_array[i][j] << " ";
	}
	cout << endl;
	}
	cout << endl;
	*/
}
void randCA() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> bin(0, 1);
	for (int i = 0; i < CN; i++) {
		for (int j = 0; j < CN; j++) {
			CA_array[i][j] = bin(mt);

		}
	}
}
// Tは現在の周波数，
// samplingRateはあらかじめ決めた周波数
double VoxRobot::vibration(double T, double samplingRate, double phi) {
	double pulse;
	pulse = TMAX * sin(T / SAMPLING * 2 * PI + phi);
	return  pulse;
}
void VoxRobot::actuateVoxels(ActVoxel *avoxel, bool act, double phase) {
	float ctime = avoxel->time * userTimeStep;
	if (act) {
		avoxel->voxel->setTemperature(vibration(ctime, SAMPLING, phase));
		avoxel->time = avoxel->time + 1;
	}
	else {
		avoxel->voxel->setTemperature(0.0);
	//	avoxel->time = 0;	
	}


}

Vec3D<float> VoxRobot::fluid(ActVoxel *avoxel)
{

	double fluidDensity = 995.646;
	CVX_Voxel *voxel = avoxel->voxel;
	// velocity
	Vec3D<double> u = Vec3D<double>(0.0, 0.0, 0.0)/*.Normalized()*/;
	Vec3D<double> vg = voxel->velocity();
	Vec3D<double> vdif = vg - u;
	double        vdifLen2 = vdif.Length2();
	double        vdifLen = vdif.Length();
	Vec3D<double> nvdif = vdif.Normalized();

	// fluid density
	double rho = fluidDensity;
	double Re = 1.0;
	double Cd = 12.0 / Re;
	double area;

	double mu = 0.001; // viscosity if the fluid : 0.001 Pa*s
	double d = voxel->baseSizeAverage(); // characteristic length


    //  normal vector of each surface at this voxel
	Vec3D<double> force = { 0,0,0 };
	Quat3D<double> quat = voxel->orientation();
	Vec3D<double> n = { 0.0, 0.0, 0.0 };
	Vec3D<double> vn = { 0.0, 0.0, 0.0 };

	for (int i = 0; i<6; ++i) {
		if (voxel->link((CVX_Voxel::linkDirection)i) == NULL) {
			if (i == 0) {
				area = voxel->transverseArea((CVX_Link::linkAxis)0);
				n = Vec3D<double>(1.0, 0.0, 0.0);
			}
			else if (i == 1) {
				area = voxel->transverseArea((CVX_Link::linkAxis)0);
				n = Vec3D<double>(-1.0, 0.0, 0.0);
			}
			else if (i == 2) {
				area = voxel->transverseArea((CVX_Link::linkAxis)1);
				n = Vec3D<double>(0.0, 1.0, 0.0);
			}
			else if (i == 3) {
				area = voxel->transverseArea((CVX_Link::linkAxis)1);
				n = Vec3D<double>(0.0, -1.0, 0.0);
			}
			else if (i == 4) {
				area = voxel->transverseArea((CVX_Link::linkAxis)2);
				n = Vec3D<double>(0.0, 0.0, 1.0);
			}
			else {
				area = voxel->transverseArea((CVX_Link::linkAxis)2);
				n = Vec3D<double>(0.0, 0.0, -1.0);
			}

			n = quat.RotateVec3D(n);
			n.NormalizeFast();
			double angle = nvdif.Dot(n);
			vn = vdifLen2 * angle * n;
			if (angle <= 1.0 && angle > 0) {
				Vec3D<float> drag = Vec3D<float>(-1.0 * 0.5 * rho * area * Cd * vn);
				//      Vec3D<float> drag = Vec3D<float>(1.0 * 12.0 * mu * d * vdifLen * angle * n);
				force += drag;
			}
		}
	}

	//buoyancy
	//       force += Vec3D<float>(0,0,1) * (this->fluidDensity * (voxel->size().x *  voxel->size().y * voxel->size().z) * 9.80665);
	return (force);
//	voxel->external()->setForce(force);

	//    if(index==0) std::cout << timeStep <<  " drag force: "  <<  force.Length() << std::endl;
	//   if(index==0) std::cout << timeStep <<  " current force: " <<   voxel->force().Length() << std::endl << std::endl;
}

int patch;
void VoxRobot::recallRobot(int rep)
{
	// 処理を書き込む
	// テストコード
	/*if(gTimeStep<10)*/
	
	int update = 0.1 * SAMPLING / userTimeStep;
	if (timeStep % 1 == 0) {
		CA(CA_timestep);
		//randCA();
		CA_timestep++;
	}

	int count = 0;

	int logTiming = (update / 10);
	R_real = 0.0;
	R_imagine = 0.0;
	R = 0.0;
	ofstream output;
	if(timeStep==0) output.open("R.txt");
	else output.open("R.txt", ios::app);

	if (timeStep == 0) {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int> bin(0, (int)(CN / 2 - Nout / 2));
		patch = bin(mt);
		cout << "patch num : " << patch << endl;
	}

	/* 移送モデル */
	Vec3D<double> target1 = Vec3D<float>(0.4 / sqrt(3.0), 0.4 / sqrt(3.0), 0.4 / sqrt(3.0));
	Vec3D<double> target2 = Vec3D<float>(-0.4 / sqrt(3.0), 0.4 / sqrt(3.0), 0.4 / sqrt(3.0));
	Vec3D<double> target3 = Vec3D<float>(0.4 / sqrt(3.0), -0.4 / sqrt(3.0), 0.4 / sqrt(3.0));
	Vec3D<double> target4 = Vec3D<float>(0.4 / sqrt(3.0), 0.4 / sqrt(3.0), -0.4 / sqrt(3.0));
	Vec3D<double> target5 = Vec3D<float>(-0.4 / sqrt(3.0), -0.4 / sqrt(3.0), 0.4 / sqrt(3.0));
	Vec3D<double> target6 = Vec3D<float>(0.4 / sqrt(3.0), -0.4 / sqrt(3.0), -0.4 / sqrt(3.0));
	Vec3D<double> target7 = Vec3D<float>(-0.4 / sqrt(3.0), 0.4 / sqrt(3.0), -0.4 / sqrt(3.0));
	Vec3D<double> target8 = Vec3D<float>(-0.4 / sqrt(3.0), -0.4 / sqrt(3.0), -0.4 / sqrt(3.0));
	Vec3D<double> target9 = Vec3D<float>(0, 0, 0);
	Vec3D<double> target = target1;
	/*
	if (timeStep == 0) {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> real(-1.0, 1.0);
		target = Vec3D<double>(real(mt), real(mt), real(mt));
		target.NormalizeFast();
		target *= 0.4;
		cout << "target : " << target.x << " " << target.y << " " << target.z << endl;
	}
	*/
	bool sensing = true;

	for (int z = 0; z <= Nout; z++) {
		for (int y = -Nout; y <= Nout; y++) {
			for (int x = -Nout; x <= Nout; x++) {
				int v = x*x + y*y + z*z;
				if (v >= Nin2 && v <= Nout2) {
					// すべて同期して収縮
					//	actuateVoxels(&actVoxelList[0][count], (true), 0);
					// 移送モデルで収縮
					if (sensing) {
						CVX_Voxel*    voxel = actVoxelList[0][count].voxel;
						Quat3D<double> quat = voxel->orientation();
						Vec3D<double> n = { 0.0, 0.0, 0.0 };
						Vec3D<double> r = { 0.0, 0.0, 0.0 };
						double angle = 0.0;
						double sensor = 0.0;
						double active = 0.0;
						for (int i = 0; i<6; ++i) {
							if (voxel->link((CVX_Voxel::linkDirection)i) == NULL) {
								if (i == 0) {
									n = Vec3D<double>(1.0, 0.0, 0.0);
								}
								else if (i == 1) {
									n = Vec3D<double>(-1.0, 0.0, 0.0);
								}
								else if (i == 2) {
									n = Vec3D<double>(0.0, 1.0, 0.0);
								}
								else if (i == 3) {
									n = Vec3D<double>(0.0, -1.0, 0.0);
								}
								else if (i == 4) {
									n = Vec3D<double>(0.0, 0.0, 1.0);
								}
								else {
									n = Vec3D<double>(0.0, 0.0, -1.0);
								}
								n = quat.RotateVec3D(n);
								r = (target - voxel->position());
								n.NormalizeFast();
								r.NormalizeFast();
								angle = r.Dot(n);
								//if (angle <= 1.0 && angle >= 0.0) {
								if(angle > sensor) sensor = angle;
									//++active;
								//}
							}
						}
						//if (active > 0.0) sensor /= active;
						double phi = 0.0;						
						if (sensor > 0 && !isnan(acos(sensor))) {
							phi = acos(sensor);
						    actuateVoxels(&actVoxelList[0][count], (CA_array[(x + Nout) + patch][(y + Nout)] == 1),phi);
							//actuateVoxels(&actVoxelList[0][count], (true), phi);
						}
						else {
							actuateVoxels(&actVoxelList[0][count], (false), phi);
						}
						if (timeStep % logTiming == 0) {
							float ctime = actVoxelList[0][count].time * userTimeStep;
							R_real += cos(ctime / SAMPLING * 2 * PI+phi);
							R_imagine += sin(ctime / SAMPLING * 2 * PI+phi);
						}
					}

					// CAセルが活性化して収縮
					// actuateVoxels(&actVoxelList[0][count], (CA_array[(x + Nout) + patch][(y + Nout)] == 1),0);

					/*
					if (timeStep % logTiming == 0) {
						float ctime = actVoxelList[0][count].time * userTimeStep;
						R_real += cos(ctime / SAMPLING * 2 * PI);
						R_imagine += sin(ctime / SAMPLING * 2 * PI);
					}
					*/

					/*if(CA_array[(x + Nout) + patch][(y + Nout)] == 1)
						actVoxelList[0].at(count).voxel->external()->setForce(Vec3D<float>(0, 0, -1e-15));
					else
						actVoxelList[0].at(count).voxel->external()->clearForce();
					*/

					if (!(actVoxelList[0].at(count).voxel->isInterior()))
							actVoxelList[0].at(count).voxel->external()->setForce(fluid(&actVoxelList[0][count]));


					count++;
				}
			}
		}
	}
	
	if (timeStep % logTiming == 0) {
		Vec3D<double> centroid = Vec3D<double>(0, 0, 0);
		for (auto n : *(Vx.voxelList())) {
			centroid += n->position();
		}
		centroid /= (double)Vx.voxelCount();
		double diff = centroid.Dist(target);
		R = pow(R_real / Vx.voxelCount(), 2.0) + pow(R_imagine / Vx.voxelCount(), 2.0);
		output << timeStep*userTimeStep << " " << R << " " << diff << endl;;
		cout << "t = " << timeStep << "  R = " << R  << "  Diff = "<< diff <<  endl;;
	}
	/*
	for (int i = 0; i < NN; i++) {
		for (int j = 0; j < NN; j++) {
			actuateVoxels(&actVoxelList[0][i + NN*j], (CA_array[i+(int)(CN/2-NN/2)][j] == 1));
			
			if(CA_array[i + (int)(CN / 2 - NN / 2)][j] == 1)
			//if (CA_array[i][j] == 1)
				actVoxelList[0].at(i + NN*j).voxel->external()->setForce(Vec3D<float>(0, 0, -1e-5));
			else
				actVoxelList[0].at(i + NN*j).voxel->external()->clearForce();
		}
	}
	*/
	/*
	bool inverse = false;
	bool overMoment = false;
	Vec3D<double>averaveVel = Vec3D<double>(0, 0, 0);
	for (int i = 0; i < actVoxelList.size(); i++) {
		if (i == 6 || i == 7) {
			for (auto &n : actVoxelList[i]) {
				averaveVel += n.voxel->velocity();
				if (fabs(n.voxel->moment().y) > 1e-3) overMoment = true;
			}
		}

		if (i == 0 || i == 2) {
			for (auto &n : actVoxelList[i]) {
				CVX_Voxel* voxel = n.voxel;
				voxel->external()->clearForce();
			}
		}
	}

	if (averaveVel.x < 0.0 && timeStep >= 10000) inverse = true;
	else if (timeStep < 10000) inverse = false;
	if (flag) {
		if (inverse) {
			firstStage = timeStep;
			flag = false;
		}
	}
	*/


	float newTimeStep;
	if (strUserTimeStep == "") {
		newTimeStep = Vx.recommendedTimeStep();
	}
	else {
		newTimeStep = userTimeStep;
	}
	if (timeStep == 0) cout << "Time Step [s] : " << newTimeStep << endl;
	Vx.doTimeStep(newTimeStep);
	timeStep ++;
	if (timeStep*newTimeStep > SIMTIME) exit(1);
	//cout << timeStep << endl;

}

void VoxRobot::resetSimTime()
{
	timeStep = 0;
	Vx.resetTime();
}


void VoxRobot::iniExternal()
{

}

void VoxRobot::logOutPut() {

	os << "voxelSize: " << Vx.voxelSize() << endl;
	os << "gravityAcceleration: " << Vx.gravity() << endl;
	os << "voxelCount: " << Vx.voxelCount() << endl;
	for (int i = 0; i < Vx.materialCount(); i++) {
		CVX_Material* m = Vx.material(i);

		os << "----------------------------------------------------------" << endl;
		os << "name: " << m->name() << endl;
		os << "youngsModulus: " << scientific << m->youngsModulus() << endl;
		os << "density: " << m->density() << endl;
		os << "red: " << m->red() << endl;
		os << "green: " << m->green() << endl;
		os << "blue: " << m->blue() << endl;
		os << "alpha: " << m->alpha() << endl;
		os << "poissonsRatio: " << scientific << m->poissonsRatio() << endl;
		os << "CTE: " << scientific << m->cte() << endl;
		os << "staticFriction: " << m->staticFriction() << endl;
		os << "kineticFriction: " << m->kineticFriction() << endl;
		os << "collisionDamping: " << m->collisionDamping() << endl;
		os << "internalDamping: " << m->internalDamping() << endl;
		os << "globalDamping: " << m->globalDamping() << endl;
	}

}

void VoxRobot::setVx(CVoxelyze Vx)
{
    (this)->Vx = (Vx);
}

CVoxelyze VoxRobot::getVx()
{
    return this->Vx;
}

void VoxRobot::setMeshRender(CVX_MeshRender* meshRender)
{
    this->meshRender = meshRender;
}

CVX_MeshRender* VoxRobot::getMeshRender()
{
    return this->meshRender;
}

void VoxRobot::setMaterial(CVX_Material* vx_mat)
{
    this->vx_mat = vx_mat;
}

void VoxRobot::setDefaultDynamicRedMaterial(CVX_Material* material)
{
    matRed = material;
}
void VoxRobot::setDefaultDynamicGreenMaterial(CVX_Material* material)
{
    matGreen = material;
}
void VoxRobot::setDefaultDynamicBlueMaterial(CVX_Material* material)
{
    matBlue = material;
}

