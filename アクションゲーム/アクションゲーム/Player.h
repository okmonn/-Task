#pragma once
#include "Typedef.h"
#include "Input.h"
#include "Camera.h"
#include <vector>
#include <map>
#include <memory>

class Game;

class Player
{
	friend Game;
public:
	// コンストラクタ
	Player(std::weak_ptr<Input>in, std::weak_ptr<Camera>cam);
	// デストラクタ
	virtual ~Player();

	// ラスタースクロール
	void RasterScroll(int image, const Vector2D& pos, const Vector2D& rect, const Vector2D& size, float expansion = 1.0f, float rotation = 0.0f, float period = 5.0f, float vibration = 5.0f, bool trans = true, bool xturn = false, bool yturn = false);

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
	Positionf GetCamPos(void);
	// 座標のセット
	void SetPos(Positionf pos);
	void SetPos(float y);

	// 状態一覧の取得
	std::vector<std::string> GetAllMode(void);

	// 反転フラグの取得
	bool GetReverse(void);

	// あたり矩形の数
	int GetAttackNum(void);
	// あたり矩形の取得
	Attack GetAttack(USHORT num);
	// あたり座標の取得
	Positionf GetAttackPos(USHORT num, bool flag = false);
	// 分割情報の取得
	CutData GetCut(void);

	void SetBlock(bool b);
	bool GetBlock(void);

	void SetVel(Vector2Df v);
	void SetDamagePW(float pw);

	bool GetMuteki(void);

	int GetHp(void);

	bool GetDie(void);

	bool GetClear(void);

private:
	// 文字列の検索
	std::string FindString(const std::string path, const char find, int offset = 0, bool start = true);
	// 読み込み
	void Load(void);

	// 待機の処理
	void Wait(void);
	// 歩きの処理
	void Walk();
	// ジャンプの処理
	void Jump(void);
	// 着地の処理
	void Ground(void);
	// パンチの処理
	void Punch(void);
	// キックの処理
	void Kick(void);
	// スライディングの処理
	void Sliding(void);
	// しゃがみの処理
	void Down(void);
	// 登りの処理
	void Climb(void);
	// ダメージの処理
	void Damage(void);
	// 死亡の処理
	void Die(void);


	// インプットクラス
	std::weak_ptr<Input>in;

	// カメラクラス
	std::weak_ptr<Camera>cam;

	// 画像データ
	int image;

	std::map<std::string, int>sound;

	// フレーム数
	UINT flam;

	// 座標
	Positionf pos;

	// モード配列
	std::map<std::string, ImageData>data;

	// 状態
	std::vector<std::string>fmode;

	// 分割データ
	std::map<std::string, std::vector<CutData>>cut;

	// あたり矩形のデータ
	std::map < std::string, std::map<int, std::vector<Attack>>>attack;

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

	// 関数ポインタ
	void (Player::*func)(void);

	// 待機フラグ
	bool wait;

	// ジャンプフラグ
	bool fly;

	// 矩形サイズ
	int attackSize;

	// 修正座標
	Positionf camPos;

	bool bl;

	float d;

	bool m;

	int mTime;

	// 体力
	int hp;

	bool die;

	bool clear;
};

