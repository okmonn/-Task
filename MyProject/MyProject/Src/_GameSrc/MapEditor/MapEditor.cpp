#include "MapEditor.h"
#include "../../Func/Func.h"

// コンストラクタ
MapEditor::MapEditor() :
	mapSize(0.0f), chipCnt(0), winSize(0)
{
	cursor = {};
	cam = {};
	map.clear();
}

// デストラクタ
MapEditor::~MapEditor()
{
}

// マップ情報のセット
void MapEditor::MapInfo(const Vec2f & mapSize, const Vec2 & chipCnt, const Vec2& winSize)
{
	this->mapSize = mapSize;
	this->chipCnt = chipCnt;
	this->winSize = winSize;
	cursor.size = { mapSize.x / chipCnt.x, mapSize.y / chipCnt.y };
	map.resize(chipCnt.x * chipCnt.y);
}

// 描画
void MapEditor::Draw(void)
{
	for (int y = 0; y < chipCnt.y; ++y)
	{
		func::DrawLine(0, mapSize.y / chipCnt.y * y, mapSize.x, mapSize.y / chipCnt.y * y, 1, 1, 1);
	}
	for (int x = 0; x < chipCnt.x; ++x)
	{
		func::DrawLine(mapSize.x / chipCnt.x * x, 0, mapSize.x / chipCnt.x * x, mapSize.y, 1, 1, 1);
	}

	func::DrawBox(cursor.local.x, cursor.local.y, cursor.size.x, cursor.size.y, 1, 1, 1);
}

// 処理
void MapEditor::UpData(void)
{
	UpDataCam();
	cursor.local = ChangeLocal(cursor.pos);

	Vec2f size = { mapSize.x / chipCnt.x , mapSize.y / chipCnt.y };

	if (func::CheckTriger(INPUT_RIGHT))
	{
		cursor.pos.x += (cursor.pos.x + size.x < mapSize.x) ? size.x : 0.0f;
	}
	else if (func::CheckTriger(INPUT_LEFT))
	{
		cursor.pos.x -= (cursor.pos.x - size.x >= 0.0f) ? size.x : 0.0f;
	}
	else if (func::CheckTriger(INPUT_DOWN))
	{
		cursor.pos.y += (cursor.pos.y + size.y < mapSize.y) ? size.y : 0.0f;
	}
	else if (func::CheckTriger(INPUT_UP))
	{
		cursor.pos.y -= (cursor.pos.y - size.y >= 0.0f) ? size.y : 0.0f;
	}
}

// カメラの更新
void MapEditor::UpDataCam(void)
{
	if (cursor.pos.x > winSize.x / 2)
	{
		cam.pos = cursor.pos;
	}
}

// ローカル座標に変換
Vec2f MapEditor::ChangeLocal(const Vec2f & pos)
{
	Vec2f tmp = pos;
	return (tmp - cam.pos);
}
