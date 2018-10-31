#include "MapEditor.h"
#include "../../Func/Func.h"

// コンストラクタ
MapEditor::MapEditor() :
	index(0), pos(0.0f), size(0.0f), mapSize(0.0f), chipCnt(0)
{
	map.clear();
}

// デストラクタ
MapEditor::~MapEditor()
{
}

// マップ情報のセット
void MapEditor::MapInfo(const Vec2f & mapSize, const Vec2 & chipCnt)
{
	this->mapSize = mapSize;
	this->chipCnt = chipCnt;
}

// 描画
void MapEditor::Draw(void)
{
	for (int y = 0; y < chipCnt.y; ++y)
	{
		func::DrawLine(0, mapSize.y / chipCnt.y * y, mapSize.x, mapSize.y / chipCnt.y * y, 1, 1, 1);
	}
}

// 処理
void MapEditor::UpData(void)
{
}
