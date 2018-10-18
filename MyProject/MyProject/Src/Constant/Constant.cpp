#include "Constant.h"
#include "../Union/Union.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../etc/Release.h"

// リソース最大数
#define RSC_MAX 1

#define RAD(X) (X) * 3.14159265f / 180.0f

// コンストラクタ
Constant::Constant(std::weak_ptr<Device>dev, std::weak_ptr<List>list) : 
	un(Union::Get()), dev(dev), list(list), heap(nullptr), rsc(nullptr), data(nullptr)
{
	wvp = {};

	Init();
}

// デストラクタ
Constant::~Constant()
{
	UnMap(rsc);
	Release(rsc);
	Release(heap);
}

// WVPの初期化
void Constant::InitWvp(void)
{
	//ダミー宣言
	FLOAT pos = 0.0f;
	//カメラの位置
	DirectX::XMVECTOR eye    = { 0, pos,  -1 };
	//カメラの焦点 
	DirectX::XMVECTOR target = { 0, pos,   0 };
	//カメラの上方向
	DirectX::XMVECTOR upper  = { 0, 1,     0 };

	DirectX::XMStoreFloat4x4(&wvp.view, DirectX::XMMatrixLookAtLH(eye, target, upper));
	DirectX::XMStoreFloat4x4(&wvp.projection, DirectX::XMMatrixPerspectiveFovLH(RAD(90.0f), (float)un.GetWinX() / (float)un.GetWinY(), 0.5f, 500.0f));
	DirectX::XMStoreFloat4x4(&wvp.world, DirectX::XMMatrixIdentity());

	wvp.winSize = { (float)un.GetWinX(), (float)un.GetWinY() };
}

// ヒープの生成
long Constant::CreateHeap(void)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = RSC_MAX;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n定数バッファ用ヒープの生成：失敗\n"));
	}

	return hr;
}

// リソースの生成
long Constant::CreateRsc(void)
{
	//プロパティ設定用構造体の設定
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

	//リソース設定用構造体の設定
	D3D12_RESOURCE_DESC desc = {};
	desc.Alignment          = 0;
	desc.DepthOrArraySize   = 1;
	desc.Dimension          = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags              = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height             = 1;
	desc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels          = 1;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width              = (sizeof(WVP) + 0xff) &~ 0xff;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n定数バッファ用リソースの生成：失敗\n"));
	}

	return hr;
}

// リソースビューの生成
void Constant::CreateView(void)
{
	//定数バッファビュー設定用構造体
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = rsc->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(WVP) + 0xff) &~0xff;

	dev.lock()->Get()->CreateConstantBufferView(&desc, heap->GetCPUDescriptorHandleForHeapStart());
}

// マップ
long Constant::Map(void)
{
	auto hr = rsc->Map(0, nullptr, (void**)(&data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n定数バッファのマップ：失敗\n"));
		return hr;
	}

	//データをコピー
	memcpy(data, &wvp, sizeof(WVP));

	return hr;
}

// 初期化
void Constant::Init(void)
{
	InitWvp();
	CreateHeap(); 
	CreateRsc();
	CreateView();
	Map();
}

// WVPの変更
void Constant::ChangeWvp(const float & eyeX, const float & eyeY, const float & eyeZ, const float & targetX, const float & targetY, const float & targetZ, const float & upX, const float & upY, const float & upZ)
{
	DirectX::XMVECTOR eye = { eyeX, eyeY, eyeZ };
	DirectX::XMVECTOR tar = { targetX, targetY, targetZ };
	DirectX::XMVECTOR upr = { upX, upY, upZ };
	DirectX::XMStoreFloat4x4(&wvp.view, DirectX::XMMatrixLookAtLH(eye, tar, upr));

	//行列データ更新
	memcpy(data, &wvp, sizeof(WVP));
}

// WVPの更新
void Constant::UpDataWvp(const float & angle)
{
	//行列更新
	DirectX::XMStoreFloat4x4(&wvp.world, DirectX::XMMatrixRotationY(RAD(angle)));

	//行列データ更新
	memcpy(data, &wvp, sizeof(WVP));
}

// 定数バッファのセット
void Constant::SetConstant(const unsigned int & index)
{
	//ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap);

	//ルートシグネチャとの関連づけ
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(index, heap->GetGPUDescriptorHandleForHeapStart());
}
