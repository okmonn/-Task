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

private:
	// ステージの全体サイズ
	Rect rect;

	// ステージデータ
	StageHeader data;

	// ステージの敵データ
	std::vector<UCHAR>eData;
};

