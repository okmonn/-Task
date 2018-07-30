#pragma once
#include "../Obj.h"
#include <memory>

class Device;

class Queue : 
	public Obj
{
public:
	// コンストラクタ
	Queue(std::weak_ptr<Device>dev);
	// デストラクタ
	~Queue();

	// コマンドキューの取得
	ID3D12CommandQueue* Get(void) const {
		return queue;
	}

private:
	// コマンドキューの生成
	HRESULT Create(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドキュー
	ID3D12CommandQueue* queue;
};

