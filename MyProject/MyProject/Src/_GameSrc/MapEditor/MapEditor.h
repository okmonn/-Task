#pragma once
#include "../etc/Vector2.h"
#include <vector>
#include <string>

class MapEditor
{
	// カーソル
	struct Cursor {
		//ワールド座標
		Vec2f pos;
		//ローカル座標
		Vec2f local;
		//サイズ
		Vec2f size;
		//マップチップ番号
		unsigned int index;
	};

	// カメラ
	struct Camera {
		//座標
		Vec2f pos;
	};

public:
	// コンストラクタ
	MapEditor();
	// デストラクタ
	~MapEditor();

	// マップ情報のセット
	void MapInfo(const Vec2f& mapSize, const Vec2& chipCnt, const Vec2& winSize = { 640, 480 });

	// 描画
	void Draw(void);
	
	// 処理
	void UpData(void);

private:
	// タイルセット1の描画
	void DrawTile1(void);

	// タイルセット2の描画
	void DrawTile2(void);

	// カメラ座標の比較
	void ComparisonPos(float& cam, const float& areaSize, const int& winSize);

	// カメラの更新
	void UpDataCam(void);

	// ローカル座標に変換
	Vec2f ChangeLocal(const Vec2f& pos);

	// タイルの変更
	void ChangeTile(void);

	// マップチップの変更
	void ChangeChip(const int& size);

	// カーソルの移動
	void MoveCursor(void);

	// マップチップの配置
	void SetChip(void);


	// マップのサイズ
	Vec2f mapSize;

	// マップチップの数
	Vec2 chipCnt;

	// ウィンドウサイズ
	Vec2 winSize;

	// カメラ
	Camera cam;

	// カーソル
	Cursor cursor;

	// タイルセット画像
	int tile;

	// 配列番号
	int index;

	// オブジェクト画像
	int obj;

	// タイルセットのサイズ
	int size;

	// タイルマップデータ
	std::vector<int>tileMap;

	// オブジェクトマップデータ
	std::vector<int>objMap;

	// 関数ポインタ
	void (MapEditor::*draw)(void);
};
