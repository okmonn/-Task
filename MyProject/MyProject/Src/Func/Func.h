#pragma once
#include <string>

namespace func {
	// �r�b�g�ϊ�
	int Bit(const int& i);

	// �o�C�g�ϊ�
	int Byte(const int& i);

	// �ΐ�
	int Log(const int& i, const int& n);

	// �ݏ�
	int Pow(const int& i, const int& n);

	// ������̊m�F
	bool CheckChar(const std::string& find, unsigned char * data, const unsigned int& dataNum = 4);

	// ������̑��
	void AddChar(const std::string& add, unsigned char* data, const unsigned int& dataNum = 4);

	// ������̌���
	std::string FindString(const std::string& path, const char& find, const unsigned int& offset = 1, const bool& end = true);

	// �t�H���_���ƃt�@�C�����̘A��
	std::string FileLink(const std::string& path1, const std::string& path2);

	// ���C�h������̕ϊ�
	std::wstring ChangeWString(const std::string& st);
}
