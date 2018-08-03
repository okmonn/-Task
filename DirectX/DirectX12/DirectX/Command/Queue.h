#pragma once
#include "Command.h"

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
	HRESULT CreateQueue(void);


	// コマンドキュー
	ID3D12CommandQueue * queue;
};
