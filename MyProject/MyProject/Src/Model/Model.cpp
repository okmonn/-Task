#include "Model.h"
#include "ModelLoader.h"
#include "../Func/Func.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Texture/Texture.h"
#include "../etc/Release.h"
#include <algorithm>

// コンストラクタ
Model::Model(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con,
	std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<Texture>tex) :
	dev(dev), list(list), con(con), root(root), pipe(pipe), tex(tex), loader(std::make_unique<ModelLoader>(dev))
{
	pmd.clear();
}

// デストラクタ
Model::~Model()
{
	for (auto itr = pmd.begin(); itr != pmd.end(); ++itr)
	{
		UnMap(itr->second.v_rsc);
		Release(itr->second.v_rsc);
	}

	loader.reset();
}

// テクスチャの読み込み
long Model::LoadTexture(const std::string& fileName, int * i)
{
	auto hr = S_OK;
	std::string path = "";
	for (unsigned int n = 0; n < pmd[i].material.lock()->size(); ++n)
	{
		if (pmd[i].material.lock()->at(n).texPath[0] != '\0')
		{
			std::string tmp = (char*)pmd[i].material.lock()->at(n).texPath;
			if (func::CheckChar("a3.spa", pmd[i].material.lock()->at(n).texPath, 20) == true)
			{
				continue;
			}
			//乗算テクスチャ
			if (func::CheckChar("*", pmd[i].material.lock()->at(n).texPath, 20) == true)
			{
				auto find = tmp.find_first_of('*');

				path = fileName + tmp.substr(0, find);
				hr = tex.lock()->Load(path, pmd[i].tex[n]);

				path = fileName + tmp.substr(find + 1, tmp.size());
				hr = tex.lock()->Load(path, pmd[i].sph[n]);
			}
			else
			{
				//乗算テクスチャ
				if (func::CheckChar("sph", pmd[i].material.lock()->at(n).texPath, 20) == true)
				{
					path = fileName + tmp;
					hr = tex.lock()->Load(path, pmd[i].sph[n]);
				}
				//加算テクスチャ
				else if (func::CheckChar("spa", pmd[i].material.lock()->at(n).texPath, 20) == true)
				{
					path = fileName + tmp;
					hr = tex.lock()->Load(path, pmd[i].spa[n]);
				}
				//通常テクスチャ
				else
				{
					path = fileName + tmp;
					hr = tex.lock()->Load(path, pmd[i].tex[n]);
				}
			}
		}
	}

	return hr;
}

// マテリアル用シェーダビューの生成
long Model::CreateMaterialView(int * i)
{
	//定数バッファビュー設定用構造体
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.SizeInBytes = ((sizeof(pmd::Mat) + 0xff) &~0xff);

	//GPUアドレス
	D3D12_GPU_VIRTUAL_ADDRESS address = pmd[i].c_rsc->GetGPUVirtualAddress();
	//先頭ハンドル
	auto handle = pmd[i].heap->GetCPUDescriptorHandleForHeapStart();

	for (unsigned int n = 0; n < pmd[i].material.lock()->size(); ++n)
	{
		desc.BufferLocation = address;

		dev.lock()->Get()->CreateConstantBufferView(&desc, handle);

		address += desc.SizeInBytes;
		handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	//マッピング
	auto hr = pmd[i].c_rsc->Map(0, nullptr, (void**)(&pmd[i].c_data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nマテリアルのマップ：失敗\n"));
		return hr;
	}

	memcpy(pmd[i].c_data, &pmd[i].mat, sizeof(pmd::Mat));

	return hr;
}

// ボーンノードのセット
void Model::SetNode(int * i)
{
	//ボーン行列の初期化
	pmd[i].bornMtx.resize(pmd[i].born.lock()->size());
	std::fill(pmd[i].bornMtx.begin(), pmd[i].bornMtx.end(), DirectX::XMMatrixIdentity());

	for (unsigned int n = 0; n < pmd[i].born.lock()->size(); ++n)
	{
		pmd[i].node[pmd[i].born.lock()->at(n).name].index = n;
		pmd[i].node[pmd[i].born.lock()->at(n).name].start = pmd[i].born.lock()->at(n).pos;
	}

	for (auto& n : pmd[i].node)
	{
		if (pmd[i].born.lock()->at(n.second.index).pIndex >= pmd[i].born.lock()->size())
		{
			continue;
		}

		auto name = pmd[i].born.lock()->at(pmd[i].born.lock()->at(n.second.index).pIndex).name;

		pmd[i].node[name].child.push_back(&n.second);
	}

	int n = 0;
}

// ボーン用シェーダビューの生成
long Model::CreateBornView(int * i)
{
	SetNode(i);

	//定数バッファビュー設定用構造体
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = pmd[i].b_rsc->GetGPUVirtualAddress();
	desc.SizeInBytes    = ((sizeof(DirectX::XMMATRIX) * pmd[i].born.lock()->size() + 0xff) &~0xff);

	auto handle = pmd[i].heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * pmd[i].material.lock()->size();
	dev.lock()->Get()->CreateConstantBufferView(&desc, handle);

	//マッピング
	auto hr = pmd[i].b_rsc->Map(0, nullptr, (void**)(&pmd[i].b_data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nボーンのマップ：失敗\n"));
		return hr;
	}

	//コピー
	memcpy(pmd[i].b_data, pmd[i].bornMtx.data(), ((sizeof(DirectX::XMMATRIX) * pmd[i].born.lock()->size() + 0xff) &~0xff));

	return hr;
}

// 頂点バッファの生成
long Model::CreateVertex(int * i)
{
	//プロパティ設定用構造体
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//リソース設定用構造体
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width            = sizeof(pmd::Vertex) * pmd[i].vertex.lock()->size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//リソース生成
	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pmd[i].v_rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMDの頂点リソースの生成：失敗\n"));
		return hr;
	}

	//マッピング
	hr = pmd[i].v_rsc->Map(0, nullptr, (void**)(&pmd[i].v_data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMDの頂点マップ：失敗\n"));
		return hr;
	}

	//頂点データのコピー
	memcpy(pmd[i].v_data, pmd[i].vertex.lock()->data(), sizeof(pmd::Vertex) * pmd[i].vertex.lock()->size());

	return hr;
}

// 読み込み
long Model::Load(const std::string & fileName, int & i)
{
	if (FAILED(loader->Load(fileName)))
	{
		return S_FALSE;
	}

	pmd[&i].vertex   = loader->GetVertex(fileName);
	pmd[&i].index    = loader->GetIndex(fileName);
	pmd[&i].material = loader->GetMaterial(fileName);
	pmd[&i].born     = loader->GetBorn(fileName);
	pmd[&i].heap     = loader->GetHeap(fileName);
	pmd[&i].c_rsc    = loader->GetMaterialRsc(fileName);
	pmd[&i].b_rsc    = loader->GetBornRsc(fileName);
	pmd[&i].i_rsc    = loader->GetIndexRsc(fileName);

	auto hr = LoadTexture(func::FindString(fileName, '/'), &i);
	hr = CreateMaterialView(&i);
	hr = CreateBornView(&i);
	hr = CreateVertex(&i);

	return hr;
}

// 描画
void Model::Draw(int & i)
{
	list.lock()->SetRoot(*root.lock()->Get());
	list.lock()->SetPipe(*pipe.lock()->Get());

	con.lock()->SetConstant();

	int* n = &i;

	UINT offset = 0;

	//定数ヒープのセット
	list.lock()->GetList()->SetDescriptorHeaps(1, &pmd[n].heap);

	//ヒープの先頭ハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE handle = pmd[n].heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * pmd[n].material.lock()->size();
	//ディスクリプターテーブルのセット
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(3, handle);

	//頂点バッファビューのセット
	D3D12_VERTEX_BUFFER_VIEW vView = {};
	vView.BufferLocation = pmd[n].v_rsc->GetGPUVirtualAddress();
	vView.SizeInBytes    = sizeof(pmd::Vertex) * pmd[n].vertex.lock()->size();
	vView.StrideInBytes  = sizeof(pmd::Vertex);
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &vView);

	//頂点インデックスビューのセット
	D3D12_INDEX_BUFFER_VIEW iView = {};
	iView.BufferLocation = pmd[n].i_rsc->GetGPUVirtualAddress();
	iView.Format         = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;;
	iView.SizeInBytes    = sizeof(USHORT) * pmd[n].index.lock()->size();
	list.lock()->GetList()->IASetIndexBuffer(&iView);

	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//送信用データ
	UINT8* data = pmd[n].c_data;

	//ヒープの先頭ハンドル
	handle = pmd[n].heap->GetGPUDescriptorHandleForHeapStart();

	for (unsigned int i = 0; i < pmd[n].material.lock()->size(); ++i)
	{
		pmd[n].mat = {};

		//マテリアル構造体に格納
		pmd[n].mat.diffuse     = pmd[n].material.lock()->at(i).diffuse;
		pmd[n].mat.alpha       = pmd[n].material.lock()->at(i).alpha;
		pmd[n].mat.specularity = pmd[n].material.lock()->at(i).specularity;
		pmd[n].mat.specula     = pmd[n].material.lock()->at(i).specula;
		pmd[n].mat.mirror      = pmd[n].material.lock()->at(i).mirror;

		//sph
		if (pmd[n].sph.find(i) != pmd[n].sph.end())
		{
			pmd[n].mat.sphFlag = TRUE;
			tex.lock()->SetDraw(pmd[n].sph[i], 4);
		}
		else
		{
			tex.lock()->SetWhite();
		}

		//spa
		if (pmd[n].spa.find(i) != pmd[n].spa.end())
		{
			pmd[n].mat.spaFlag = TRUE;
			tex.lock()->SetDraw(pmd[n].spa[i], 5);
		}
		else
		{
			tex.lock()->SetBlack();
		}

		//通常テクスチャ
		if (pmd[n].tex.find(i) != pmd[n].tex.end())
		{
			pmd[n].mat.texFlag = TRUE;
			tex.lock()->SetDraw(pmd[n].tex[i]);
		}
		else
		{
			tex.lock()->SetDraw(pmd[n].tex.begin()->second);
		}

		//tex.lock()->SetDraw(pmd[n].toon[model[n].material[i].toonIndex], 6);
		tex.lock()->SetGrade();

		//(mat.flag == TRUE) ? tex.lock()->SetDraw(model[n].tex[i]) : tex.lock()->SetDraw(model[n].tex.begin()->second);

		//定数ヒープのセット
		list.lock()->GetList()->SetDescriptorHeaps(1, &pmd[n].heap);
		//ディスクリプターテーブルのセット
		list.lock()->GetList()->SetGraphicsRootDescriptorTable(2, handle);

		//コピー
		memcpy(data, &pmd[n].mat, sizeof(pmd::Mat));

		//描画
		list.lock()->GetList()->DrawIndexedInstanced(pmd[n].material.lock()->at(i).indexNum, 1, offset, 0, 0);

		//ハンドル更新
		handle.ptr += dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//データ更新
		data = (UINT8*)(((sizeof(pmd::Mat) + 0xff) &~0xff) + (CHAR*)(data));

		//オフセット更新
		offset += pmd[n].material.lock()->at(i).indexNum;
	}
}
