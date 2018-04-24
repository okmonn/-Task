#pragma once
#include "Typedef.h"

class Player
{
public:
	// コンストラクタ
	Player();
	// デストラクタ
	virtual ~Player();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// 文字列の検索
	std::string FindString(const std::string path, const char find , int offset = 0, bool start = true);
	// 読み込み
	void Load(void);


	// 画像データ
	int image;
	
	// フレーム数
	int flam;

	// 座標
	Positionf pos;

	// 分割
	Rect rect;
};

