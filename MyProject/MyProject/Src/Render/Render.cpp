#include "Render.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../etc/Release.h"
#include <dxgi1_6.h>

//クリアカラー
const FLOAT color[] = {
	0.0f,
	0.0f,
	0.0f,
	0.0f
};

// コンストラクタ
Render::Render(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap) :
	dev(dev), list(list), swap(swap), heap(nullptr), size(0)
{
	rsc.clear();

	CreateHeap();
	CreateRsc();
}

// デストラクタ
Render::~Render()
{
	for (auto itr = rsc.begin(); itr != rsc.end(); ++itr)
	{
		Release(*itr);
	}
	Release(heap);
}

// ヒープの生成
long Render::CreateHeap(void)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = swap.lock()->GetCnt();
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nレンダーターゲット用ヒープの生成：失敗\n"));
		return hr;
	}

	//ヒープサイズの格納
	size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(desc.Type);

	return hr;
}

// リソースの生成
long Render::CreateRsc(void)
{
	//配列のメモリ確保
	rsc.resize(swap.lock()->GetCnt());

	//レンダーターゲット設定用構造体
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	desc.Format               = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension        = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice   = 0;
	desc.Texture2D.PlaneSlice = 0;

	//先頭ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();

	auto hr = S_OK;

	for (UINT i = 0; i < rsc.size(); ++i)
	{
		hr = swap.lock()->Get()->GetBuffer(i, IID_PPV_ARGS(&rsc[i]));
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nバッファの取得：失敗\n"));
			break;
		}

		//レンダーターゲットの生成
		dev.lock()->Get()->CreateRenderTargetView(rsc[i], &desc, handle);

		//ハンドル位置の更新
		handle.ptr += size;
	}

	return hr;
}

// レンダーターゲットのセット
void Render::SetRender(ID3D12DescriptorHeap & depth)
{
	//ヒープの先頭ハンドルの取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += size * swap.lock()->Get()->GetCurrentBackBufferIndex();

	//レンダーターゲットのセット
	list.lock()->GetList()->OMSetRenderTargets(1, &handle, false, &depth.GetCPUDescriptorHandleForHeapStart());

	//レンダーターゲットのクリア
	list.lock()->GetList()->ClearRenderTargetView(handle, color, 0, nullptr);
}
