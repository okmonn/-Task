#pragma once
#include "Obj.h"
#include <memory>

class Device;

class Root :
	public Obj
{
public:
	// コンストラクタ
	Root(std::weak_ptr<Device>dev);
	// デストラクタ
	~Root();

	// 頂点シェーダのコンパイル
	HRESULT ComVertex(LPCWSTR fileName, LPCSTR func, LPCSTR target = "vs_5_0");
	// ピクセルシェーダのコンパイル
	HRESULT ComPixel(LPCWSTR fileName, LPCSTR func, LPCSTR target = "ps_5_0");

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

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const {
		return root;
	}

private:
	// シリアライズ
	HRESULT Serialize(void);

	// ルートシグネチャの生成
	HRESULT CreateRoot(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// メッセージ
	ID3DBlob * signature;

	// エラーメッセージ
	ID3DBlob* error;

	// 頂点シェーダ
	ID3DBlob* vertex;

	// ピクセルシェーダ
	ID3DBlob* pixel;

	// ルートシグネチャ
	ID3D12RootSignature * root;
};

