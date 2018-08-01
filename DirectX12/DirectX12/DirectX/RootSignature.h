#pragma once
#include "Obj.h"
#include <memory>

class Device;

class RootSignature :
	public Obj
{
public:
	// コンストラクタ
	RootSignature(std::weak_ptr<Device>dev);
	RootSignature(std::weak_ptr<Device>dev, LPCWSTR fileName, LPCSTR func1, LPCSTR func2);
	// デストラクタ
	~RootSignature();

	// 頂点シェーダのコンパイル
	HRESULT ComVer(LPCWSTR fileName, LPCSTR func, LPCSTR target = "vs_5_0");
	// ピクセルシェーダのコンパイル
	HRESULT ComPix(LPCWSTR fileName, LPCSTR func, LPCSTR target = "ps_5_0");

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const {
		return rootSignature;
	}

	// メッセージの取得
	ID3DBlob* GetSig(void) const {
		return signature;
	}

	// エラーメッセージの取得
	ID3DBlob* GetErr(void) const {
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
	// シリアライズ
	HRESULT Serialize(void);

	// ルートシグネチャの生成
	HRESULT Create(void);
	// ルートシグネチャの生成
	HRESULT CreateSignature(void);


	// デバイス
	std::weak_ptr<Device>dev;

	//ルートシグネチャ
	ID3D12RootSignature* rootSignature;

	//メッセージ
	ID3DBlob* signature;

	//エラーメッセージ
	ID3DBlob* error;

	// 頂点シェーダ
	ID3DBlob* vertex;

	// ピクセルシェーダ
	ID3DBlob* pixel;
};

