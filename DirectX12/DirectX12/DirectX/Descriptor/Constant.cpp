#include "Constant.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Swap.h"
#include <tchar.h>

// コンストラクタ
Constant::Constant(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap) : win(win)
{
	this->dev = dev;
	this->list = list;
	this->swap = swap;
	wvp = {};
	data = nullptr;


	SetWorldViewProjection();
	CreateResource();
	CreateView();
}

// デストラクタ
Constant::~Constant()
{
	Liberation(true);
}

// WVPのセット
void Constant::SetWorldViewProjection(void)
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
	wvp.world = DirectX::XMMatrixIdentity();
	wvp.viewProjection = view * projection;
}

// リソースの生成
HRESULT Constant::CreateResource(void)
{
	result = CreateHeap(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	if (FAILED(result))
	{
		return result;
	}

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
	desc.Width            = (sizeof(WVP) + 0xff) &~0xff;
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resource.resize(sizeof(BYTE));

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource[0]));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n定数バッファのリソースの生成：失敗\n"));
	}

	return result;
}

// リソースビューの生成
HRESULT Constant::CreateView(void)
{
	//定数バッファビュー設定用構造体の設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = resource[0]->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(WVP) + 0xff) &~0xff;

	//定数バッファビュー生成
	dev.lock()->Get()->CreateConstantBufferView(&desc, GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//送信範囲
	D3D12_RANGE range = { 0, 0 };

	//マッピング
	result = resource[0]->Map(0, &range, (void**)(&data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n定数バッファのマップ：失敗\n"));
	}

	//コピー
	memcpy(data, &wvp, sizeof(DirectX::XMMATRIX));

	return result;
}

// WVPの更新
void Constant::UpDataWVP(void)
{
	static float angle = 0.0f;
	//回転
	angle++;
	//行列更新
	wvp.world = DirectX::XMMatrixRotationY(RAD(angle));

	//行列データ更新
	memcpy(data, &wvp, sizeof(WVP));
}

// 定数バッファのセット
void Constant::SetConstant(UINT index, UINT table)
{
	//定数バッファヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE handle = heap[index]->GetGPUDescriptorHandleForHeapStart();

	//定数バッファヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &heap[index]);

	//定数バッファディスクラプターテーブルのセット
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(table, handle);
}
