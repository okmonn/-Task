#pragma once
#include "../Descriptor/Descriptor.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <map>

class Root;
class Pipe;
class Constant;
class Texture;
class VMD;
struct Model;

class PMD :
	public Descriptor
{
	// マテリアル
	struct Mat {
		//基本色
		DirectX::XMFLOAT3 diffuse;
		//透明度
		FLOAT alpha;
		//反射強度
		FLOAT specularity;
		//反射色
		DirectX::XMFLOAT3 specula;
		//環境色
		DirectX::XMFLOAT3 mirror;
		//テクスチャフラグ
		BOOL flag;
	};

public:
	// コンストラクタ
	PMD(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Root>root, 
		std::weak_ptr<Pipe>pipe, std::weak_ptr<Constant>con, std::weak_ptr<Texture>tex, std::weak_ptr<VMD>vmd);
	// デストラクタ
	~PMD();

	// PMD読み込み
	HRESULT LoadPMD(UINT& index, const std::string& fileName);

	// ボーンの回転
	void RotateBorn(UINT& index, const std::string& name, const DirectX::XMMATRIX& matrix);

	// ボーンの回転
	void RotateBorn(UINT& index, UINT& motion);


	// 描画
	void Draw(UINT& index);

	// PMDの消去
	void Delete(UINT& index);

private:
	// テクスチャの読み込み
	HRESULT LoadTex(UINT* index, const std::string& fileName);

	// 定数バッファヒープの生成
	HRESULT CreateHeap(UINT* index);

	// 定数バッファリソースの生成
	HRESULT CreateResource(UINT* index);

	// 定数バッファビューの生成
	HRESULT CreateConView(UINT* index);

	// ボーン用定数バッファリソースの生成
	HRESULT CreateBornResource(UINT* index);

	// ボーン用定数バッファビューの生成
	HRESULT CreateBornConView(UINT* index);

	// 頂点バッファの生成
	HRESULT CreateVeretxBuffer(UINT* index);

	// 頂点インデックスの生成
	HRESULT CreateIndexBuffer(UINT* index);

	// ボーンのセット
	void SetBorn(UINT* index);

	// 子ボーンの回転
	void RotateChildBorn(UINT& index, USHORT id, const DirectX::XMMATRIX& matrix);;


	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// 定数バッファ
	std::weak_ptr<Constant>con;

	// テクスチャ
	std::weak_ptr<Texture>tex;

	// VMD
	std::weak_ptr<VMD>vmd;

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
