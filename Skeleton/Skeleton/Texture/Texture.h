#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <DirectXMath.h>

struct ID3D12GraphicsCommandList;
struct D3D12_SUBRESOURCE_DATA;
struct ID3D12Resource;
class DescriptorMane;
class TextureLoader;
class Window;
class Device;
class List;
class Root;
class Pipe;

namespace tex {
	// 頂点
	struct Vertex {
		//座標
		DirectX::XMFLOAT3 pos;
		//uv
		DirectX::XMFLOAT2 uv;
	};

	struct Info
	{
		//移動行列
		DirectX::XMFLOAT4X4 matrix;
		//ウィンドウサイズ
		DirectX::XMFLOAT2 window;
		//uv座標
		DirectX::XMFLOAT2 uvPos;
		//uvサイズ
		DirectX::XMFLOAT2 uvSize;
		//反転
		DirectX::XMFLOAT2 reverse;
		//アルファ値
		float alpha;
	};
}

class Texture
{
	//画像データ
	struct Tex {
		//画像リソース
		ID3D12Resource* rsc;
		//デコード
		unsigned char* decode;
		//サブデータ
		std::weak_ptr<D3D12_SUBRESOURCE_DATA>sub;
		//定数リソースID
		int cRsc;
		//定数送信データ
		tex::Info* info;
		//バンドルリスト
		std::unique_ptr<List>list;
	};

public:
	// コンストラクタ
	Texture(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Texture();

	// 読み込み
	void Load(const std::string& fileName, int& i);

	// 配列データから画像を生成
	void CreateImg(const std::vector<unsigned char>&data, const unsigned int& width, const unsigned int& height, int& i);

	// 描画
	void Draw(std::weak_ptr<List>list, int& i, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size,
		const DirectX::XMFLOAT2& uvPos, const DirectX::XMFLOAT2& uvSize, const float& alpha = 1.0f,
		const bool& turnX = false, const bool& turnY = false);

	// 削除
	void Delete(int& i);

private:
	// 頂点のセット
	void SetVertex(void);

	// 頂点リソースの生成
	long CreateVertexRsc(void);

	// 頂点リソースのマップ
	long MapVertex(void);

	// 定数リソースの生成
	long CreateConRsc(int* i);

	// 定数バッファビューの生成
	void CreateConView(int* i);

	// シェーダーリソースの生成
	long CreateShaderRsc(int* i, const unsigned int& width, const unsigned int& height);

	// シェーダーリソースビューの生成
	void CreateShaderView(int* i);

	// サブリソースに書き込み
	long WriteSub(int* i);

	// 作成テクスチャのサブリソースに書き込み
	long WriteSub(int* i, const std::vector<unsigned char>&data);

	// 定数バッファのマップ
	long MapCon(int* i);

	// バンドルのセット
	void SetBundle(int* i);


	// ディスクリプターマネージャー
	DescriptorMane& descMane;

	// テクスチャローダー
	TextureLoader& loader;

	// ウィンドウ
	std::weak_ptr<Window>win;

	// デバイス
	std::weak_ptr<Device>dev;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// 頂点リソース
	int vRsc;

	// 頂点送信用データ
	void* vertexData;

	// 頂点
	std::vector<tex::Vertex>vertex;

	// 画像データ
	std::unordered_map<int*, Tex>tex;
};
