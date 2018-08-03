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
	Fence(std::weak_ptr<Device>dev, std::weak_ptr<Queue>queue);
	// デストラクタ
	~Fence();

	// 待機処理
	void Wait(void);

	// フェンスの取得
	ID3D12Fence* Get(void) const {
		return fence;
	}

	// フェンス値の取得
	UINT64 GetCnt(void) const {
		return cnt;
	}

private:
	// フェンスの取得
	HRESULT CreateFence(void);

	// フェンスイベントの生成
	HRESULT CreateEvt(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドキュー
	std::weak_ptr<Queue>queue;

	//フェンス
	ID3D12Fence* fence;

	//フェンスイベント
	HANDLE handle;

	//フェンス値
	UINT64 cnt;
};

