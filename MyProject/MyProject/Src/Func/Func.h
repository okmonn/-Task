#pragma once
#include <string>

namespace func {
	// ビット変換
	int Bit(const int& i);

	// バイト変換
	int Byte(const int& i);

	// 対数
	int Log(const int& i, const int& n);

	// 累乗
	int Pow(const int& i, const int& n);

	// 文字列の確認
	bool CheckChar(const std::string& find, unsigned char * data, const unsigned int& dataNum = 4);

	// 文字列の代入
	void AddChar(const std::string& add, unsigned char* data, const unsigned int& dataNum = 4);

	// 文字列の検索
	std::string FindString(const std::string& path, const char& find, const unsigned int& offset = 1, const bool& end = true);

	// フォルダ名とファイル名の連結
	std::string FileLink(const std::string& path1, const std::string& path2);

	// ワイド文字列の変換
	std::wstring ChangeWString(const std::string& st);
}
