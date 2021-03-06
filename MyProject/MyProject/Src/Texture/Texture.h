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
		std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Texture();

	// 白テクスチャのセット
	void SetWhite(const unsigned int& rootNum = 4);
	// 黒テクスチャのセット
	void SetBlack(const unsigned int& rootNum = 5);
	// グラデーションテクスチャのセット
	void SetGrade(const unsigned int& rootNum = 6);

	// 読み込み
	long Load(const std::string& fileName, int& i);

	// 描画準備
	long SetDraw(int& i, const unsigned int& rootNum = 1);

	// ヒープのセット
	void SetHeap(int& i, const unsigned int& rootNum = 1);

	// 描画
	void Draw(int& i, const float& x, const float& y, const int& turnX, const int& turnY, const float& alpha);

	// 描画・サイズ指定
	void DrawSize(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY, const int& turnX, const int& turnY, const float& alpha);

	// 描画・サイズ・範囲指定
	void DrawRectSize(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const int& turnX, const int& turnY, const float& alpha);

	// 描画・座標4点指定
	void FreelyDraw(int & i, const float & x1, const float & y1, const float & x2, const float & y2, 
		const float & x3, const float & y3, const float & x4, const float & y4, const int & turnX, const int & turnY, const float & alpha);

	// 描画・座標4点・範囲指定
	void FreelyDrawRect(int & i, const float & x1, const float & y1, const float & x2, const float & y2,const float & x3, const float & y3, const float & x4, const float & y4, 
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const int & turnX, const int & turnY, const float & alpha);

	// 削除
	void DeleteImg(int& i);

private:
	// ヒープの生成
	long CreateHeap(ID3D12DescriptorHeap** heap);

	// リソースの生成
	long CreateRsc(ID3D12Resource** rsc, int w, int h);

	// 白テクスチャの生成
	void CreateWhite(void);

	// 黒テクスチャの生成
	void CreateBlack(void);

	// グラデーションテクスチャの生成
	void CreateGrade(void);

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
	std::unique_ptr<TextureLoader>loader;

	// 白テクスチャ
	texture::Dummy white;

	// 黒テクスチャ
	texture::Dummy black;

	// グラデーションテクスチャ
	texture::Dummy grade;

	// 画像データ
	std::map<int*, texture::Tex>tex;
};
