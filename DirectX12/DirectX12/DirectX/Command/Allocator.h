#pragma once
#include "../Obj.h"
#include <memory>

class Device;

class Allocator :
	public Obj
{
public:
	// コンストラクタ
	Allocator(std::weak_ptr<Device>dev);
	// デストラクタ
	~Allocator();

	// コマンドアロケータの取得
	ID3D12CommandAllocator* Get(void) const {
		return allo;
	}

private:
	// コマンドアロケータの生成
	HRESULT Create(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドアロケータ
	ID3D12CommandAllocator * allo;
};

