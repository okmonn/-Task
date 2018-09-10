#include "Constant.h"
#include "../../Window/Window.h"
#include "../../Device/Device.h"
#include "../../Command/List/List.h"
#include <tchar.h>

#define RESOURCE_MAX 1

// コンストラクタ
Constant::Constant(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list) : 
	win(win)
{
	this->dev  = dev;
	this->list = list;

	resource.clear();
	data.clear();
	wvp = {};
	
	SetWVP();
	Create();
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
	//カメラの位置
	DirectX::XMVECTOR eye    = { 0, pos,  -1 };
	//カメラの焦点
	DirectX::XMVECTOR target = { 0, pos,   0 };
	//カメラの上方向
	DirectX::XMVECTOR upper  = { 0, 1,     0 };

	wvp.view = DirectX::XMMatrixLookAtLH(eye, target, upper);

	wvp.projection = DirectX::XMMatrixPerspectiveFovLH(RAD(90.0f), ((static_cast<FLOAT>(win.lock()->GetX()) / static_cast<FLOAT>(win.lock()->GetY()))), 0.5f, 500.0f);

	//更新
	wvp.world  = DirectX::XMMatrixIdentity();

	wvp.window = { static_cast<FLOAT>(this->win.lock()->GetX()), static_cast<FLOAT>(this->win.lock()->GetY()) };
}

// WVPの変更
void Constant::ChangeView(const Vec3f & pos, const Vec3f & target, const Vec3f & up)
{
	DirectX::XMVECTOR eye = { pos.x, pos.y, pos.z };
	DirectX::XMVECTOR tar = { target.x, target.y, target.z };
	DirectX::XMVECTOR upr = { up.x, up.y, up.z };
	wvp.view = DirectX::XMMatrixLookAtLH(eye, tar, upr);

	//行列データ更新
	memcpy(data[0], &wvp, sizeof(WVP));
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
	desc.Width            = ((size + 0xff) &~0xff);
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
HRESULT Constant::CreateView(UINT index, UINT size)
{
	//ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += this->size * index;

	//定数バッファビュー設定用構造体の設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.SizeInBytes    = ((size + 0xff) &~0xff);
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
	memcpy(data[index], &wvp, size);

	return result;
}

// 生成
void Constant::Create(void)
{
	resource.resize(RESOURCE_MAX);
	data.resize(RESOURCE_MAX);

	CreateHeap(RESOURCE_MAX, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	CreateResource(0, sizeof(WVP));
	CreateView(0, sizeof(WVP));
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
