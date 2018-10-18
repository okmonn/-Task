#pragma once
#include <memory>

struct ID3D12CommandQueue;
class Device;

class Queue
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
	long Create(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドキュー
	ID3D12CommandQueue* queue;
};
