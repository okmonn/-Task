#pragma once
#include <string>
#include <memory>

struct ID3D12RootSignature;
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct D3D12_ROOT_PARAMETER;

class Device;

class Root
{
public:
	// コンストラクタ
	Root(std::weak_ptr<Device>dev, const std::wstring& fileName);
	// デストラクタ
	~Root();

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const {
		return root;
	}
	// メッセージの取得
	ID3DBlob* GetMsg(void) const {
		return signature;
	}
	// エラーメッセージの取得
	ID3DBlob* GetError(void) const {
		return error;
	}
	// 頂点シェーダの取得
	ID3DBlob* GetVertex(void) const {
		return vertex;
	}
	// ピクセルシェーダの取得
	ID3DBlob* GetPixel(void) const {
		return pixel;
	}

private:
	// シェーダのコンパイル
	long ShaderCompile(const std::wstring& fileName);

	// ルートシグネチャの生成
	long Create(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// ルートシグネチャ
	ID3D12RootSignature* root;

	// メッセージ
	ID3DBlob * signature;

	// エラーメッセージ
	ID3DBlob* error;

	// 頂点シェーダ
	ID3DBlob* vertex;

	// ピクセルシェーダ
	ID3DBlob* pixel;
};
