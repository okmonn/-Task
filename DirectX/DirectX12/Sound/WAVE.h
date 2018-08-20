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

	// 音声データの読み込み
	int Load(void);

	// インデックスの取得
	int GetIndex(void) const {
		return index == 0 ? 1 : 0;
	}
	// 読み込み終了フラグの取得
	bool GetEnd(void) const {
		return end;
	}


	// フォーマット
	WAVEFORMATEX format;

	// コールバック
	VoiceCallback callback;

	// データ
	std::vector<std::vector<BYTE>>data;

private:
	// 文字確認
	int CheckChar(unsigned char * data, int dataSize, const std::string & find);


	// ファイル
	FILE * file;

	// インデックス
	int index;

	// 現在の読み込みバイト数
	int read;

	// 最大読み込みバイト数
	int readMax;

	// 読み込み終了フラグ
	bool end;
};
