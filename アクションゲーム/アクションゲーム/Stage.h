#pragma once
#include "Typedef.h"
#include <vector>

class Stage
{
public:
	// コンストラクタ
	Stage();
	// デストラクタ
	virtual ~Stage();
	
	// 読み込み
	void Load(std::string fileName);

	// 処理
	void UpData(void);

	// ステージサイズの取得
	Rect& GetStageRange(void);

	// ステージの敵の取得
	std::vector<UCHAR>GetEnemyData(int minx, int max);
	std::vector<UCHAR>GetEventData(int minx, int max);

private:
	// ステージの全体サイズ
	Rect rect;

	// ステージデータ
	StageHeader data;

	// ステージの敵データ
	std::vector<UCHAR>eData;

	// ステージのイベントデータ
	std::vector<UCHAR>evData;

	// 読み込み
	int readX;

	int readY;
};

