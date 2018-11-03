#pragma once
#include "Scene.h"
#include <memory>

class MapEditor;

class Title :
	public Scene
{
public:
	// コンストラクタ
	Title();
	// デストラクタ
	~Title();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	std::shared_ptr<MapEditor>map;
};
