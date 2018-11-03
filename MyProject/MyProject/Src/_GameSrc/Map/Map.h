#pragma once
#include "../etc/Vector2.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

class Camera;

class Map
{
public:
	// コンストラクタ
	Map(std::weak_ptr<Camera>cam);
	// デストラクタ
	~Map();

	// CSVの読み込み
	int LoadCsv(const std::string& dir);

private:
	// カメラ
	std::weak_ptr<Camera>cam;

	// マップデータ
	std::map<int, std::vector<int>>map;
};
