#pragma once
#include "../Descriptor/Descriptor.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <map>

class Pipe;
class Texture;
struct Model;

class PMD :
	public Descriptor
{
	// マテリアル
	struct Mat {
		//基本色
		DirectX::XMFLOAT3 diffuse;
		//テクスチャフラグ
		BOOL flag;
	};

public:
	// コンストラクタ
	PMD(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Pipe>pipe, std::weak_ptr<Texture>tex);
	// デストラクタ
	~PMD();

	// PMD読み込み
	HRESULT LoadPMD(UINT& index, const std::string& fileName);

	// 描画
	void Draw(UINT& index);

private:
	// テクスチャの読み込み
	HRESULT LoadTex(UINT* index, const std::string& fileName);

	// 定数バッファヒープの生成
	HRESULT CreateHeap(UINT* index);

	// 定数バッファリソースの生成
	HRESULT CreateResource(UINT* index);

	// 定数バッファビューの生成
	HRESULT CreateConView(UINT* index);

	// 頂点バッファの生成
	HRESULT CreateVeretxBuffer(UINT* index);

	// 頂点インデックスの生成
	HRESULT CreateIndexBuffer(UINT* index);


	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// テクスチャ
	std::weak_ptr<Texture>tex;

	// モデルデータ
	std::map<UINT*, Model>model;

	// マテリアル
	Mat mat;
};

namespace func {
	// 文字列の検索・先頭から抜き出し
	std::string FindFirstString(const std::string& path, const CHAR& find, UINT offset = 1, bool end = false);

	// 文字列の検索・見つけた場所から抜き出し
	std::string FindEndString(const std::string& path, const CHAR& find, UINT offset = 1, bool end = true);

	// フォルダー名とファイル名の連結
	std::string FileLink(const std::string& path1, const std::string& path2);
}
