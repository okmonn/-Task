#include "Render.h"
#include "../Device.h"
#include "../Swap.h"

// コンストラクタ
Render::Render(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap)
{
	this->dev = dev;
	this->swap = swap;


	Create();
}

// デストラクタ
Render::~Render()
{
	Liberation();
}

// リソースの生成
HRESULT Render::Create(void)
{
	result = CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	if (FAILED(result))
	{
		return result;
	}

	//レンダーターゲット設定用構造体
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	desc.Format               = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension        = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice   = 0;
	desc.Texture2D.PlaneSlice = 0;

	resource.resize(swap.lock()->GetBack());

	//先頭ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle = GetHeap()->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < resource.size(); ++i)
	{
		result = swap.lock()->Get()->GetBuffer(i, IID_PPV_ARGS(&resource[i]));

		OutDebug(L"\nバッファの取得：失敗\n", result);

		if (FAILED(result))
		{
			break;
		}

		//レンダーターゲットの生成
		dev.lock()->Get()->CreateRenderTargetView(resource[i], &desc, handle);

		//ハンドル位置の更新
		handle.ptr += size;
	}

	return result;
}
