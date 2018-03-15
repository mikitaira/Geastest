#include "GeasViewer.h"
#include "AgentSimulation.h"
int mouse_l = 0;
int mouse_m = 0;
int mouse_r = 0;

float mpos[2];  // mpos[0]=x���W, mpos[1]=y���W

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
bool wireframe = true;

int   gTimeStep;
int	  simTimeStep;
int   gSkipStep;
float gCamAng;                  // Camera angle
float gCamSpeed;                // Camera speed
Vec3D <float> gCamPos;          // Camera position
Vec3D <float> gCamForward;      // Camera forward direction
Vec3D <float> gCamRight;		// Camera right direction
int   gMode;				    // Orthographic: gMode = 0, Perspective: gMode = 1
int   gGridSize;
std::string movie;
winObj winobj, ctlWinobj;
// CVX_Logger logger;

// GLCapture glCapture;
// GLCapture glCapture1;
// GLCapture glCapture2;
// GLCapture glCapture3;

GLfloat lightPosition[4] = { 0.25f, 1.0f, 0.25f, 0.0f };
GLfloat lightDiffuse[3] = { 1.0f, 1.0f, 1.0f };
GLfloat lightAmbient[3] = { 0.25f, 0.25f, 0.25f };
GLfloat lightSpecular[3] = { 1.0f, 1.0f, 1.0f };

GLfloat diffuse[3] = { 1.0f, 1.0f, 0.0f };
GLfloat ambient[3] = { 0.25f, 0.25f, 0.25f };
GLfloat specular[3] = { 1.0f, 1.0f, 1.0f };
GLfloat shininess[1] = { 32.0f };

int GLframe = 0;
int GLtimenow = 0;
int GLtimebase = 0;


std::vector<int> vecWinID;
std::map<int, bool> winDisp;

extern char dirname[100];
int alterCount = 0;
extern std::ofstream os;

extern bool saveave;
extern bool savedetail;
extern bool creatdir;
std::string saveitems;
std::string strUserSkipStep;

std::string dispLayer;
std::string colorScale;
bool pressureView;
bool fixview = true;
bool setviewpoint = true;


Vec3D<double> fixViewPoint = Vec3D<double>(0, 0, 0);

float curcolorScale;
float extColorScale;

AgentSimulation As;

void initializeViewer(int argc, char *argv[])
{


	glutInit(&argc, argv);

	int pos[2];
	int size[2];
	char name[50];

	gTimeStep = 0;
	gCamAng = 90.0f;
	gCamSpeed = 1.0f;
	// to view all object
	//gCamPos = Vec3D<float>{ 0.0f, 0.35f, 0.75f };

//	gCamPos = Vec3D<float>{ -0.4f, 0.5f, 0.5f };
//	gCamPos = Vec3D<float>{ 0.6f, 0.6f, 0.6f };
	gCamPos = Vec3D<float>{ 0.0f, 0.35f, 0.5f };
	gCamForward = Vec3D<float>(0.0f, 0.0f, 0.0f) - gCamPos;
	gCamRight = Vec3D<float>(1.0f, 0.0f, 0.0f);
	gMode = 0;
	gGridSize = 100;

	setWinObj(movie);

	//------------------------------------------
	// Main Window
	//------------------------------------------
	int mainId;
	strcpy(name, "Geass Viewer");
	pos[0] = mainWindowPosX;
	pos[1] = mainWindowPosY;
	size[0] = mainWindowW;
	size[1] = mainWindowH;
	createWindow(mainId, name, pos, size, paintGL, reshapeMain);
	vecWinID.push_back(mainId);
	winDisp[mainId] = true;

	for (int loop = 0; loop < vecWinID.size(); loop++) {
		if (winDisp.find(vecWinID[loop]) != winDisp.end() && winDisp.at(vecWinID[loop])) {
			std::cout << "search window : " << loop << " "  << vecWinID[loop] << std::endl;
		}
	}
	//------------------------------------------
	// Sub Window 1
	//------------------------------------------
	/*
	if (winIsSet(winobj, SUB_1)) {
	int id;
	strcpy(name, "Geass Subviewer");
	pos[0] = mainWindowPosX + mainWindowW;
	pos[1] = mainWindowPosY;
	size[0] = subWindowW;
	size[1] = subWindowH;
	createWindow(id, name, pos, size, paintExternalForceX, reshapeExternalForceX);
	vecWinID.push_back(id);
	winDisp[id] = true;
	}
	*/

	//------------------------------------------
	// idle
	//------------------------------------------
	glutIdleFunc(idleGL);

	/*
	logger = CVX_Logger(&(voxRobot->Vx));
	if (creatdir) {
	char dir[100];
	strcpy(dir, dirname);
	setSaveMovie(dir);
	if (saveave) (&logger)->setOutAverage(true);
	if (savedetail) (&logger)->setOutDetail(true);
	if (saveave || savedetail) {
	(&logger)->setDatObj(saveitems);
	(&logger)->setDirectory(dir);
	(&logger)->iniLogging();
	}
	}
	*/

	As.configuration();

	glutMainLoop();

}

void setWinObj(std::string onoff)
{
	winobj = 0;
	dispWindows edat = dispWindows::MAIN;
	if (onoff.size() != 7) return;

	for (char c : onoff) {
		winSet(winobj, edat, (c == '1') ? true : false);
		++edat;
	}

	ctlWinobj = winobj;
}

void createWindow(int &id, char *name, int pos[2], int size[2], void(*dispCallback)(void), void(*resizeCallback)(int, int))
{
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

void initGL()
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
	static GLfloat lightPos[4] = { 10.0f, 10.0f, 10.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);


	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}


void setCurrentTime(int t)
{
	simTimeStep = t;
}

// ** ここまで実装済み ** //

/*
void setSaveMovie(std::string dirname) {

//------------------------------------------
// ����ۑ��ݒ�
//------------------------------------------
char buf[100];
//------------------------------------------
// Main Window
//------------------------------------------
if (winIsSet(winobj, MAIN)) {
strcpy(buf, dirname.c_str());
strcat(buf, "/_main.avi");
glCapture.setWriteFile(buf, CV_FOURCC('X', 'V', 'I', 'D'), 30.0f, cv::Size(mainWindowW, mainWindowH));
//�����ōׂ����ݒ�
//glCapture.setWriteFile("output2.avi",CV_FOURCC('M','J','P','G'),60.0f,cv::Size(320,240));

//�g��k�����̕�ԕ��@�̐ݒ�i�f�t�H���g�̓o�C���j�A��ԁj
glCapture.setInterpolation(cv::INTER_AREA);		//�s�N�Z���̈�̊֌W�𗘗p�������T���v�����O�ɕύX
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
*/

void idleGL() {
	gSkipStep = 1;
	if (!spaceKey && !pause) {
		for (int i = 0; i < gSkipStep; i++) {
			As.simulation();
		}
		setCurrentTime(As.timeStep);
	}


	for (int loop = 0; loop < vecWinID.size(); loop++) {
		if (winDisp.find(vecWinID[loop]) != winDisp.end() && winDisp.at(vecWinID[loop])) {
			glutSetWindow(vecWinID[loop]);
			glutPostRedisplay();
		}
	}

	if (!spaceKey && !pause) {
		if (simTimeStep != gTimeStep) {
			gTimeStep = simTimeStep;
		}
	}
}

void drawSphere(int index) {
	GLfloat m[16];
	btDefaultMotionState *motion = static_cast<btDefaultMotionState*>(As.Gd.rigidbody()->bodyList()->at(index)->getMotionState());
	btSphereShape *sphere = static_cast<btSphereShape*>(As.Gd.rigidbody()->bodyList()->at(index)->getCollisionShape());
	double radius = sphere->getRadius();
	motion->m_graphicsWorldTrans.getOpenGLMatrix(m);

	glPushMatrix();
	glMultMatrixf(m);
	glColor3d(1.0, 0.0, 0.0);
	if (wireframe)	glutWireSphere(radius, 32, 32);
	else			glutSolidSphere(radius, 32, 32);
	glPopMatrix();
}

void drawBox(int index) {
	GLfloat m[16];
	btDefaultMotionState *motion = static_cast<btDefaultMotionState*>(As.Gd.rigidbody()->bodyList()->at(index)->getMotionState());
	btBoxShape *box = static_cast<btBoxShape*>(As.Gd.rigidbody()->bodyList()->at(index)->getCollisionShape());
	Vec3D<double> halfExtent = Vec3D<double>(box->getHalfExtentsWithMargin().getX(),
											 box->getHalfExtentsWithMargin().getY(),
											 box->getHalfExtentsWithMargin().getZ());
	motion->m_graphicsWorldTrans.getOpenGLMatrix(m);

	glPushMatrix();
	glMultMatrixf(m);
	glColor3d(1.0, 0.5, 0.0);
	if (wireframe)	glutWireCube(2 * halfExtent.x);
	else			glutSolidCube(2 * halfExtent.x);

	glPopMatrix();
}
void drawCapsule(int index) {
	GLfloat m[16];
	btDefaultMotionState *motion = static_cast<btDefaultMotionState*>(As.Gd.rigidbody()->bodyList()->at(index)->getMotionState());
	btCapsuleShape *capsule = static_cast<btCapsuleShape*>(As.Gd.rigidbody()->bodyList()->at(index)->getCollisionShape());
	motion->m_graphicsWorldTrans.getOpenGLMatrix(m);
	double radius = capsule->getRadius();
	double length = capsule->getHalfHeight();
	glPushMatrix();
		glMultMatrixf(m);
		glColor3d(1.0, 0.5, 0.0);
		if(wireframe)	glutWireCylinder(radius, 2 * length, 32, 32);
		else			glutSolidCylinder(radius, 2 * length, 32, 32);
		glPushMatrix();
			glTranslated(0, 0, 2 * length);
			if (wireframe)	glutWireSphere(radius, 32, 32);
			else			glutSolidSphere(radius, 32, 32);
		glPopMatrix();
		glPushMatrix();
			glTranslated(0, 0, 0);
			if (wireframe)	glutWireSphere(radius, 32, 32);
			else			glutSolidSphere(radius, 32, 32);
		glPopMatrix();
	glPopMatrix();
}

void drawCylinder(int index) {
	GLfloat m[16];
	btDefaultMotionState *motion = static_cast<btDefaultMotionState*>(As.Gd.rigidbody()->bodyList()->at(index)->getMotionState());
	btCylinderShape *cylinder = static_cast<btCylinderShape*>(As.Gd.rigidbody()->bodyList()->at(index)->getCollisionShape());
	motion->m_graphicsWorldTrans.getOpenGLMatrix(m);

	double radius = cylinder->getRadius();
	Vec3D<double> halfExtent = Vec3D<double>(cylinder->getHalfExtentsWithMargin().getX(),
											 cylinder->getHalfExtentsWithMargin().getY(),
											 cylinder->getHalfExtentsWithMargin().getZ());
	glPushMatrix();
		glMultMatrixf(m);
		glColor3d(1.0, 0.5, 0.0);
		if (wireframe)	glutWireCylinder(radius, 2 *  halfExtent.y, 32, 32);
		else			glutSolidCylinder(radius, 2 * halfExtent.y, 32, 32);
	glPopMatrix();
}

void drawRigidBody(int index) {
	int shapeType = As.Gd.rigidbody()->getShape(index); //vGd.rigidbody()->bodyList()->at(i).shapeType;
	if (shapeType == GD_Rigidbody::SPHERE)	drawSphere(index);
	else if (shapeType == GD_Rigidbody::BOX)	drawBox(index);
	else if (shapeType == GD_Rigidbody::CAPSULE)	drawCapsule(index);
	else if (shapeType == GD_Rigidbody::CYLINDER)	drawCylinder(index);
}

void paintGL() {
	// glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setupCamera(true);

	//// ** Plane ** //
	float halfVoxSize = As.Gd.softbody(0)->Vx.voxelSize() / 2.0f;
	glPushMatrix();
	glTranslatef(0, 0, 0);
	drawGrid(gGridSize);
	glPopMatrix();

	// ** Rigid body ** //
	int rgNum = As.Gd.rigidbody()->bodyList()->size();
	for (int i = 0; i < rgNum; i++) {
		drawRigidBody(i);
	}

	

	// ** Soft body ** //
	for (int nb = 0; nb < As.Gd.softBodyList.size(); nb++){
		glPushMatrix();
		glTranslated(As.Gd.softbody(nb)->position.x, As.Gd.softbody(nb)->position.y, As.Gd.softbody(nb)->position.z);
		glRotated(180 / PI * As.Gd.softbody(nb)->quaternion.Angle(), As.Gd.softbody(nb)->quaternion.Axis().x, As.Gd.softbody(nb)->quaternion.Axis().y, As.Gd.softbody(nb)->quaternion.Axis().z);
		drawVoxels(nb, 0, false);
		glPopMatrix();
	}

	std::stringstream ss;
	ss << "Simulation Time: " << gTimeStep;
	DrawString(ss.str(), mainWindowW, mainWindowH, 0, 10);

	glutSwapBuffers();

	/*
	if (winIsSet(ctlWinobj, MAIN)) {
	glCapture.write();
	}
	*/
}

void reshapeMain(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

}

void setupCamera(bool _3D) {


	Vec3D <float> cCamPos;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (gMode = 1)	glOrtho(-0.5, +0.5, -0.5, +0.5, -0.01, 10.0);
	else if (gMode = 0)	glOrtho(-0.02, +0.02, -0.02, +0.02, -0.01, 0.1);
	else			gluPerspective(gCamAng, 1.0, 0.001f, 1000.0f);
	glRotated(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vec3D<float>centroid = Vec3D<float>(0, 0, 0);
	if (_3D)
	{
		fixViewPoint = centroid;
		//fixViewPoint = As.Gd.softbody(0)->voxelList()->at(0)->position();
		cCamPos = gCamPos + centroid;
	}
	//gluLookAt(cCamPos.x, cCamPos.y, cCamPos.z,
	//	fixViewPoint.x, fixViewPoint.y, fixViewPoint.z,
	//	0.0f, 1.0f, 0.0f);
	//gluLookAt(cCamPos.x, cCamPos.y, cCamPos.z,
	//	cCamPos.x, cCamPos.y, 0,
	//	0.0f, 1.0f, 0.0f);
	gluLookAt(cCamPos.x-0.1f, cCamPos.y + 0.2f, cCamPos.z,
		cCamPos.x, cCamPos.y, 0,
		0.0f, 1.0f, 0.0f);
}

void drawGrid(int gridSize)
{
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_POLYGON_SMOOTH);
	int numGrid = 0;
	//float gGridScale = voxRobot->Vx.voxelSize() * 10.0;
	float gGridScale = 0.1;
	for (int i = -gridSize; i <= gridSize; i++) {
		for (int j = -gridSize; j <= gridSize; j++) {
			if (numGrid % 2 == 0)	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
			if (numGrid % 2 == 1)	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			float x0 = gGridScale *  i;
			float z0 = gGridScale *  j;
			float x1 = gGridScale * (i + 1);
			float z1 = gGridScale * (j + 1);
			glBegin(GL_LINE_LOOP);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(x0, 0.0f, z0);
			glVertex3f(x1, 0.0f, z0);
			glVertex3f(x1, 0.0f, z1);
			glVertex3f(x0, 0.0f, z1);
			glEnd();
			numGrid++;
		}
	}

	glEnable(GL_POLYGON_SMOOTH);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void drawVoxels(int index, int axis, bool line)
{

	for (int l = 0; l < As.Gd.softbody(index)->Vx.maxLayer(); l++) {

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
			stateType = CVoxelyze::KINETIC_ENERGY;
		}
		else {
			colorScheme = CVX_MeshRender::MATERIAL;
			stateType = CVoxelyze::DISPLACEMENT;
		}

		CVX_MeshRender voxMeshRender(&As.Gd.softbody(index)->Vx, l, colorScheme, stateType, colorScale);

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

		//glPushMatrix();
		//glTranslated(0, As.Gd.softbody(index)->Vx.voxelSize() * 0.5 ,0);
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
		//glPopMatrix();

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

		//glPushMatrix();
		//glTranslated(0, As.Gd.softbody(index)->Vx.voxelSize() * 0.5, 0);
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
		//glPopMatrix();

		glEnable(GL_POLYGON_SMOOTH);
		glDisable(GL_BLEND);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);

	}

}

void resetSimulation()
{

	reset = true;
	pause = true;
	gTimeStep = 0;
}

void startSimulation()
{

	ctlWinobj = winobj;
	start = true;
	pause = false;
}

void pauseSimulation()
{

	pause = true;

}


/*
void alterVoxels() {

ctlWinobj = 0;

pauseSimulation();


alterCount++;

char dir[100];
if (creatdir) {

strcpy(dir, dirname);
strcat(dir, "/sim");
strcat(dir, to_string(alterCount).c_str());
MakeDirectory(dir);
}

voxRobot->alterVoxels(alterCount);

voxUpdateDisp();

if (creatdir) {
setSaveMovie(dir);

if (saveave || savedetail) {
(&logger)->setDirectory(dir);
(&logger)->iniLogging();
}
}
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

void setgSkipStep(float timestep) {

	if (strUserSkipStep == "") {
		gSkipStep = 1 / (30 * timestep);
	}
	else {
		gSkipStep = stoi(strUserSkipStep);
	}

	std::cout << "Simulation SkipStep:";
	std::cout << gSkipStep << std::endl;

}

float getColorScale(float value, float scale)
{

	float absValue = (float)abs((double)value);

	if (absValue >= scale) return 0;

	float rate;

	rate = 1 - absValue / scale;

	return rate;

}


void DrawString(std::string str, int w, int h, int x0, int y0)
{
	glColor3d(1.0, 1.0, 1.0);

	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	glRasterPos2f(x0, y0);
	int size = (int)str.size();
	for (int i = 0; i < size; ++i) {
		char ic = str[i];
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ic);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

