#pragma once
#include "Typedef.h"
#include <map>
#include <vector>
#include <string>

class Enemy
{
public:
	// コンストラクタ
	Enemy();
	// デストラクタ
	virtual ~Enemy();

	// 文字列の検索
	std::string FindString(const std::string path, const char find, int offset = 0, bool start = true);
	// 読み込み
	void Load(std::string s);

	// 中心点の設定
	void SetSenter(Position & pos, bool r);
	// 座標の設定
	void SetPos(Positionf pos);
	void SetPos(float y);

	// 座標の取得
	Positionf GetPos(void);
	// 状態の取得
	std::string GetMode(void);
	// 分割情報の取得
	CutData GetCutData(std::string m, UINT index);

	// 状態のセット
	virtual void SetMode(std::string m, bool r) = 0;
	// 描画
	virtual void Draw(void) = 0;
	// 処理
	virtual void UpData(void) = 0;

protected:
	// 画像データ
	int image;

	// フレーム
	int flam;

	// 配列番号
	UINT index;

	// 座標
	Positionf pos;

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

	// 待機フラグ
	bool wait;

	// ジャンプフラグ
	bool fly;

	// 矩形サイズ
	int attackSize;

	// 画像データ
	std::map<std::string, ImageData>data;

	// 分割データ
	std::map < std::string, std::vector<CutData>>cut;

	// あたり矩形のデータ
	std::map < std::string, std::map<int, std::vector<Attack>>>attack;
};

