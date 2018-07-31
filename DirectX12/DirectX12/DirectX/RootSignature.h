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
	// デストラクタ
	~RootSignature();

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

private:
	// シリアライズ
	HRESULT Serialize(void);

	// ルートシグネチャの生成
	HRESULT Create(void);


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

