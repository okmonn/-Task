#pragma once
#include "Typedef.h"
#include "Input.h"
#include <vector>
#include <map>
#include <memory>

class Player
{
public:
	// コンストラクタ
	Player(std::weak_ptr<Input>in);
	// デストラクタ
	virtual ~Player();

	// 描画
	void Draw(void);

	// 処理
	void UpData();


	void SetCenter(Position& pos, bool r = false);
	// 状態のセット
	void SetMode(std::string m, bool r = false);
	// 状態の取得
	std::string GetMode(void);

	// 中心点の取得
	Position Getcenter(void);

	// 状態一覧の取得
	std::vector<std::string> GetAllMode(void);

private:
	// 文字列の検索
	std::string FindString(const std::string path, const char find , int offset = 0, bool start = true);
	// 読み込み
	void Load(void);

	// 
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
};

