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
	bool LoadMap(std::string fileName);

	// ヘッダーの取得
	ImageHeader GetHeader(std::string m);
	// 画像データの取得
	ImageData GetImageData(std::string m, USHORT index);
	// 分割データの取得
	std::vector<CutData> GetCutData(std::string m, std::string name);
	// あたり短径の取得
	std::map<std::string, std::map<int, std::vector<Attack>>> GetAttac(std::string m);

	// 画像データのサイズの取得
	UINT GetImageDataSize(std::string m);
	// 分割データのサイズの取得
	UINT GetCutDataSize(std::string m);

	// ステージヘッダーの取得
	StageHeader GetStageHeader(std::string fileName);

	std::vector<UCHAR>GetEnemyData(std::string p,int min, int max);

private:
	// コンストラクタ
	Load();

	// インスタンス変数
	static Load* s_Instance;

	// ヘッダー
	std::map<std::string, ImageHeader> header;
	// 画像データ
	std::map<std::string, std::map<USHORT, ImageData>>data;
	// 分割データ
	std::map<std::string, std::map<std::string, std::vector<CutData>>>cut;
	// 攻撃短形データ
	std::map<std::string, std::map<std::string, std::map<int, std::vector<Attack>>>>attack;

	// ステージヘッダー
	std::map<std::string, StageHeader>st;

	// ステージの敵データ
	std::map<std::string, std::vector<UCHAR>>eData;
};