#pragma once
#include "VoiceCallback.h"
#include <iostream>
#include <vector>
#include <string>

class WAVE
{
public:
	// コンストラクタ
	WAVE();
	// デストラクタ
	~WAVE();

	// 読み込み
	int Load(const std::string& fileName);

	
	// フォーマット
	WAVEFORMATEX format;

	// コールバック
	VoiceCallback callback;

	// 現在データ
	std::vector<BYTE>data;

private:
	// 文字確認
	int CheckChar(unsigned char * data, int dataSize, const std::string & find);


	// ファイル
	FILE * file;
};
