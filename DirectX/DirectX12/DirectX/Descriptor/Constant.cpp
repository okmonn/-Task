#include "Constant.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Command/List.h"
#include <tchar.h>

#define RESOURCE_MAX 2

// コンストラクタ
Constant::Constant(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list) : 
	win(win)
{
	this->dev  = dev;
	this->list = list;

	resource.clear();
	resource.resize(RESOURCE_MAX);
	data.clear();
	data.resize(RESOURCE_MAX);
	wvp = {};
	window = {static_cast<FLOAT>(this->win.lock()->GetX()), static_cast<FLOAT>(this->win.lock()->GetY())};
	
	SetWVP();
	CreateHeap();
	CreateResource(0, ((sizeof(WVP) + 0xff) &~0xff));
	CreateResource(1, ((sizeof(DirectX::XMFLOAT2) + 0xff) &~0xff));
	CreateView(0, (sizeof(WVP) + 0xff) &~0xff, sizeof(WVP));
	CreateView(1, (sizeof(DirectX::XMFLOAT2) + 0xff) &~0xff, sizeof(DirectX::XMFLOAT2));
}

// デストラクタ
Constant::~Constant()
{
	for (UINT i = 0; i < resource.size(); ++i)
	{
		if (resource[i] != nullptr)
		{
			resource[i]->Unmap(0, nullptr);
		}

		Release(resource[i]);
	}
	Release(heap);
}

// WVPのセット
void Constant::SetWVP(void)
{
	//ダミー宣言
	FLOAT pos = 0.0f;
	DirectX::XMMATRIX view   = DirectX::XMMatrixIdentity();
	//カメラの位置
	DirectX::XMVECTOR eye    = { 0, pos,  -1 };
	//カメラの焦点
	DirectX::XMVECTOR target = { 0, pos,   0 };
	//カメラの上方向
	DirectX::XMVECTOR upper  = { 0, 1,     0 };

	view = DirectX::XMMatrixLookAtLH(eye, target, upper);

	//ダミー宣言
	DirectX::XMMATRIX projection = DirectX::XMMatrixIdentity();

	projection = DirectX::XMMatrixPerspectiveFovLH(RAD(90.0f), ((static_cast<FLOAT>(win.lock()->GetX()) / static_cast<FLOAT>(win.lock()->GetY()))), 0.5f, 500.0f);

	//更新
	wvp.world          = DirectX::XMMatrixIdentity();
	wvp.viewProjection = view * projection;
}

// ヒープの生成
HRESULT Constant::CreateHeap(void)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = RESOURCE_MAX;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//ヒープ生成
	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nヒープの生成：失敗\n"));
		return result;
	}

	size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return result;
}

// リソースの生成
HRESULT Constant::CreateResource(UINT index, UINT64 size)
{
	//プロパティ設定用構造体の設定
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//リソース設定用構造体の設定
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width            = size;
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource[index]));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n定数バッファのリソースの生成：失敗\n"));
	}

	return result;
}

// リソースビューの生成
HRESULT Constant::CreateView(UINT index, UINT size, UINT stride)
{
	//ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += this->size * index;

	//定数バッファビュー設定用構造体の設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.SizeInBytes    = size;
	desc.BufferLocation = resource[index]->GetGPUVirtualAddress();

	//送信範囲
	D3D12_RANGE range = { 0, 0 };

	//定数バッファビュー生成
	dev.lock()->Get()->CreateConstantBufferView(&desc, handle);

	//マッピング
	result = resource[index]->Map(0, &range, (void**)(&data[index]));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n定数バッファのリソースマッピング：失敗\n"));
		return result;
	}

	//コピー
	memcpy(data[index], &wvp, stride);

	return result;
}

// WVPの更新
void Constant::UpDataWVP(void)
{
	//回転
	static float angle = 0.0f;
	//行列更新
	wvp.world = DirectX::XMMatrixRotationY(RAD(angle));

	//行列データ更新
	memcpy(data[0], &wvp, sizeof(WVP));

	angle++;
}

// Imageの更新
void Constant::UpDataWindow(void)
{
	memcpy(data[1], &window, sizeof(DirectX::XMFLOAT2));
}

// 定数バッファのセット
void Constant::SetConstant(UINT rootIndex, UINT index)
{
	// ハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += size * index;

	//ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap);

	//ルートシグネチャとの関連づけ
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(rootIndex, handle);
}
