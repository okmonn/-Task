#pragma once
#include "Typedef.h"
#include <map>
#include <vector>

class Event
{
public:
	// コンストラクタ
	Event();
	// デストラクタ
	virtual ~Event();

	// 文字列の検索
	std::string FindString(const std::string path, const char find, int offset, bool start);

	// 読み込み
	void Load(std::string fileName);

	// あたり判定
	bool CheackHit(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2);
	// あたり判定
	bool CheackHit2(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2);

	// 描画
	virtual void Draw(void) = 0;

	// 処理
	virtual void UpData(void) = 0;

protected:
	// 座標
	Positionf pos;

	// カメラ座標
	Positionf camPos;

	// 画像
	int image;

	// 矩形サイズ
	int attackSize;

	// モード
	std::string mode;

	// フレーム
	int flam;

	// 配列番号
	UINT index;

	// 画像データ
	std::map<std::string, ImageData>data;

	// 分割データ
	std::map < std::string, std::vector<CutData>>cut;

	// あたり矩形のデータ
	std::map < std::string, std::map<int, std::vector<Attack>>>attack;
};

