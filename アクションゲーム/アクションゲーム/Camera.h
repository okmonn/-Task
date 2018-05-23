#pragma once
#include "Stage.h"
#include "Typedef.h"
#include <memory>

class Player;

class Camera
{
public:
	// コンストラクタ
	Camera(std::weak_ptr<Stage>st);
	// デストラクタ
	virtual ~Camera();

	// 中心オブジェクトの設定
	void SetFocus(std::shared_ptr<Player>pl);

	// 座標の取得
	Positionf& GetPos(void);

	// 目視サイズの取得
	Rect& GetViewSize(void);

	// 描画
	void Draw(void);
	// 処理
	void UpData(void);

	// 座標修正
	Positionf CorrectionPos(Positionf pos);

private:
	// プレイヤークラス
	std::weak_ptr<Player>pl;
	
	// ステージクラス
	std::weak_ptr<Stage>st;

	// 座標
	Positionf pos;
};

