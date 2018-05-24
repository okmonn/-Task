#pragma once
#include "Typedef.h"

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
};

