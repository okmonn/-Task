#pragma once
#include "Typedef.h"
#include "Input.h"
#include <vector>
#include <map>
#include <memory>

class Game;

class Player
{
	friend Game;
public:
	// コンストラクタ
	Player(std::weak_ptr<Input>in);
	// デストラクタ
	virtual ~Player();

	// 描画
	void Draw(void);

	// 処理
	void UpData();

	// 中心座標のセット
	void SetCenter(Position& pos, bool r = false);
	// 状態のセット
	void SetMode(std::string m, bool r = false);
	// 状態の取得
	std::string GetMode(void);

	// 座標の取得
	Positionf GetPos(void);
	// 座標のセット
	void SetPos(Positionf pos);
	void SetPos(float y);

	// 状態一覧の取得
	std::vector<std::string> GetAllMode(void);

	// 反転フラグの取得
	bool GetReverse(void);

private:
	// 文字列の検索
	std::string FindString(const std::string path, const char find , int offset = 0, bool start = true);
	// 読み込み
	void Load(void);

	// 待機の処理
	void Wait(void);
	// 歩きの処理
	void Walk();
	// ジャンプの処理
	void Jump(void);
	// パンチの処理
	void Panch(void);
	// キックの処理
	void Kick(void);
	// スライディングの処理
	void Sliding(void);
	// しゃがみの処理
	void Down(void);
	// ダメージの処理
	void Damage(void);

	// 状態の変更
	void ChangeMode(void);



	// インプットクラス
	std::weak_ptr<Input>in;

	// 画像データ
	int image;
	
	// フレーム数
	UINT flam;

	// 座標
	Positionf pos;

	// 分割
	Rect rect;

	// モード配列
	std::map<std::string, ImageData>data;

	// 状態
	std::vector<std::string>fmode;

	// 分割データ
	std::map<std::string, std::vector<CutData>>cut;

	// 配列番号
	UINT index;

	// 状態
	std::string mode;

	// 中心
	Position center;

	// 反転フラグ
	bool reverse;

	// ループフラグ
	bool loop;

	// 速度
	Vector2Df vel;

	// 
	void (Player::*func)(void);
};

