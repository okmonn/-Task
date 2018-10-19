#include "Func.h"
#include <Windows.h>
#include <algorithm>
#include <tchar.h>

// �r�b�g�ϊ�
int func::Bit(const int & i)
{
	return i * 8;
}

// �o�C�g�ϊ�
int func::Byte(const int & i)
{
	return i / 8;
}

// �ΐ�
int func::Log(const int & i, const int & n)
{
	return static_cast<int>(std::log(n) / std::log(i));
}

// �ݏ�
int func::Pow(const int & i, const int & n)
{
	return (n == 0) ? 1 : i << (n - 1);
}

// ������̊m�F
bool func::CheckChar(const std::string & find, unsigned char * data, const unsigned int& dataNum)
{
	if (find.size() - 1 > dataNum)
	{
		OutputDebugString(_T("\n�������������񂪃f�[�^���𒴂��Ă��܂�\n"));
		return false;
	}

	unsigned int cnt = 0;
	for (unsigned int i = 0; i < dataNum; ++i)
	{
		if (data[i] == find[cnt])
		{
			if (cnt + 1 < find.size())
			{
				++cnt;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

// ������̑��
void func::AddChar(const std::string & add, unsigned char * data, const unsigned int& dataNum)
{
	if (add.size() - 1 > dataNum)
	{
		OutputDebugString(_T("\n��������������񂪃f�[�^���𒴂��Ă��܂�\n"));
		return;
	}

	for (unsigned int i = 0; i < dataNum; ++i)
	{
		data[i] = add[i];
	}
}

// ������̌���
std::string func::FindString(const std::string & path, const char & find, const unsigned int & offset, const bool & end)
{
	unsigned int pos = (end == false) ? path.find_first_of(find) : path.find_last_of(find);
	pos += offset;

	std::string tmp = path.substr(0, pos);

	return tmp;
}

// �t�H���_���ƃt�@�C�����̘A��
std::string func::FileLink(const std::string & path1, const std::string & path2)
{
	int index1 = path1.rfind('/');
	int index2 = path1.rfind('\\');
	int index = max(index1, index2);

	std::string folder = path1.substr(0, index) + "/" + path2;

	return folder; return std::string();
}

// ���C�h������̕ϊ�
std::wstring func::ChangeWString(const std::string & st)
{
	//�������̎擾
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//�ϊ�
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}
