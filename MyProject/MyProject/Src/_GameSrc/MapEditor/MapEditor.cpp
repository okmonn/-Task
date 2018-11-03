#include "MapEditor.h"
#include "../etc/TilesetData.h"
#include "../../Func/Func.h"

// コンストラクタ
MapEditor::MapEditor() :
	mapSize(0.0f), chipCnt(0), winSize(0), tile(0), index(0), obj(0), size(_countof(tile1))
{
	cursor = {};
	cam = {};

	tileMap.clear();
	objMap.clear();

	func::LoadImg("Rsc/tileset.png", tile);
	func::LoadImg("Rsc/objects-Sheet.png", obj);

	draw = &MapEditor::DrawTile1;
}

// デストラクタ
MapEditor::~MapEditor()
{
	func::DeleteImg(obj);
	func::DeleteImg(tile);
}

// マップ情報のセット
void MapEditor::MapInfo(const Vec2f & mapSize, const Vec2 & chipCnt, const Vec2& winSize)
{
	this->mapSize = mapSize;
	this->chipCnt = chipCnt;
	this->winSize = winSize;
	cursor.size = { mapSize.x / chipCnt.x, mapSize.y / chipCnt.y };

	tileMap.resize(chipCnt.x * chipCnt.y);
	memset(tileMap.data(), -1, sizeof(int) * tileMap.size());
	for (auto& i : tileMap)
	{
		func::LoadImg("Rsc/tileset.png", i);
	}
	objMap.resize(chipCnt.x * chipCnt.y);
	memset(objMap.data(), -1, sizeof(int) * objMap.size());
	for (auto& i : objMap)
	{
		func::LoadImg("Rsc/objects-Sheet.png", i);
	}
}

// タイルセット1の描画
void MapEditor::DrawTile1(void)
{
	func::DrawRectImg(tile, cursor.local.x, cursor.local.y, mapSize.x / chipCnt.x * tile1[index].ratio.x, mapSize.y / chipCnt.y * tile1[index].ratio.y,
		tile1[index].pos.x, tile1[index].pos.y, tile1[index].size.x, tile1[index].size.y);
}

// タイルセット2の描画
void MapEditor::DrawTile2(void)
{
	func::DrawRectImg(obj, cursor.local.x, cursor.local.y, mapSize.x / chipCnt.x * tile2[index].ratio.x, mapSize.y / chipCnt.y * tile2[index].ratio.y,
		tile2[index].pos.x, tile2[index].pos.y, tile2[index].size.x, tile2[index].size.y);
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

	(this->*draw)();

	int cnt = 0;
	for (auto& i : tileMap)
	{
		if (i != -1)
		{
			auto pos = ChangeLocal({ (mapSize.x / chipCnt.x) * (cnt / chipCnt.x), (mapSize.y / chipCnt.y) * (cnt % chipCnt.y) });
			func::DrawRectImg(i, pos.x, pos.y, 
				mapSize.x / chipCnt.x * tile1[i].ratio.x, mapSize.y / chipCnt.y * tile1[i].ratio.y,
				tile1[i].pos.x, tile1[i].pos.y, tile1[i].size.x, tile1[i].size.y);
		}
		++cnt;
	}
}

// カメラ座標の比較
void MapEditor::ComparisonPos(float & cam, const float & areaSize, const int & winSize)
{
	auto size = (float)(winSize / 2);

	if (cam - size < 0.0f)
	{
		cam = size;
	}
	else if (cam + size > areaSize)
	{
		cam = areaSize - size;
	}

	if (cam < 0.0f)
	{
		cam = 0.0f;
	}
}

// カメラの更新
void MapEditor::UpDataCam(void)
{
	cam.pos = cursor.pos;
	ComparisonPos(cam.pos.x, mapSize.x, winSize.x);
	ComparisonPos(cam.pos.y, mapSize.y, winSize.y);
}

// ローカル座標に変換
Vec2f MapEditor::ChangeLocal(const Vec2f & pos)
{
	Vec2f tmp = pos;
	tmp -= { (cam.pos.x - (float)(winSize.x / 2)), (cam.pos.y - (float)(winSize.y / 2)) };
	
	Vec2f remainder = { (float)((int)tmp.x % ((int)mapSize.x / chipCnt.x)), (float)((int)tmp.y % ((int)mapSize.y / chipCnt.y)) };
	tmp -= remainder;

	return tmp;
}

// タイルの変更
void MapEditor::ChangeTile(void)
{
	if (func::CheckTriger(INPUT_W))
	{
		size = _countof(tile1);
		index = (index >= size) ? size - 1 : index;
		draw = &MapEditor::DrawTile1;
	}
	else if (func::CheckTriger(INPUT_S))
	{
		size = _countof(tile2);
		index = (index >= size) ? size - 1 : index;
		draw = &MapEditor::DrawTile2;
	}
}

// マップチップの変更
void MapEditor::ChangeChip(const int & size)
{
	if (func::CheckTriger(INPUT_D))
	{
		index += (index + 1 < size) ? 1 : 0;
	}
	else if (func::CheckTriger(INPUT_A))
	{
		index -= (index - 1 >= 0) ? 1 : 0;
	}
}

// カーソルの移動
void MapEditor::MoveCursor(void)
{
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

// マップチップの配置
void MapEditor::SetChip(void)
{
	if (func::CheckTriger(INPUT_SPACE))
	{
		auto x = (int)(cursor.pos.x / (mapSize.x / chipCnt.x));
		auto y = (int)(cursor.pos.y / (mapSize.y / chipCnt.y));

		if (draw == &MapEditor::DrawTile1)
		{
			tileMap[chipCnt.x * x + y] = (tileMap[y + x] == -1) ? index : -1;
		}
		else
		{
			objMap[chipCnt.x * x + y] = (objMap[y + x] == -1) ? index : -1;
		}
	}
}

// 処理
void MapEditor::UpData(void)
{
	UpDataCam();
	cursor.local = ChangeLocal(cursor.pos);

	ChangeTile();
	ChangeChip(size);
	MoveCursor();

	SetChip();
}

