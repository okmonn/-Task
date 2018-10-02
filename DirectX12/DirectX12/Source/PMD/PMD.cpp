#include "PMD.h"
#include "../Device/Device.h"
#include "../Command/List/List.h"
#include "../RootSignature/Root.h"
#include "../PipeLine/Pipe.h"
#include "../Descriptor/ConstantBuffer/Constant.h"
#include "../Texture/Texture.h"
#include "../VMD/VMD.h"
#include "PmdData.h"
#include <algorithm>
#include <tchar.h>

// コンストラクタ
PMD::PMD(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, 
	std::weak_ptr<Constant>con, std::weak_ptr<Texture>tex, std::weak_ptr<VMD>vmd) :
	root(root), pipe(pipe), con(con), tex(tex), vmd(vmd), mat({})
{
	this->dev = dev;
	this->list = list;
	size = this->dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	model.clear();
}

// デストラクタ
PMD::~PMD()
{
	for (auto itr = model.begin(); itr != model.end(); ++itr)
	{
		if (itr->second.v.resource != nullptr)
		{
			itr->second.v.resource->Unmap(0, nullptr);
		}
		if (itr->second.cB.resource != nullptr)
		{
			itr->second.cB.resource->Unmap(0, nullptr);
		}
		if (itr->second.c.resource != nullptr)
		{
			itr->second.c.resource->Unmap(0, nullptr);
		}
		Release(itr->second.v.resource);
		Release(itr->second.i.resource);
		Release(itr->second.cB.resource);
		Release(itr->second.c.resource);
		Release(itr->second.heap);
	}
}

// テクスチャの読み込み
HRESULT PMD::LoadTex(UINT * index, const std::string & fileName)
{
	for (UINT i = 0; i < model[index].material.size(); ++i)
	{
		if (model[index].material[i].texPath[0] != '\0')
		{
			result = tex.lock()->LoadWIC(model[index].id[i], func::FileLink(fileName, model[index].material[i].texPath).c_str());
			if (FAILED(result))
			{
				OutputDebugString(_T("\nPMDテクスチャの読み込み：失敗\n"));
				return result;
			}
		}
	}

	return result;
}

// 定数バッファヒープの生成
HRESULT PMD::CreateHeap(UINT * index)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = model[index].material.size() + 1;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&model[index].heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMDの定数バッファ用ヒープの生成：失敗\n"));
	}

	return result;
}

// 定数バッファリソースの生成
HRESULT PMD::CreateResource(UINT * index)
{
	if (FAILED(CreateHeap(index)))
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
	desc.Width            = ((sizeof(Mat) + 0xff) &~ 0xff) * model[index].material.size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model[index].c.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMDの定数バッファ用リソースの生成：失敗\n"));
	}

	return result;
}

// 定数バッファビューの生成
HRESULT PMD::CreateConView(UINT * index)
{
	if (FAILED(CreateResource(index)))
	{
		return result;
	}

	//定数バッファビュー設定用構造体
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.SizeInBytes = ((sizeof(Mat) + 0xff) &~0xff);

	//GPUアドレス
	D3D12_GPU_VIRTUAL_ADDRESS address = model[index].c.resource->GetGPUVirtualAddress();
	//先頭ハンドル
	model[index].handle = model[index].heap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < model[index].material.size(); ++i)
	{
		desc.BufferLocation = address;

		dev.lock()->Get()->CreateConstantBufferView(&desc, model[index].handle);

		address += desc.SizeInBytes;
		model[index].handle.ptr += size;
	}

	//送信範囲
	D3D12_RANGE range = { 0, 0 };

	//マッピング
	result = model[index].c.resource->Map(0, &range, (void**)(&model[index].c.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMDの定数バッファのマッピング：失敗\n"));
		return result;
	}

	memcpy(model[index].c.data, &mat, sizeof(Mat));

	return result;
}

// ボーン用定数バッファリソースの生成
HRESULT PMD::CreateBornResource(UINT * index)
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
	desc.Width            = ((sizeof(DirectX::XMMATRIX) * model[index].matrix.size() + 0xff) &~0xff);
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model[index].cB.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMDの定数バッファ用リソースの生成：失敗\n"));
	}

	return result;
}

// ボーン用定数バッファビューの生成
HRESULT PMD::CreateBornConView(UINT * index)
{
	if (FAILED(CreateBornResource(index)))
	{
		return result;
	}

	//定数バッファビュー設定用構造体
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = model[index].cB.resource->GetGPUVirtualAddress();
	desc.SizeInBytes    = ((sizeof(DirectX::XMMATRIX) * model[index].matrix.size() + 0xff) &~0xff);

	dev.lock()->Get()->CreateConstantBufferView(&desc, model[index].handle);

	//送信範囲
	D3D12_RANGE range = { 0, 0 };

	//マッピング
	result = model[index].cB.resource->Map(0, &range, (void**)(&model[index].cB.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMDのボーン用定数バッファのマッピング：失敗\n"));
		return result;
	}

	//コピー
	memcpy(model[index].cB.data, model[index].matrix.data(), ((sizeof(DirectX::XMMATRIX) * model[index].matrix.size() + 0xff) &~0xff));

	return result;
}

// 頂点バッファの生成
HRESULT PMD::CreateVeretxBuffer(UINT * index)
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
	desc.Width            = sizeof(PmdVertex) * model[index].vertex.size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//リソース生成
	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model[index].v.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMDの頂点リソースの生成：失敗\n"));
		return result;
	}

	//送信範囲
	D3D12_RANGE range = { 0,0 };

	//マッピング
	result = model[index].v.resource->Map(0, &range, (void**)(&model[index].v.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMDの頂点マッピング：失敗\n"));
		return result;
	}

	//頂点データのコピー
	memcpy(model[index].v.data, model[index].vertex.data(), sizeof(PmdVertex) * model[index].vertex.size());

	//頂点バッファ設定用構造体の設定
	model[index].v.view.BufferLocation = model[index].v.resource->GetGPUVirtualAddress();
	model[index].v.view.SizeInBytes    = sizeof(PmdVertex) * model[index].vertex.size();
	model[index].v.view.StrideInBytes  = sizeof(PmdVertex);

	return result;
}

// 頂点インデックスの生成
HRESULT PMD::CreateIndexBuffer(UINT * index)
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
	desc.Width            = sizeof(USHORT) * model[index].index.size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//リソース生成
	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model[index].i.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMDのインデックスリソースの生成：失敗\n"));
		return result;
	}

	//送信範囲
	D3D12_RANGE range = { 0,0 };

	//マッピング
	result = model[index].i.resource->Map(0, &range, (void**)(&model[index].i.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMDのインデックスマッピング：失敗\n"));
		return result;
	}

	//頂点データのコピー
	memcpy(model[index].i.data, model[index].index.data(), sizeof(USHORT) * model[index].index.size());

	//アンマップ
	model[index].i.resource->Unmap(0, nullptr);

	//頂点バッファ設定用構造体の設定
	model[index].i.view.BufferLocation = model[index].i.resource->GetGPUVirtualAddress();
	model[index].i.view.SizeInBytes    = sizeof(USHORT) * model[index].index.size();
	model[index].i.view.Format         = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;

	return result;
}

// ボーンのセット
void PMD::SetBorn(UINT * index)
{
	//ボーン座標配列のメモリ確保
	model[index].pos.resize(model[index].born.size());

	//ボーンノード配列のメモリ確保
	model[index].node.resize(model[index].born.size());

	//ボーン行列配列のメモリ確保
	model[index].matrix.resize(model[index].born.size());

	//ボーン行列の全配列の初期化
	std::fill(model[index].matrix.begin(), model[index].matrix.end(), DirectX::XMMatrixIdentity());

	for (UINT i = 0; i < model[index].born.size(); ++i)
	{
		model[index].name[model[index].born[i].name] = i;

		if (model[index].born[i].cIndex != 0)
		{
			//先頭座標格納
			model[index].pos[i].head = model[index].born[i].pos;

			//末尾座標格納
			model[index].pos[i].tail = model[index].born[model[index].born[i].cIndex].pos;
		}

		if (model[index].born[i].pIndex != 0xffff)
		{
			//参照ボーン格納
			model[index].node[model[index].born[i].pIndex].push_back(i);
		}
	}
}

// PMD読み込み
HRESULT PMD::LoadPMD(UINT & index, const std::string & fileName)
{
	UINT* n = &index;

	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nPMDの読み込み：失敗\n"));
		return S_FALSE;
	}

	//ヘッダー読み込み
	fread(&model[n].header, sizeof(model[n].header), 1, file);

	//頂点データ読み込み
	model[n].vertex.resize(model[n].header.vertexNum);
	for (auto& i : model[n].vertex)
	{
		fread(&i.pos,    sizeof(i.pos),    1, file);
		fread(&i.normal, sizeof(i.normal), 1, file);
		fread(&i.uv,     sizeof(i.uv),     1, file);
		fread(&i.bornNo, sizeof(i.bornNo), 1, file);
		fread(&i.weight, sizeof(i.weight), 1, file);
		fread(&i.edge,   sizeof(i.edge),   1, file);
	}

	//頂点インデックス読み込み
	UINT num = 0;
	fread(&num, sizeof(num), 1, file);
	model[n].index.resize(num);
	for (auto& i : model[n].index)
	{
		fread(&i, sizeof(USHORT), 1, file);
	}

	//マテリアル読み込み
	fread(&num, sizeof(num), 1, file);
	model[n].material.resize(num);
	fread(model[n].material.data(), sizeof(Material), model[n].material.size(), file);

	//ボーン読み込み
	fread(&num, sizeof(USHORT), 1, file);
	model[n].born.resize(num);
	for (auto& i : model[n].born)
	{
		fread(&i.name,     sizeof(i.name),     1, file);
		fread(&i.pIndex,   sizeof(i.pIndex),   1, file);
		fread(&i.cIndex,   sizeof(i.cIndex),   1, file);
		fread(&i.type,     sizeof(i.type),     1, file);
		fread(&i.IKpIndex, sizeof(i.IKpIndex), 1, file);
		fread(&i.pos,      sizeof(i.pos),      1, file);
	}

	fclose(file);

	SetBorn(n);

	if (FAILED(LoadTex(n, func::FindFirstString(fileName, '/', 1, true))))
	{
		return result;
	}
	if (FAILED(CreateConView(n)))
	{
		return result;
	}
	if (FAILED(CreateBornConView(n)))
	{
		return result;
	}
	if (FAILED(CreateVeretxBuffer(n)))
	{
		return result;
	}
	if (FAILED(CreateIndexBuffer(n)))
	{
		return result;
	}

	return result;
}

// ボーンの回転
void PMD::RotateBorn(UINT& index, const std::string & name, const DirectX::XMMATRIX & matrix)
{
	if (model[&index].node.empty())
	{
		return;
	}
	
	//ダミー宣言
	DirectX::XMVECTOR head = DirectX::XMLoadFloat3(&model[&index].pos[model[&index].name[name]].head);
	DirectX::XMVECTOR tmp  = DirectX::XMVectorSet(-model[&index].pos[model[&index].name[name]].head.x, -model[&index].pos[model[&index].name[name]].head.y, -model[&index].pos[model[&index].name[name]].head.z, 1.0f);
	DirectX::XMVECTOR tail = DirectX::XMLoadFloat3(&model[&index].pos[model[&index].name[name]].tail);
	DirectX::XMMATRIX mt   = DirectX::XMMatrixTranslationFromVector(tmp);

	mt *= matrix;
	mt *= DirectX::XMMatrixTranslationFromVector(head);

	tail = DirectX::XMVector3Transform(tail, mt);

	DirectX::XMStoreFloat3(&model[&index].pos[model[&index].name[name]].tail, tail);

	model[&index].matrix[model[&index].name[name]] = mt;

	for (auto &child : model[&index].node[model[&index].name[name]])
	{
		RotateChildBorn(index, child, mt);
	}

	//ボーン行列の更新
	memcpy(model[&index].cB.data, model[&index].matrix.data(), ((sizeof(DirectX::XMMATRIX) * model[&index].matrix.size() + 0xff) &~0xff));
}

// ボーンの回転
void PMD::RotateBorn(UINT& index, UINT& motion)
{
	auto m = vmd.lock()->GetMotion(motion);

	for (auto& i : m)
	{
		//ダミー宣言
		DirectX::XMVECTOR head = DirectX::XMLoadFloat3(&model[&index].pos[model[&index].name[i.name]].head);
		DirectX::XMVECTOR tmp  = DirectX::XMVectorSet(-model[&index].pos[model[&index].name[i.name]].head.x, -model[&index].pos[model[&index].name[i.name]].head.y, -model[&index].pos[model[&index].name[i.name]].head.z, 1.0f);
		DirectX::XMVECTOR tail = DirectX::XMLoadFloat3(&model[&index].pos[model[&index].name[i.name]].tail);
		DirectX::XMMATRIX mt   = DirectX::XMMatrixTranslationFromVector(tmp);

		mt *= DirectX::XMMatrixRotationQuaternion(i.quaternion);
		mt *= DirectX::XMMatrixTranslationFromVector(head);

		tail = DirectX::XMVector3Transform(tail, mt);

		DirectX::XMStoreFloat3(&model[&index].pos[model[&index].name[i.name]].tail, tail);

		model[&index].matrix[model[&index].name[i.name]] = mt;

		for (auto &child : model[&index].node[model[&index].name[i.name]])
		{
			RotateChildBorn(index, child, mt);
		}
	}

	//ボーン行列の更新
	memcpy(model[&index].cB.data, model[&index].matrix.data(), ((sizeof(DirectX::XMMATRIX) * model[&index].matrix.size() + 0xff) &~0xff));
}

// 子ボーンの回転
void PMD::RotateChildBorn(UINT& index, USHORT id, const DirectX::XMMATRIX & matrix)
{
	if (model[&index].node.empty() || id == 0)
	{
		return;
	}

	DirectX::XMVECTOR head = DirectX::XMLoadFloat3(&model[&index].pos[id].head);
	head = DirectX::XMVector3Transform(head, matrix);
	DirectX::XMStoreFloat3(&model[&index].pos[id].head, head);

	DirectX::XMVECTOR tail = DirectX::XMLoadFloat3(&model[&index].pos[id].tail);
	tail = DirectX::XMVector3Transform(tail, matrix);
	DirectX::XMStoreFloat3(&model[&index].pos[id].tail, tail);

	model[&index].matrix[id] = matrix;

	for (auto& child : model[&index].node[id])
	{
		RotateChildBorn(index, child, matrix);
	}
}

// 描画
void PMD::Draw(UINT& index)
{
	list.lock()->SetRoot(root.lock()->Get());
	list.lock()->SetPipe(pipe.lock()->Get());

	con.lock()->SetConstant();

	UINT* n = &index;

	UINT offset = 0;

	//定数ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &model[&index].heap);

	//ヒープの先頭ハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE handle = model[n].heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += size * model[n].material.size();
	//ディスクリプターテーブルのセット
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(3, handle);

	list.lock()->GetList()->IASetVertexBuffers(0, 1, &model[&index].v.view);
	list.lock()->GetList()->IASetIndexBuffer(&model[&index].i.view);
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//送信用データ
	UINT8* data = model[n].c.data;

	//ヒープの先頭ハンドル
	handle = model[n].heap->GetGPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < model[n].material.size(); ++i)
	{
		mat.diffuse = model[n].material[i].diffuse;

		mat.flag = (model[n].material[i].texPath[0] != '\0');
		if (mat.flag == TRUE)
		{
			tex.lock()->SetDraw(model[n].id[i]);
		}
		else
		{
			tex.lock()->SetDraw(model[n].id.begin()->second);
		}

		//定数ヒープのセット
		list.lock()->GetList()->SetDescriptorHeaps(1, &model[&index].heap);
		//ディスクリプターテーブルのセット
		list.lock()->GetList()->SetGraphicsRootDescriptorTable(2, handle);

		//コピー
		memcpy(data, &mat, sizeof(Mat));

		//描画
		list.lock()->GetList()->DrawIndexedInstanced(model[n].material[i].indexNum, 1, offset, 0, 0);

		//ハンドル更新
		handle.ptr += size;

		//データ更新
		data = (UINT8*)(((sizeof(Mat) + 0xff) &~0xff) + (CHAR*)(data));

		//オフセット更新
		offset += model[n].material[i].indexNum;
	}
}

// PMDの消去
void PMD::Delete(UINT & index)
{
	if (model.find(&index) != model.end())
	{
		for (auto itr = model[&index].id.begin(); itr != model[&index].id.end(); ++itr)
		{
			tex.lock()->Delete(itr->second);
		}

		if (model[&index].v.resource != nullptr)
		{
			model[&index].v.resource->Unmap(0, nullptr);
		}
		if (model[&index].c.resource != nullptr)
		{
			model[&index].c.resource->Unmap(0, nullptr);
		}
		Release(model[&index].v.resource);
		Release(model[&index].i.resource);
		Release(model[&index].c.resource);
		Release(model[&index].heap);

		model.erase(model.find(&index));
	}
}

// 文字列の検索・先頭から抜き出し
std::string func::FindFirstString(const std::string & path, const CHAR & find, UINT offset, bool end)
{
	UINT pos = (end == false) ? path.find_first_of(find) : path.find_last_of(find);
	pos += offset;

	std::string tmp = path.substr(0, pos);

	return tmp;
}

// 文字列の検索・見つけた場所から抜き出し
std::string func::FindEndString(const std::string & path, const CHAR & find, UINT offset, bool end)
{
	UINT pos = (end == true) ? path.find_last_of(find) : path.find_first_of(find);
	pos += offset;

	std::string tmp = path.substr(pos, path.size());

	return tmp;
}

// フォルダー名とファイル名の連結
std::string func::FileLink(const std::string & path1, const std::string & path2)
{
	INT index1 = path1.rfind('/');
	INT index2 = path1.rfind('\\');
	INT index = max(index1, index2);

	std::string folder = path1.substr(0, index) + "/" + path2;

	return folder;
}
