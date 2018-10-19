#include "Func.h"
#include <Windows.h>
#include <algorithm>
#include <tchar.h>

// ビット変換
int func::Bit(const int & i)
{
	return i * 8;
}

// バイト変換
int func::Byte(const int & i)
{
	return i / 8;
}

// 対数
int func::Log(const int & i, const int & n)
{
	return static_cast<int>(std::log(n) / std::log(i));
}

// 累乗
int func::Pow(const int & i, const int & n)
{
	return (n == 0) ? 1 : i << (n - 1);
}

// 文字列の確認
bool func::CheckChar(const std::string & find, unsigned char * data, const unsigned int& dataNum)
{
	if (find.size() - 1 > dataNum)
	{
		OutputDebugString(_T("\n見つけたい文字列がデータ数を超えています\n"));
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

// 文字列の代入
void func::AddChar(const std::string & add, unsigned char * data, const unsigned int& dataNum)
{
	if (add.size() - 1 > dataNum)
	{
		OutputDebugString(_T("\n代入したい文字列がデータ数を超えています\n"));
		return;
	}

	for (unsigned int i = 0; i < dataNum; ++i)
	{
		data[i] = add[i];
	}
}

// 文字列の検索
std::string func::FindString(const std::string & path, const char & find, const unsigned int & offset, const bool & end)
{
	unsigned int pos = (end == false) ? path.find_first_of(find) : path.find_last_of(find);
	pos += offset;

	std::string tmp = path.substr(0, pos);

	return tmp;
}

// フォルダ名とファイル名の連結
std::string func::FileLink(const std::string & path1, const std::string & path2)
{
	int index1 = path1.rfind('/');
	int index2 = path1.rfind('\\');
	int index = max(index1, index2);

	std::string folder = path1.substr(0, index) + "/" + path2;

	return folder; return std::string();
}

// ワイド文字列の変換
std::wstring func::ChangeWString(const std::string & st)
{
	//文字数の取得
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//変換
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, st.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}
