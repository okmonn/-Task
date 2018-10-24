#pragma once
#include "PmdData.h"
#include <memory>

class Device;
class List;
class ModelLoader;

class Model
{
public:
	// コンストラクタ
	Model(std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~Model();

	// 読み込み
	long Load(const std::string& fileName, int& i);

private:
	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// モデルローダー
	std::shared_ptr<ModelLoader>loader;

	// 頂点
	std::map<int*, std::weak_ptr<std::vector<pmd::Vertex>>>vertex;
};
