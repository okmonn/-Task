#pragma once
#include "TextureData.h"
#include <string>
#include <map>

class Device;
class List;
class Constant;
class Constant;
class Root;
class Pipe;
class TextureLoader;

class Texture
{
public:
	// コンストラクタ
	Texture(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con, 
		std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<TextureLoader>loader);
	// デストラクタ
	~Texture();

	// 読み込み
	long Load(const std::string& fileName, int& i);

	// 描画準備
	long SetDraw(int& i, const unsigned int& rootNum = 1);

	// ヒープのセット
	void SetHeap(int& i, const unsigned int& rootNum = 1);

	// 描画
	void Draw(int& i, const float& x, const float& y, const float& alpha, const int& turnX, const int& turnY);

private:
	// リソースビューの生成
	void CreateView(int* i);

	// 頂点リソースの生成
	long CreateRsc(int* i);

	// マップ
	long Map(int* i);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// 定数バッファ
	std::weak_ptr<Constant>con;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// テクスチャローダー
	std::weak_ptr<TextureLoader>loader;

	// 画像データ
	std::map<int*, Tex>tex;
};
