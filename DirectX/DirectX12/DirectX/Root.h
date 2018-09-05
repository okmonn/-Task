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

	// ルートシグネチャの生成
	HRESULT CreateRoot(const D3D12_ROOT_PARAMETER* range, UINT num);

	// メッセージの取得
	ID3DBlob* GetMsg(void) const {
		return signature;
	}
	// エラーメッセージの取得
	ID3DBlob* GetError(void) const {
		return error;
	}
	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const {
		return root;
	}

private:
	// ルートシグネチャの生成
	HRESULT CreateRoot(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// メッセージ
	ID3DBlob * signature;

	// エラーメッセージ
	ID3DBlob* error;

	// ルートシグネチャ
	ID3D12RootSignature * root;
};

