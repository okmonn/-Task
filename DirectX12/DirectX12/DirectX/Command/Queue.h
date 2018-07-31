#pragma once
#include "Command.h"
#include <memory>

class Queue : 
	public Command
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


	// コマンドキュー
	ID3D12CommandQueue* queue;
};

