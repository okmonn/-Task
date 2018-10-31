#pragma once
#include "../etc/Vector2.h"
#include <vector>

class MapEditor
{
public:
	// コンストラクタ
	MapEditor();
	// デストラクタ
	~MapEditor();

	// マップ情報のセット
	void MapInfo(const Vec2f& mapSize, const Vec2& chipCnt);

	// 描画
	void Draw(void);
	
	// 処理
	void UpData(void);

private:
	// カーソル座標
	Vec2f pos;

	// カーソルサイズ
	Vec2f size;

	// マップのサイズ
	Vec2f mapSize;

	// マップチップの数
	Vec2 chipCnt;

	// マップチップのインデックス
	unsigned int index;

	// マップデータ
	std::vector<char> map;
};
