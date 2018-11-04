#pragma once
#include "../etc/Vector2.h"
#include <string>
#include <map>
#include <vector>
#include <list>
#include <memory>
#include <thread>

class Camera;
class Chip;

class Map
{
public:
	// コンストラクタ
	Map(std::weak_ptr<Camera>cam);
	// デストラクタ
	~Map();

	// CSVの読み込み
	int LoadCsv(const std::string& fileName);

	// 描画
	void Draw(const std::string& type);

private:
	// ソート
	void Sort(void);

	// 画像の読み込み
	void Load(const std::string& fileName, const std::string& type);

	

	
	// カメラ
	std::weak_ptr<Camera>cam;

	// スレッド管理フラグ
	bool threadFlag;

	// マップデータ
	std::map<std::string, std::vector<int>>map;

	// マップチップ
	std::map<std::string, std::list<std::shared_ptr<Chip>>>chip;

	// スレッド
	std::map<std::string, std::thread>th;
};
