#pragma once
#include "Typedef.h"
#include <vector>
#include <map>
#include <string>

class Load
{
public:
	// デストラクタ
	~Load();

	// インスタンス化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// インスタンス変数の取得
	static Load* GetInstance(void)
	{
		return s_Instance;
	}

	// 読み込み
	bool LoadAct(std::string fileName);

	// ヘッダーの取得
	ImageHeader GetHeader(void);
	// 画像データの取得
	ImageData GetImageData(USHORT index);
	// 分割データの取得
	std::vector<CutData> GetCutData(std::string name);

	// 画像データのサイズの取得
	UINT GetImageDataSize(void);
	// 分割データのサイズの取得
	UINT GetCutDataSize(void);

private:
	// コンストラクタ
	Load();

	// インスタンス変数
	static Load* s_Instance;

	// ヘッダー
	ImageHeader header;
	// 画像データ
	std::map<USHORT, ImageData>data;
	// 分割データ
	std::map<std::string, std::vector<CutData>>cut;
	// 攻撃短形データ
	std::map<std::string, std::map<int, std::vector<Attack>>>attack;
};