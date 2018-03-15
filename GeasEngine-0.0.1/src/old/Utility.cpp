#include "Utility.h"

using namespace std;

bool MakeDirectory(string lpPathName)
{
	int nStart = 0;
	bool res = false;
	string  strMakePath;
	string  strPathName = lpPathName;

	// �f�B���N�g����1���J��Ԃ��쐬
	while (strMakePath != lpPathName)
	{
		// �쐬����f�B���N�g������ݒ�
		nStart = strPathName.find('/', nStart + 1);
		strMakePath = strPathName.substr(0, nStart);

		// �f�B���N�g�������݂��邩�`�F�b�N��������΍쐬
		struct stat st;
		if (!(stat( strMakePath.c_str(), &st ) == 0 ||
			(strMakePath.length() == 2 && strMakePath.at(1) == ':')))
			res = _mkdir(strMakePath.c_str());
	}
	return res;
}

/*
* ������̐擪�Ɩ����ɂ���󔒂��폜����
* @param[in] s �Ώە�����
* @return �폜�����󔒂̐���Ԃ�
*/
void myTrim(char* str) {

	int i;
	int count = 0;
	string buf;

	/* ��|�C���^��? */
	if (str == NULL) { /* yes */
		return;
	}

	buf = str;
	/* �������珇�ɋ󔒂łȂ��ʒu��T�� */
	i = buf.size();
	while (--i >= 0 && buf[i] == ' ') count++;

	/* �I�[�i��������t������ */
	buf.replace(i + 1, count, "");

	/* �擪���珇�ɋ󔒂łȂ��ʒu��T�� */
	i = 0;
	while (buf[i] != '\0' && buf[i] == ' ') i++;
	buf.replace(0, i, "");

	strcpy(str, buf.c_str());

	return;
}
