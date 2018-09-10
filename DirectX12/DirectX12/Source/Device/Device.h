#pragma once
#include "../BaseObj/Obj.h"

class Device :
	public Obj
{
public:
	// コンストラクタ
	Device();
	// デストラクタ
	~Device();

	// デバイスの取得
	ID3D12Device* Get(void) const {
		return dev;
	}

private:
	// デバイスの生成
	HRESULT CreateDev(void);


	// デバイス
	ID3D12Device * dev;

	// 機能レベル
	D3D_FEATURE_LEVEL level;
};
