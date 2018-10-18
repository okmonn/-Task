#pragma once
#include <memory>

struct ID3D12Fence;

class Device;
class Queue;

class Fence
{
public:
	// コンストラクタ
	Fence(std::weak_ptr<Device>dev, std::weak_ptr<Queue>queue);
	// デストラクタ
	~Fence();

	// 待機
	void Wait(void);

	// フェンスの取得
	ID3D12Fence* Get(void) const {
		return fence;
	}
	// フェンスイベントの取得
	void* GetHandle(void) const {
		return handle;
	}
	// フェンスカウントの取得
	long long GetCnt(void) const {
		return cnt;
	}

private:
	// フェンスの生成
	long Create(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドキュー
	std::weak_ptr<Queue>queue;

	// フェンス
	ID3D12Fence* fence;

	// フェンスイベント
	void* handle;

	// フェンスカウン
	long long cnt;
};
