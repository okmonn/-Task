#pragma once
#include "Scene.h"
#include <memory>

class Camera;
class Map;
class Player;

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
	std::shared_ptr<Camera>cam;
	std::shared_ptr<Map>map;
	std::shared_ptr<Player>pl;
};
