#include "Swap.h"
#include "../Window/Window.h"
#include "Command\Queue.h"
#include <tchar.h>

#pragma comment (lib, "dxgi.lib")

// コンストラクタ
Swap::Swap(std::weak_ptr<Window>win, std::weak_ptr<Queue>queue):
	win(win), queue(queue), factory(nullptr), swap(nullptr), bufferCnt(0)
{
	CreateFactory();
	CreateSwap();
}

// デストラクタ
Swap::~Swap()
{
	Release(swap);
	Release(factory);
}

// ファクトリーの生成
HRESULT Swap::CreateFactory(void)
{
	result = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nファクトリーの生成：失敗\n"));
	}

	return result;
}

// スワップチェインの生成
HRESULT Swap::CreateSwap(void)
{
	//スワップチェイン設定用構造体
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = 2;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = 0;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = win.lock()->GetY();
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = win.lock()->GetX();

	//スワップチェイン生成
	result = factory->CreateSwapChainForHwnd(queue.lock()->Get(), win.lock()->GetHandle(), &desc, nullptr, nullptr, (IDXGISwapChain1**)(&swap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nスワップチェインの生成：失敗\n"));
	}

	//バックバッファ数保存
	bufferCnt = desc.BufferCount;

	return result;
}

// 裏・表の入れ替え
void Swap::Present(void)
{
	swap->Present(1, 0);
}
