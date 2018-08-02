#pragma once
#include "Obj.h"
#include <memory>

class Device;
class Queue;

class Fence :
	public Obj
{
public:
	// コンストラクタ
	Fence(std::weak_ptr<Device>dev, std::weak_ptr<Queue>list);
	// デストラクタ
	~Fence();

	// フェンスの取得
	ID3D12Fence* Get(void) const {
		return fence;
	}

	// フェンス数の取得
	UINT64 GetCnt(void) const {
		return cnt;
	}

	// 待機
	void Wait(void);

private:
	// フェンスの生成
	HRESULT CreateFence(void);

	// フェンスイベントの生成
	HRESULT CreateEvt(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<Queue>queue;

	//フェンス
	ID3D12Fence* fence;

	//フェンスイベント
	HANDLE event;

	//フェンス値
	UINT64 cnt;
};

