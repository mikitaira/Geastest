#include "Utility.h"

using namespace std;

bool MakeDirectory(string lpPathName)
{
	int nStart = 0;
	bool res = false;
	string  strMakePath;
	string  strPathName = lpPathName;

	// ディレクトリを1つずつ繰り返し作成
	while (strMakePath != lpPathName)
	{
		// 作成するディレクトリ名を設定
		nStart = strPathName.find('/', nStart + 1);
		strMakePath = strPathName.substr(0, nStart);

		// ディレクトリが存在するかチェックし無ければ作成
		struct stat st;
		if (!(stat( strMakePath.c_str(), &st ) == 0 ||
			(strMakePath.length() == 2 && strMakePath.at(1) == ':')))
			res = _mkdir(strMakePath.c_str());
	}
	return res;
}

/*
* 文字列の先頭と末尾にある空白を削除する
* @param[in] s 対象文字列
* @return 削除した空白の数を返す
*/
void myTrim(char* str) {

	int i;
	int count = 0;
	string buf;

	/* 空ポインタか? */
	if (str == NULL) { /* yes */
		return;
	}

	buf = str;
	/* 末尾から順に空白でない位置を探す */
	i = buf.size();
	while (--i >= 0 && buf[i] == ' ') count++;

	/* 終端ナル文字を付加する */
	buf.replace(i + 1, count, "");

	/* 先頭から順に空白でない位置を探す */
	i = 0;
	while (buf[i] != '\0' && buf[i] == ' ') i++;
	buf.replace(0, i, "");

	strcpy(str, buf.c_str());

	return;
}
