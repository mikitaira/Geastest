#include "VoxGraphics.h"

using namespace std;

// Global variables : to avoid multiple definition
int   gTimeStep;
int   gSkipStep;
bool  gCamRot;                  // Camera rotation
float gCamAng;                  // Camera angle
float gCamSpeed;                // Camera speed
Vec3D <float> gCamPos;          // Camera position
Vec3D <float> gCamForward;      // Camera forward direction
Vec3D <float> gCamRight;		// Camera right direction
int   gMode;				    // Orthographic: gMode = 0, Perspective: gMode = 1
int   gGridSize;

VoxRobot* voxRobot;
CVX_Logger logger;


int mouse_l = 0;
int mouse_m = 0;
int mouse_r = 0;

float mpos[2];  // mpos[0]=x座標, mpos[1]=y座標

double trans[3] = { 0.0, 0.0, 0.0 };
double theta[3] = { 0.0, 0.0, 0.0 };
double angle[3] = { 0.0, 0.0, 0.0 };

bool spaceKey;
bool pause = false;
bool reset = false;
bool start = true;
bool keyup;
bool keydown;
bool keyleft;
bool keyright;


GLCapture glCapture;
GLCapture glCapture1;
GLCapture glCapture2;
GLCapture glCapture3;

GLfloat lightPosition[4] = { 0.25f, 1.0f, 0.25f, 0.0f };
GLfloat lightDiffuse[3] = { 1.0f, 1.0f, 1.0f };
GLfloat lightAmbient[3] = { 0.25f, 0.25f, 0.25f };
GLfloat lightSpecular[3] = { 1.0f, 1.0f, 1.0f };

GLfloat diffuse[3] = { 1.0f, 1.0f, 0.0f };
GLfloat ambient[3] = { 0.25f, 0.25f, 0.25f };
GLfloat specular[3] = { 1.0f, 1.0f, 1.0f };
GLfloat shininess[1] = { 32.0f };

int GLframe = 0; //フレーム数
int GLtimenow = 0;//経過時間
int GLtimebase = 0;//計測開始時間

int mainWindowW = 300;
int mainWindowH = 300;

int subWindowW = 300;
int subWindowH = 300;

int mainWindowPosX = 10;
int mainWindowPosY = 10;

vector<int> vecWinID; //ウィンドウID
map<int, bool> winDisp;  //ウィンドウ表示状態

extern char dirname[100];
int alterCount = 0;
extern ofstream os;
extern string movie;
extern bool saveave;
extern bool savedetail;
extern bool creatdir;
extern string saveitems;
extern string strUserSkipStep;

// extern を外したらビルド成功
string dispLayer;
string colorScale;
bool pressureView;
bool fixview = true;
bool setviewpoint = true;

winObj winobj, ctlWinobj;

Vec3D<double> fixViewPoint = Vec3D<double>(0, 0, 0);


float curcolorScale;
float extColorScale;

VoxGraphics::VoxGraphics() {
	voxRobot = new VoxRobot();
	cout << "new VoxRobot()" << endl;
}

void VoxGraphics::initVoxGraphics(int argc, char *argv[])
{
	int pos[2];
	int size[2];
	char name[50];

    glutInit(&argc, argv);

	gTimeStep = 0;
	gCamRot = true;
	gCamAng = 90.0f;
	gCamSpeed = 1.0f;
	gCamPos = Vec3D<float>{ 0.0f, 0.055f, 0.2f }; // Camera position
												 //gCamPos = Vec3D<float>{ 0.0f, 0.02f, 0.0f }; // Camera position
	gCamForward = Vec3D<float>(0.0f, 0.0f, 0.0f) - gCamPos;	// Camera pose
	gCamRight = Vec3D<float>(1.0f, 0.0f, 0.0f);	// Camera right direction
	gMode = 1;
	gGridSize = 100;


	setWinObj(movie);
	//------------------------------------------
	// Main Window
	//------------------------------------------
	int mainId;
	strcpy(name, "Soft-bodied Agent");
	pos[0] = mainWindowPosX;
	pos[1] = mainWindowPosY;
	size[0] = mainWindowW;
	size[1] = mainWindowH;
	createWindow(mainId, name, pos, size, paintGL, reshapeMain);
	vecWinID.push_back(mainId);
	winDisp[mainId] = true;
	

	//------------------------------------------
	// Sub Window 1
	//------------------------------------------
	if (winIsSet(winobj, EXT_X)) {
		int id;
		strcpy(name, "Cell Automaton on SBA");
		pos[0] = mainWindowPosX + mainWindowW;
		pos[1] = mainWindowPosY;
		size[0] = subWindowW;
		size[1] = subWindowH;
		createWindow(id, name, pos, size, paintExternalForceX, reshapeExternalForceX);
		vecWinID.push_back(id);
		winDisp[id] = true;
	}

	//------------------------------------------
	// Sub Window 2
	//------------------------------------------
	if (winIsSet(winobj, EXT_Y)) {
		int id;
		strcpy(name, "Displacement in SBA - top view");
		pos[0] = mainWindowPosX;
		pos[1] = mainWindowPosY + mainWindowW;
		size[0] = subWindowW;
		size[1] = subWindowH;
		createWindow(id, name, pos, size, paintExternalForceY, reshapeExternalForceY);
		vecWinID.push_back(id);
		winDisp[id] = true;
	}


	//------------------------------------------
	// Sub Window 3
	//------------------------------------------
	if (winIsSet(winobj, EXT_Z)) {
		int id;
		strcpy(name, "Displacement in SBA - 3d view");
		pos[0] = mainWindowPosX + mainWindowW;
		pos[1] = mainWindowPosY + mainWindowW;
		size[0] = subWindowW;
		size[1] = subWindowH;
		createWindow(id, name, pos, size, paintExternalForceZ, reshapeExternalForceZ);
		vecWinID.push_back(id);
		winDisp[id] = true;
	}

	//------------------------------------------
	// idle
	//------------------------------------------
    glutIdleFunc(idleGL);

	initVx();

	logger = CVX_Logger(&(voxRobot->Vx));

	if (creatdir) {
		//------------------------------------------
		// 動画保存設定
		//------------------------------------------
		char dir[100];
		strcpy(dir, dirname);
		//strcat(dir, "/sim0");
		setSaveMovie(dir);

		if (saveave) (&logger)->setOutAverage(true);
		if (savedetail) (&logger)->setOutDetail(true);
		if (saveave || savedetail) {
			(&logger)->setDatObj(saveitems);
			(&logger)->setDirectory(dir);
			(&logger)->iniLogging();
		}
	}

    glutMainLoop();

}


VoxGraphics::~VoxGraphics()
{
    //cout << "Called destructor :: VoxGraphics" << endl;
}

void VoxGraphics::initGL()
{
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);

    gCamForward.Normalize();
    gCamRight.Normalize();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    static GLfloat lightPos[4] = {10.0f, 10.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0 , GL_POSITION , lightPos);


	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}


void VoxGraphics::initVx()
{
	voxRobot->initVoxRobot();
}

void VoxGraphics::createWindow(int &id, char *name, int pos[2], int size[2], void(*dispCallback)(void), void(*resizeCallback)(int,int)) {

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(pos[0], pos[1]);
	glutInitWindowSize(size[0], size[1]);
	id = glutCreateWindow(name);
	glutDisplayFunc(dispCallback);
	glutReshapeFunc(resizeCallback);
	glutMouseFunc(mouse);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	glutMouseWheelFunc(mouseWheel);
	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

	initGL();

}

void setSaveMovie(string dirname){

	//------------------------------------------
	// 動画保存設定
	//------------------------------------------
	char buf[100];
	//------------------------------------------
	// Main Window
	//------------------------------------------
	if (winIsSet(winobj, MAIN)) {
		strcpy(buf, dirname.c_str());
		strcat(buf, "/_main.avi");
		glCapture.setWriteFile(buf, CV_FOURCC('X', 'V', 'I', 'D'), 30.0f, cv::Size(mainWindowW, mainWindowH));
		//引数で細かく設定
		//glCapture.setWriteFile("output2.avi",CV_FOURCC('M','J','P','G'),60.0f,cv::Size(320,240));

		//拡大縮小時の補間方法の設定（デフォルトはバイリニア補間）
		glCapture.setInterpolation(cv::INTER_AREA);		//ピクセル領域の関係を利用したリサンプリングに変更
	}

	//------------------------------------------
	// Sub Window 1
	//------------------------------------------
	if (winIsSet(winobj, EXT_X)) {
		strcpy(buf, dirname.c_str());
		strcat(buf, "/_ca.avi");
		glCapture1.setWriteFile(buf, CV_FOURCC('X', 'V', 'I', 'D'), 30.0f, cv::Size(mainWindowW, mainWindowH));
		glCapture1.setInterpolation(cv::INTER_AREA);
	}

	//------------------------------------------
	// Sub Window 2
	//------------------------------------------
	if (winIsSet(winobj, EXT_Y)) {
		strcpy(buf, dirname.c_str());
		strcat(buf, "/_top.avi");
		glCapture2.setWriteFile(buf, CV_FOURCC('X', 'V', 'I', 'D'), 30.0f, cv::Size(mainWindowW, mainWindowH));
		glCapture2.setInterpolation(cv::INTER_AREA);
	}

	//------------------------------------------
	// Sub Window 3
	//------------------------------------------
	if (winIsSet(winobj, EXT_Z)) {
		strcpy(buf, dirname.c_str());
		strcat(buf, "/_3d.avi");
		glCapture3.setWriteFile(buf, CV_FOURCC('X', 'V', 'I', 'D'), 30.0f, cv::Size(mainWindowW, mainWindowH));
		glCapture3.setInterpolation(cv::INTER_AREA);
	}
}

void idleGL() {

	if (!spaceKey && !pause) {
		voxUpdatePhysX();
	}


	//

	voxUpdateDisp();

	if (!spaceKey && !pause) {
		gTimeStep++;
	}
	
}

Vec3D<double> target1 = Vec3D<float>(0.4 / sqrt(3.0), 0.4 / sqrt(3.0), 0.4 / sqrt(3.0));
Vec3D<double> target2 = Vec3D<float>(-0.4 / sqrt(3.0), 0.4 / sqrt(3.0), 0.4 / sqrt(3.0));
Vec3D<double> target3 = Vec3D<float>(0.4 / sqrt(3.0), -0.4 / sqrt(3.0), 0.4 / sqrt(3.0));
Vec3D<double> target4 = Vec3D<float>(0.4 / sqrt(3.0), 0.4 / sqrt(3.0), -0.4 / sqrt(3.0));
Vec3D<double> target5 = Vec3D<float>(-0.4 / sqrt(3.0), -0.4 / sqrt(3.0), 0.4 / sqrt(3.0));
Vec3D<double> target6 = Vec3D<float>(0.4 / sqrt(3.0), -0.4 / sqrt(3.0), -0.4 / sqrt(3.0));
Vec3D<double> target7 = Vec3D<float>(-0.4 / sqrt(3.0), 0.4 / sqrt(3.0), -0.4 / sqrt(3.0));
Vec3D<double> target8 = Vec3D<float>(-0.4 / sqrt(3.0), -0.4 / sqrt(3.0), -0.4 / sqrt(3.0));
Vec3D<double> target9 = Vec3D<float>(0, 0, 0);
bool sensing = true;
void paintGL(){
	Vec3D<double> target = target1;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    setupCamera(true);

	/*
	glPushMatrix();
	GLfloat coef = 12.5 / (voxRobot->Vx.voxelSize() *  voxRobot->Vx.voxelCount());
	//if (coef < 0.4f) coef = 0.4f;
	glScalef(coef + mpos[1], coef + mpos[1], coef + mpos[1]);
	*/

	float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
	
	glPushMatrix();
        glTranslatef(-halfVoxSize, -halfVoxSize, -halfVoxSize);
        drawGrid(gGridSize);
    glPopMatrix();
	

	drawVoxels(7,false);
	/*
	if (pressureView) {
		drawVoxels(7);
	}
	else {
		drawVoxels(0);
	}
	*/

	//glPopMatrix();

	glPushMatrix();

		glTranslatef(-halfVoxSize, -halfVoxSize, -halfVoxSize+1e-4);
		glLineWidth(0.8);
		glBegin(GL_LINES);	

		// x axis
		glColor3f((GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);
		glVertex3d(-50.0, 0, 0);
		glVertex3d(50.0, 0, 0);

		// y axis
		glColor3f((GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);
		glVertex3d(0, -50.0, 0);
		glVertex3d(0.0, 50.0, 0);

		// z axis
		
		glColor3f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0);
		glVertex3d(0, 0, -50.0);
		glVertex3d(0, 0, 50.0);
		
		glEnd();
		
	glPopMatrix();

	glPushMatrix();
	glColor3d(1.0, 0.0, 0.0); //色の設定
	glTranslatef((GLfloat)target.x, (GLfloat)target.y, (GLfloat)target.z);
	glutSolidSphere(0.05, 20, 20);//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
	glPopMatrix();

	std::stringstream ss;
	ss << "Simulation Time[s]: " << scientific << voxRobot->Vx.currentSimTime();
	DrawString(ss.str(), mainWindowW, mainWindowH, 0, 10);

	glutSwapBuffers();

	if (winIsSet(ctlWinobj, MAIN)) {
	  	glCapture.write();
	}
}

void reshapeMain(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

}

void paintExternalForceX() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setupCameraZoom(false);


	
	float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
	/*
	glPushMatrix();
	glTranslatef(0, 0, -halfVoxSize);
	drawGrid(gGridSize);
	glPopMatrix();
	*/
	

	drawVoxels(6,true);
//	drawVoxels(7);

	glPushMatrix();

	glTranslatef(-halfVoxSize, -halfVoxSize, -halfVoxSize + 1e-4);
	glLineWidth(0.8);
	glBegin(GL_LINES);

	// x axis
	glColor3f((GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);
	glVertex3d(-50.0, 0, 0);
	glVertex3d(50.0, 0, 0);

	// y axis
	glColor3f((GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);
	glVertex3d(0, -50.0, 0);
	glVertex3d(0.0, 50.0, 0);

	// z axis
	/*
	glColor3f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 3.0);
	*/
	glEnd();

	glPopMatrix();
	std::stringstream ss;
	ss << "Simulation Time[s]: " << scientific << voxRobot->Vx.currentSimTime();
	DrawString(ss.str(), mainWindowW, mainWindowH, 0, 10);
	
	glutSwapBuffers();

	if (winIsSet(ctlWinobj, EXT_X)) {
		glCapture1.write();
	}

}

void reshapeExternalForceX(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

}

void paintExternalForceY() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setupCameraZoom(false);


	float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
	/*
	glPushMatrix();
	glTranslatef(0, 0, -halfVoxSize);
	drawGrid(gGridSize);
	glPopMatrix();
	*/

	drawVoxels(7,true);

	glPushMatrix();

	glTranslatef(-halfVoxSize, -halfVoxSize, -halfVoxSize + 1e-4);
	glLineWidth(0.8);
	glBegin(GL_LINES);

	// x axis
	glColor3f((GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);
	glVertex3d(-50.0, 0, 0);
	glVertex3d(50.0, 0, 0);

	// y axis
	glColor3f((GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);
	glVertex3d(0, -50.0, 0);
	glVertex3d(0.0, 50.0, 0);

	// z axis
	/*
	glColor3f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 3.0);
	*/
	glEnd();

	glPopMatrix();
	std::stringstream ss;
	ss << "Simulation Time[s]: " << scientific << voxRobot->Vx.currentSimTime();
	DrawString(ss.str(), mainWindowW, mainWindowH, 0, 10);
	glutSwapBuffers();

	if (winIsSet(ctlWinobj, EXT_Y)) {
		glCapture2.write();
	}

}

void reshapeExternalForceY(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

}

void paintExternalForceZ() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setupCameraZoom(true);
	/*
	glPushMatrix();
	GLfloat coef = 12.5 / (voxRobot->Vx.voxelSize() *  voxRobot->Vx.voxelCount());
	//if (coef < 0.4f) coef = 0.4f;
	glScalef(coef + mpos[1], coef + mpos[1], coef + mpos[1]);
	*/

	float halfVoxSize = voxRobot->Vx.voxelSize() / 2.0f;
	/*
	glPushMatrix();
	glTranslatef(0, 0, -halfVoxSize);
	drawGrid(gGridSize);
	glPopMatrix();
	*/
	drawVoxels(7,true);

	glPushMatrix();

	glTranslatef(-halfVoxSize, -halfVoxSize, -halfVoxSize + 1e-4);
	glLineWidth(0.8);
	glBegin(GL_LINES);

	// x axis
	glColor3f((GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);
	glVertex3d(-50.0, 0, 0);
	glVertex3d(50.0, 0, 0);

	// y axis
	glColor3f((GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);
	glVertex3d(0, -50.0, 0);
	glVertex3d(0.0, 50.0, 0);

	// z axis
	glColor3f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)1.0);
	glVertex3d(0, 0, -50.0);
	glVertex3d(0, 0, 50.0);
	glEnd();

	glPopMatrix();

	std::stringstream ss;
	ss << "Simulation Time[s]: " << scientific << voxRobot->Vx.currentSimTime();
	DrawString(ss.str(), mainWindowW, mainWindowH, 0, 10);
	glutSwapBuffers();

	if (winIsSet(ctlWinobj, EXT_Z)) {
		glCapture3.write();
	}

}

void reshapeExternalForceZ(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

}

void voxUpdatePhysX()
{
	if(voxRobot->Vx.currentSimTime() == 0.0f){
		(&logger)->doLogging();
	}

    for(int i=0; i<gSkipStep; i++)
    {
       voxRobot->recallRobot(i);
    }

		(&logger)->doLogging();
	
}

void voxUpdateDisp() {

	for (int loop = 0; loop < vecWinID.size(); loop++) {
		if (winDisp.find(vecWinID[loop]) != winDisp.end() && winDisp.at(vecWinID[loop])) {
			glutSetWindow(vecWinID[loop]);
			glutPostRedisplay(); //再描画 
		}
	}

}

void setupCamera(bool _3D) {

	CVoxelyze* Vx = &voxRobot->Vx;
	
	Vec3D <float> cCamPos;
	Vec3D<double> centroid = Vec3D<double>(0, 0, 0);
//	if (!fixview || !setviewpoint) {
		for (auto n : *(Vx->voxelList())) {
			centroid += n->position();
		}
		centroid /= (double)Vx->voxelCount();

//	}else{
	//	centroid = fixViewPoint;
	//}

//	if (!setviewpoint) {
	//	fixViewPoint = centroid;
		//setviewpoint = true;
//	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (gMode = 0) glOrtho(-0.02, +0.02, -0.02, +0.02, -0.01, 0.1);
	else        gluPerspective(gCamAng, 1.0, 0.001f, 10.0f);
	glRotated(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//cout << centroid.x << " " << centroid.y << endl;

	if (_3D)
	{
		fixViewPoint = centroid;
	   cCamPos = Vec3D<float>(-0.3, -0.3, 0.075) + centroid;
	}
	else {
		if (fabs(centroid.x) < 0.25 && fabs(centroid.y) < 0.25)
			cCamPos = Vec3D<float>(0, -1e-8, 0.25) + fixViewPoint;
		else if (fabs(centroid.x) < 0.50 && fabs(centroid.y) < 0.50)
			cCamPos = Vec3D<float>(0, -1e-8, 0.5) + fixViewPoint;
		else if (fabs(centroid.x) < 0.75 && fabs(centroid.y) < 0.75)
			cCamPos = Vec3D<float>(0, -1e-8, 0.75) + fixViewPoint;
		else
			cCamPos = Vec3D<float>(0, -1e-8, 1.0) + fixViewPoint;
	}

	//cCamPos = Vec3D<float>(0, -0.11, -1e-8) + centroid;

	if (keyup) {

		gluLookAt(gCamPos.x + centroid.x, gCamPos.y + centroid.y, gCamPos.z + centroid.z + 0.2f,
			centroid.x, centroid.y, centroid.z,
			0.0f, 0.0f, 1.0f);

	}
	else if (keydown) {

		gluLookAt(gCamPos.x + centroid.x, gCamPos.y + centroid.y, -0.005f,
			centroid.x, centroid.y, centroid.z,
			0.0f, 0.0f, 1.0f);

	}
	else if (keyleft) {

		gluLookAt(gCamPos.x + centroid.x + 0.15f, gCamPos.y + centroid.y - 0.2f, gCamPos.z + centroid.z,
			centroid.x, centroid.y, centroid.z,
			0.0f, 0.0f, 1.0f);

	}
	else if (keyright) {

		gluLookAt(gCamPos.x + centroid.x - 0.15f, gCamPos.y + centroid.y - 0.2f, gCamPos.z + centroid.z,
			centroid.x, centroid.y, centroid.z,
			0.0f, 0.0f, 1.0f);

	}
	else {

		gluLookAt(cCamPos.x, cCamPos.y, cCamPos.z,
			fixViewPoint.x, fixViewPoint.y, fixViewPoint.z,
			0.0f, 0.0f, 1.0f);

	}

}

void setupCameraZoom(bool _3D) {

	CVoxelyze* Vx = &voxRobot->Vx;

	Vec3D <float> cCamPos;
	Vec3D<double> centroid = Vec3D<double>(0, 0, 0);
	for (auto n : *(Vx->voxelList())) {
		centroid += n->position();
	}
	centroid /= (double)Vx->voxelCount();

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (gMode = 0) glOrtho(-0.02, +0.02, -0.02, +0.02, -0.01, 0.1);
	else        gluPerspective(gCamAng, 1.0, 0.001f, 10.0f);
	glRotated(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(!_3D)cCamPos = Vec3D<float>(0, -1e-4, 0.2) + centroid;
	else   cCamPos = Vec3D<float>(-0.15, -0.15, 0.05) + centroid;
	gluLookAt(cCamPos.x, cCamPos.y, cCamPos.z,
			centroid.x, centroid.y, centroid.z,
			0.0f, 0.0f, 1.0f);
}

void drawGrid(int gridSize)
{
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_POLYGON_SMOOTH);
    int numGrid  = 0;
    //float gGridScale = voxRobot->Vx.voxelSize() * 10.0;
	float gGridScale = 0.01 * 10.0;
    for(int i=-gridSize; i<=gridSize; i++){
        for(int j=-gridSize; j<=gridSize; j++){
            if(numGrid%2 == 0)	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
            if(numGrid%2 == 1)	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

            float x0 = gGridScale *  i;
            float y0 = gGridScale *  j;
            float x1 = gGridScale * (i + 1);
            float y1 = gGridScale * (j + 1);
            glBegin(GL_LINE_LOOP);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(x0, y0, 0.0f);
            glVertex3f(x1, y0, 0.0f);
            glVertex3f(x1, y1, 0.0f);
            glVertex3f(x0, y1, 0.0f);
            glEnd();
            numGrid ++;
        }
    }

    glEnable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void drawVoxels(int axis, bool line)
{

	for (int l = 0; l < (&voxRobot->Vx)->maxLayer(); l++) {
	//for (int l = 0; l < 1; l++) {
		
		if (dispLayer != "") {
			std::string sLayer = "," + std::to_string(l) + ",";
			if (dispLayer.find(sLayer) == std::string::npos) continue;
		}

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_POLYGON_SMOOTH);

		CVX_MeshRender::viewColoring colorScheme;
		CVoxelyze::stateInfoType stateType;

		/*  
		EX) stateType = CVoxelyze::PRESSURE;

		DISPLACEMENT, //!< Displacement from a nominal position in meters
		VELOCITY, //!< Velocity in meters per second
		KINETIC_ENERGY, //!< Kinetic energy in joules
		ANGULAR_DISPLACEMENT, //!< Angular displacement from nominal orientation in radians
		ANGULAR_VELOCITY, //!< Angular velocity in radians per second
		ENG_STRESS, //!< Engineering stress in pascals
		ENG_STRAIN, //!< Engineering strain (unitless)
		STRAIN_ENERGY, //!< Strain energy in joules
		PRESSURE, //!< pressure in pascals
		MASS //!< mass in Kg
		*/
		if (axis == 7) {
			colorScheme = CVX_MeshRender::STATE_INFO;
			//stateType = CVoxelyze::DISPLACEMENT; // ここを変える
			stateType = CVoxelyze::KINETIC_ENERGY;
		}
		else {
			colorScheme = CVX_MeshRender::MATERIAL;
			stateType = CVoxelyze::DISPLACEMENT;
		}

		CVX_MeshRender voxMeshRender(&voxRobot->Vx, l, colorScheme, stateType, colorScale);
	
		// quads
		std::vector<int> quads;
		quads.clear();
		(&voxMeshRender)->getQuads(quads);
		int qCount = quads.size() / 4;


		std::vector<int> quadVoxIndices;
		quadVoxIndices.clear();
		(&voxMeshRender)->getQuadVoxIndices(quadVoxIndices);

		// quadColors
		std::vector<float> quadColors;
		quadColors.clear();
		(&voxMeshRender)->getQuadColors(quadColors);

		std::vector<float> quadExtColors;
		std::vector<float> quadInnColors;
		switch (axis) {
		case 1:
		case 2:
		case 3: 
			quadInnColors.clear();
			setColorInnForce(&voxRobot->Vx, qCount, quadVoxIndices, quadInnColors, axis);
			break;
		case 4:
		case 5:
		case 6:
			quadExtColors.clear();
			setColorExtForce(&voxRobot->Vx, qCount, quadVoxIndices, quadExtColors, axis);
			break;
		default:
			break;
		}

		// quadNormals
		std::vector<float> quadNormals;
		quadNormals.clear();
		(&voxMeshRender)->getQuadNormals(quadNormals);

		// vertices
		std::vector<float> vertices;
		vertices.clear();
		(&voxMeshRender)->getVertices(vertices);

		// lines
		std::vector<int> lines;
		lines.clear();
		(&voxMeshRender)->getLines(lines);
		int lCount = lines.size() / 2;

		if (line) {
			glLineWidth(0.5);
			glColor3f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0);
			glBegin(GL_LINES);


			for (int i = 0; i < lCount; i++) {
				glVertex3d(vertices[3 * lines[2 * i]], vertices[3 * lines[2 * i] + 1], vertices[3 * lines[2 * i] + 2]);
				glVertex3d(vertices[3 * lines[2 * i + 1]], vertices[3 * lines[2 * i + 1] + 1], vertices[3 * lines[2 * i + 1] + 2]);
			}

			glEnd();
		}

		glColor3f((GLfloat)0.0, (GLfloat)0.0, (GLfloat)0.0);

		std::vector<float> dispColors;
		dispColors.clear();
		if (axis != 0 && axis <= 3) {
			copy(quadInnColors.begin(), quadInnColors.end(), back_inserter(dispColors));
		}
		else if (axis == 0 || axis == 7) {
			copy(quadColors.begin(), quadColors.end(), back_inserter(dispColors));
		}
		else {
			copy(quadExtColors.begin(), quadExtColors.end(), back_inserter(dispColors));
		}

		for (int i = 0; i < qCount; i++) {
			glNormal3d(quadNormals[i * 3], quadNormals[i * 3 + 1], quadNormals[i * 3 + 2]);
			glColor3d(dispColors[i * 3], dispColors[i * 3 + 1], dispColors[i * 3 + 2]);
			//glLoadName(quadVoxIndices[i]); //to enable picking

			glBegin(GL_TRIANGLES);
			glVertex3d(vertices[3 * quads[4 * i]], vertices[3 * quads[4 * i] + 1], vertices[3 * quads[4 * i] + 2]);
			glVertex3d(vertices[3 * quads[4 * i + 1]], vertices[3 * quads[4 * i + 1] + 1], vertices[3 * quads[4 * i + 1] + 2]);
			glVertex3d(vertices[3 * quads[4 * i + 2]], vertices[3 * quads[4 * i + 2] + 1], vertices[3 * quads[4 * i + 2] + 2]);

			glVertex3d(vertices[3 * quads[4 * i + 2]], vertices[3 * quads[4 * i + 2] + 1], vertices[3 * quads[4 * i + 2] + 2]);
			glVertex3d(vertices[3 * quads[4 * i + 3]], vertices[3 * quads[4 * i + 3] + 1], vertices[3 * quads[4 * i + 3] + 2]);
			glVertex3d(vertices[3 * quads[4 * i]], vertices[3 * quads[4 * i] + 1], vertices[3 * quads[4 * i] + 2]);
			glEnd();
		}

		glEnable(GL_POLYGON_SMOOTH);
		glDisable(GL_BLEND);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);

	}

}

/*!
* シミュレーションリセット
*/
void resetSimulation() {

	reset = true;
	pause = true;
	voxRobot->resetSimTime();

	// 再描画
	voxUpdateDisp();

}

/*!
* シミュレーション開始
*/
void startSimulation() {

	// 録画再開
	ctlWinobj = winobj;
	start = true;
	pause = false;
}

/*!
* シミュレーション一時停止
*/
void pauseSimulation() {

	pause = true;

}

/*!
* Voxel変更
*/
void alterVoxels() {

	// 録画停止
	ctlWinobj = 0;

	// シミュレート一時停止
	pauseSimulation();


	alterCount++;

	// ディレクトリ作成
	char dir[100];
	if (creatdir) {

		strcpy(dir, dirname);
		strcat(dir, "/sim");
		strcat(dir, to_string(alterCount).c_str());
		MakeDirectory(dir);
	}

	// Voxel変更
	voxRobot->alterVoxels(alterCount);

	// 再描画
	voxUpdateDisp();

	if (creatdir) {
		// 動画保存先変更
		setSaveMovie(dir);

		// 解析データ保存先変更
		if (saveave || savedetail) {
			(&logger)->setDirectory(dir);
			(&logger)->iniLogging();
		}
	}
}

/*
void VoxGraphics::saveImage(){

    unsigned char* buffer =  (unsigned char*)calloc(400*400*4, sizeof(unsigned char));
    glReadBuffer((GL_FRONT));
    glReadPixels(0,0,400,400,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
    std::ostringstream oss;
    oss << "../image/im" << timeStep << ".rgba";
    std::ofstream ofs(oss.str());
    ofs.write(reinterpret_cast<const char*>(buffer), 400*400*4);
    ofs.close();

    std::ostringstream output;
    output << "../image/im" << timeStep << ".png";
    std::stringstream command;
    command << "convert -size " << "400x400" << " -depth 8 " << oss.str() << " " << output.str();
    system(command.str().c_str());
    std::remove(oss.str().c_str());
}
*/


void mouse(int button, int state, int x, int y)
{
	//cout << "mouse event" << endl;
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			//mpos[0] = x;
			//mpos[1] = y;
			//mouse_l = 1;
		}
		if (state == GLUT_UP) {
			mouse_l = 0;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			//mouse_l = 1;
			mpos[0] = 0;
			mpos[1] = 0;
		}
		if (state == GLUT_UP) {
			mouse_l = 0;
		}
		break;
	default:
		break;
	}
}

//void motion(int x, int y)
//{
//	cout << "motion event" << endl;
//
//	if (mouse_l == 1) {
//		theta[0] = (double)(y - mpos[1]) / 5.0;
//		theta[1] = (double)(x - mpos[0]) / 5.0;
//	}
//	if (mouse_l == 1 || mouse_m == 1 || mouse_r == 1) {
//		mpos[0] = x;  // ボタンを押しているときの マウスのx座標
//		mpos[1] = y;  // ボタンを押しているときの マウスのy座標
//		angle[0] += theta[0];
//		angle[1] += theta[1];
//		//glutPostRedisplay();
//	}
//}


void mouseWheel(int WheelNumber, int Direction, int x, int y)
{
	//cout << "mouseWheel event" << endl;
	mpos[0] = mpos[0] + Direction*0.01;
	mpos[1] = mpos[1] + Direction*0.01;
}


void key(unsigned char key, int x, int y) {
	int id;

	switch (key) {
		case 0x20:
			spaceKey = true;
			break;
		case 0x50: //P
		case 0x70: //p
			pause = !pause;
			break;
		case 0x52: //R
		case 0x72: //r
			resetSimulation();
			break;
		case 0x53: //S
		case 0x73: //s
			startSimulation();
			break;
		case 0x41: //A
		case 0x61: //a
			//alterVoxels();
			break;
		case 0x1B: //ESC
			id = glutGetWindow();
			if (id == vecWinID[0]) {
				exit(0);
			}
			winDisp[id] = false;
			glutDestroyWindow(id);
			break;
		default:
			break;
	}

	
}

void special(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_UP:
			keyup = true;
			break;
		case GLUT_KEY_RIGHT:
			keyright = true;
			break;
		case GLUT_KEY_DOWN:
			keydown = true;
			break;
		case GLUT_KEY_LEFT:
			keyleft = true;
			break;
		default:
			break;
	}


}


void keyUp(unsigned char key, int x, int y) {

	spaceKey = false;

}

void specialUp(int key, int x, int y) {

	keyup = false;
	keydown = false;
	keyright = false;
	keyleft = false;

}

void  setWinObj(std::string onoff) {

	winobj = 0;
	dispWindows edat = dispWindows::MAIN;

	if (onoff.size() != 7) return;

	for (char c : onoff) {
		winSet(winobj, edat, (c == '1') ? true : false);
		++edat;
	}

	ctlWinobj = winobj;

}

void setgSkipStep(float timestep) {

	if (strUserSkipStep == "") {
		gSkipStep = 1/(30*timestep);
	}
	else {
		gSkipStep = stoi(strUserSkipStep);
	}

	cout << "Simulation SkipStep:";
	cout << gSkipStep << endl;

}

void setColorExtForce(CVoxelyze* voxelyzeInstance, int quadCount, std::vector<int>  &quadVoxIndices, std::vector<float>  &array, int coloraxis)
{

	array.resize(quadCount * 3);

	for (int i = 0; i < quadCount; i++) {
		CVX_Voxel* pV = voxelyzeInstance->voxel(quadVoxIndices[i]);
		pV->external();
		Vec3D<float> extF = pV->externalForce();
		//os << "extF.x = " << extF.x << ", y= " << extF.y << ", z= " << extF.z << endl;

		switch (coloraxis) {
		case 4: //x軸

			if (extF.x > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(extF.x, extColorScale);
				array[i * 3 + 2] = getColorScale(extF.x, extColorScale);
			}
			else if (extF.x == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(extF.x, extColorScale);
				array[i * 3 + 1] = getColorScale(extF.x, extColorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;

		case 5: //y軸
			if (extF.y > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(extF.y, extColorScale);
				array[i * 3 + 2] = getColorScale(extF.y, extColorScale);
			}
			else if (extF.y == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(extF.y, extColorScale);
				array[i * 3 + 1] = getColorScale(extF.y, extColorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;
		default:
			if (extF.z > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(extF.z, extColorScale);
				array[i * 3 + 2] = getColorScale(extF.z, extColorScale);
			}
			else if (extF.z == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(extF.z, extColorScale);
				array[i * 3 + 1] = getColorScale(extF.z, extColorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;
		}
	}
}

void setColorInnForce(CVoxelyze* voxelyzeInstance, int quadCount, std::vector<int>  &quadVoxIndices, std::vector<float>  &array, int coloraxis)
{
	array.resize(quadCount * 3);

	for (int i = 0; i < quadCount; i++) {
		CVX_Voxel* pV = voxelyzeInstance->voxel(quadVoxIndices[i]);
		Vec3D<float> innF = pV->force();
		//os << "innF.x = " << innF.x << ", y= " << innF.y << ", z= " << innF.z << endl;

		switch (coloraxis) {
		case 1: //x軸

			if (innF.x > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(innF.x, curcolorScale);
				array[i * 3 + 2] = getColorScale(innF.x, curcolorScale);
			}
			else if (innF.x == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(innF.x, curcolorScale);
				array[i * 3 + 1] = getColorScale(innF.x, curcolorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;

		case 2: //y軸
			if (innF.y > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(innF.y, curcolorScale);
				array[i * 3 + 2] = getColorScale(innF.y, curcolorScale);
			}
			else if (innF.y == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(innF.y, curcolorScale);
				array[i * 3 + 1] = getColorScale(innF.y, curcolorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;
		default:
			if (innF.z > 0) {
				array[i * 3] = 1.0f;
				array[i * 3 + 1] = getColorScale(innF.z, curcolorScale);
				array[i * 3 + 2] = getColorScale(innF.z, curcolorScale);
			}
			else if (innF.z == 0) {

				array[i * 3] = 1.0f;
				array[i * 3 + 1] = 1.0f;
				array[i * 3 + 2] = 1.0f;
			}
			else {
				array[i * 3] = getColorScale(innF.z, curcolorScale);
				array[i * 3 + 1] = getColorScale(innF.z, curcolorScale);
				array[i * 3 + 2] = 1.0f;

			}
			break;
		}
	}

}

float getColorScale(float value, float scale) {

	float absValue = (float)abs((double)value);

	if (absValue >= scale) return 0;

	float rate;

	rate = 1 - absValue / scale;

	return rate;

}


/*!
* 文字列描画
* @param[in] str 文字列
* @param[in] w,h ウィンドウサイズ
* @param[in] x0,y0 文字列の位置(左上原点のスクリーン座標系,文字列の左下がこの位置になる)
*/
void DrawString(string str, int w, int h, int x0, int y0)
{
	glColor3d(1.0, 1.0, 1.0);

	glDisable(GL_LIGHTING);
	// 平行投影にする
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// 画面上にテキスト描画
	glRasterPos2f(x0, y0);
	int size = (int)str.size();
	for (int i = 0; i < size; ++i) {
		char ic = str[i];
		//glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ic);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}