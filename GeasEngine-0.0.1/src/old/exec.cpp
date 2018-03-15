#include "exec.h"

using namespace std;
char dirname[100];
ofstream os;

char inputJson[100];
string movie;
bool savelog;
bool savejson;
bool saveave;
bool savedetail;
bool creatdir = false;
string saveitems;
string strUserTimeStep;
string strUserSkipStep;

int main(int argc, char *argv[])
{

	time_t t;
	struct tm *timeptr, result;

	char cinbuff[100];

	//---------------------------------
	// User Input
	//---------------------------------
	// Json file path
	cout << "Input Json File:";
	cin.getline(inputJson, sizeof(inputJson));
	myTrim(inputJson);

	// save main window
	cout << "Set Save Main Window Y/N:" << endl;
	cin.getline(cinbuff, sizeof(cinbuff));
	myTrim(cinbuff);
	string saveMainWindow = "0";
	if (strcmp(cinbuff, "y") == 0 || strcmp(cinbuff, "Y") == 0) {
		saveMainWindow = "1";
	}

	// save sub window
	cout << "Set Save Sub Windows. e.g.100000(6bit)" << endl;
	cout << "External X" << endl;
	cout << "External Y" << endl;
	cout << "External Z" << endl;
	cout << "Current X" << endl;
	cout << "Current Y" << endl; 
	cout << "Current Z" << endl;
	cout << ":";
	cin.getline(cinbuff, sizeof(cinbuff));
	myTrim(cinbuff);
	if (strlen(cinbuff) == 6) {
		try {
			double i = stod(cinbuff);
			movie = cinbuff;
			movie = saveMainWindow + movie;
			if(movie != "0000000"){
				creatdir = true;
			}
		}
		catch (const invalid_argument& e) {
			//動画保存なし
		}
	}

	// save log
	savelog = false;
	cout << "Save Log Y/N:";
	cin.getline(cinbuff, sizeof(cinbuff));
	myTrim(cinbuff);
	if (strcmp(cinbuff,"y") == 0 || strcmp(cinbuff, "Y") == 0) {
		savelog = true;
		creatdir = true;

	}

	// analytical data
	saveave = false;
	cout << "Save Analytical Average Data Y/N:";
	cin.getline(cinbuff, sizeof(cinbuff));
	 myTrim(cinbuff);
	 if (strcmp(cinbuff, "y") == 0 || strcmp(cinbuff, "Y") == 0) {
		saveave = true;
		creatdir = true;
	}
	savedetail = false;
	cout << "Save Analytical Detail Data Y/N:";
	cin.getline(cinbuff, sizeof(cinbuff));
	myTrim(cinbuff);
	if (strcmp(cinbuff, "y") == 0 || strcmp(cinbuff, "Y") == 0) {
		savedetail = true;
		creatdir = true;
	}

	saveitems = "00000000000000000";
	cout << "If you input Save Detail Data is ""Y"", set Save Items. e.g.10000000000000000(17bit)" << endl;
	cout << "POSITION" << endl;
	cout << "CORNERPOSITION" << endl;
	cout << "CORNEROFFSET" << endl;
	cout << "VOXELSIZE" << endl;
	cout << "BASESIZE" << endl;
	cout << "DISPLACEMENT" << endl;
	cout << "ORIENTATION" << endl;
	cout << "FORCE" << endl;
	cout << "MOMENT" << endl;
	cout << "VELOCITY" << endl;
	cout << "ANGULARVELOCITY" << endl;
	cout << "PRESSURE" << endl;
	cout << "KINETICENERGY" << endl;
	cout << "VOLUMETRICSTRAIN" << endl;
	cout << "TRANSVERSEAREA" << endl;
	cout << "TRANSVERSESTRAINSUM" << endl;
	cout << "DAMPINGMULTIPLIER" << endl;
	cout << ":";
	cin.getline(cinbuff, sizeof(cinbuff));
	myTrim(cinbuff);
	if (strlen(cinbuff) == 17) {
		try {
			double i = stod(cinbuff);
			saveitems = cinbuff;
			if (i == 0) {
				saveave = false;
				savedetail = false;
			}
		}
		catch (const invalid_argument& e) {
			//解析データ出力なし
		}
	}

	// TimeStep
	cout << "---------------------------------" << endl;
	cout << "Set TimeStep:";
	cin.getline(cinbuff, sizeof(cinbuff));
	myTrim(cinbuff);
	if (strlen(cinbuff) != 0) {
		try {
			double i = stod(cinbuff);
			if (i == 0) {
				strUserTimeStep = "";
			}
			else {
				strUserTimeStep = cinbuff;
			}			
		}
		catch (const invalid_argument& e) {
			strUserTimeStep = "";
		}
	}

	// SkipStep
	cout << "Set SkipStep:";
	cin.getline(cinbuff, sizeof(cinbuff));
	myTrim(cinbuff);
	if (strlen(cinbuff) != 0) {
		try {
			double i = stod(cinbuff);
			if (i == 0) {
				strUserSkipStep = "";
			}
			else {
				strUserSkipStep = cinbuff;
			}
		}
		catch (const invalid_argument& e) {
			strUserSkipStep = "";
		}
	}

	// save json
	savejson = false;
	cout << "Save Json Y/N:";
	cin.getline(cinbuff, sizeof(cinbuff));
	myTrim(cinbuff);
	if (strcmp(cinbuff, "y") == 0 || strcmp(cinbuff, "Y") == 0) {
		savejson = true;
		creatdir = true;

	}

	setlocale(LC_ALL, "/QSYS.LIB/EN_US.LOCALE");
	t = time(NULL);
	timeptr = localtime(&t);
	strftime(dirname, sizeof(dirname), "%Y%m%d%H%M", timeptr);
	

	// ディレクトリ作成
	if (creatdir) {
		char dir[100];
		strcpy(dir, dirname);
		strcat(dir, "/sim0");
		MakeDirectory(dir);
	}

	char buf[100];
	strcpy(buf, dirname);
	strcat(buf, "/user.log");

	if(savelog){
		os = ofstream(buf, ios::app);

		os << "Input Json File:" << inputJson << endl;
		os << "Set Save Movies:" << movie << endl;
		os << "Save Log:" << savelog << endl;
		os << "Save Analytical Average Data:" << saveave << endl;
		os << "Save Analytical Detail Data:" << savedetail << endl;
		os << "Set Save Items:" << saveitems << endl;
		os << "Set TimeStep:" << strUserTimeStep << endl;
		os << "Set SkipStep:" << strUserSkipStep << endl;
	}


	VoxGraphics *voxgrahics = new VoxGraphics();
	voxgrahics->initVoxGraphics(argc, argv);

	os.close();


	return 0;
}

