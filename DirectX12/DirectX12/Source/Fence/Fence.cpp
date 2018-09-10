#include "Fence.h"
#include "../Device/Device.h"
#include "../Command/Queue/Queue.h"
#include <tchar.h>

// コンストラクタ
Fence::Fence(std::weak_ptr<Device>dev, std::weak_ptr<Queue>queue) :
	dev(dev), queue(queue), fence(nullptr), handle(nullptr), cnt(0)
{
	CreateFence();
	CreateEvt();
}

// デストラクタ
Fence::~Fence()
{
	Release(fence);
}

// フェンスの取得
HRESULT Fence::CreateFence(void)
{
	result = dev.lock()->Get()->CreateFence(cnt, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nフェンスの生成：失敗\n"));
		return result;
	}

	//フェンス値の更新
	cnt = 1;

	return result;
}

// フェンスイベントの生成
HRESULT Fence::CreateEvt(void)
{
	handle = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (handle == nullptr)
	{
		OutputDebugString(_T("\nフェンスイベントの生成：失敗\n"));
		return S_FALSE;
	}

	return S_OK;
}

// 待機処理
void Fence::Wait(void)
{
	//フェンス値更新
	++cnt;

	//フェンス値を変更
	{
		result = queue.lock()->Get()->Signal(fence, cnt);
		if (FAILED(result))
		{
			OutputDebugString(_T("\nフェンス値の更新：失敗\n"));
			return;
		}
	}

	//完了を待機(ポーリング)
	while (fence->GetCompletedValue() != cnt)
	{
		//フェンスイベントのセット
		result = fence->SetEventOnCompletion(cnt, handle);
		if (FAILED(result))
		{
			OutputDebugString(_T("\nフェンスイベントのセット：失敗\n"));
			return;
		}

		//フェンスイベントの待機
		WaitForSingleObject(handle, INFINITE);
	}
}
