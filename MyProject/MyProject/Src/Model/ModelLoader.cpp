#include "ModelLoader.h"
#include "PmdData.h"
#include "../Device/Device.h"
#include "../Texture/Texture.h"
#include "../Func/Func.h"
#include "../etc/Release.h"
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

// コンストラクタ
ModelLoader::ModelLoader(std::weak_ptr<Device>dev, std::weak_ptr<Texture>texture) :
	dev(dev), texture(texture)
{
	vertex.clear();
	index.clear();
	material.clear();
	born.clear();
	ikBorn.clear();
	heap.clear();
	c_rsc.clear();
	b_rsc.clear();
	i_rsc.clear();
	sph.clear();
	spa.clear();
	tex.clear();
	toon.clear();
	data.clear();
}

// デストラクタ
ModelLoader::~ModelLoader()
{
	for (auto itr = i_rsc.begin(); itr != i_rsc.end(); ++itr)
	{
		Release(itr->second);
	}
	for (auto itr = b_rsc.begin(); itr != b_rsc.end(); ++itr)
	{
		UnMap(itr->second);
		Release(itr->second);
	}
	for (auto itr = c_rsc.begin(); itr != c_rsc.end(); ++itr)
	{
		UnMap(itr->second);
		Release(itr->second);
	}
	for (auto itr = heap.begin(); itr != heap.end(); ++itr)
	{
		Release(itr->second);
	}
}

// テクスチャの読み込み
long ModelLoader::LoadTex(const std::string & fileName)
{
	auto hr = S_OK;
	std::string path = "";

	sph[fileName] = std::make_shared<std::map<int, int>>();
	spa[fileName] = std::make_shared<std::map<int, int>>();
	tex[fileName] = std::make_shared<std::map<int, int>>();

	for (unsigned int n = 0; n < material[fileName]->size(); ++n)
	{
		if (material[fileName]->at(n).texPath[0] != '\0')
		{
			std::string tmp = (char*)material[fileName]->at(n).texPath;
			if (func::CheckChar("a3.spa", material[fileName]->at(n).texPath, 20) == true)
			{
				continue;
			}
			//乗算テクスチャ
			if (func::CheckChar("*", material[fileName]->at(n).texPath, 20) == true)
			{
				auto find = tmp.find_first_of('*');

				/*path = func::FindString(fileName, '/') + tmp.substr(0, find);
				tex[fileName]->emplace(n, 0);
				hr = texture.lock()->Load(path, tex[fileName]->at(n));*/

				path = func::FindString(fileName, '/') + tmp.substr(find + 1, tmp.size());
				sph[fileName]->emplace(n, 0);
				hr = texture.lock()->Load(path, sph[fileName]->at(n));
			}
			else
			{
				//乗算テクスチャ
				if (func::CheckChar("sph", material[fileName]->at(n).texPath, 20) == true)
				{
					path = func::FindString(fileName, '/') + tmp;
					sph[fileName]->emplace(n, 0);
					hr = texture.lock()->Load(path, sph[fileName]->at(n));
				}
				//加算テクスチャ
				else if (func::CheckChar("spa", material[fileName]->at(n).texPath, 20) == true)
				{
					path = func::FindString(fileName, '/') + tmp;
					spa[fileName]->emplace(n, 0);
					hr = texture.lock()->Load(path, spa[fileName]->at(n));
				}
				//通常テクスチャ
				else
				{
					path = func::FindString(fileName, '/') + tmp;
					tex[fileName]->emplace(n, 0);
					hr = texture.lock()->Load(path, tex[fileName]->at(n));
				}
			}
		}
	}

	return hr;
}

// トゥーンテクスチャの読み込み
long ModelLoader::LoadToon(const std::string & fileName)
{
	auto hr = S_OK;

	toon[fileName] = std::make_shared<std::map<int, int>>();

	for (unsigned int i = 0; i < material[fileName]->size(); ++i)
	{
		int tmp = (material[fileName]->at(i).toonIndex >= 0xff) ? 0 : material[fileName]->at(i).toonIndex;

		std::string path = func::FindString(fileName, '/') + "toon/" + toonPath[tmp];
		if (toon[fileName]->find(material[fileName]->at(i).toonIndex) == toon[fileName]->end())
		{
			toon[fileName]->emplace(material[fileName]->at(i).toonIndex, 0);
			hr = texture.lock()->Load(path, toon[fileName]->at(material[fileName]->at(i).toonIndex));
			if (FAILED(hr))
			{
				std::string path2 = fileName + "/" + toonPath[tmp];
				if (PathFileExistsA(path2.c_str()))
				{
					hr = texture.lock()->Load(fileName + "/", toon[fileName]->at(material[fileName]->at(i).toonIndex));
				}
			}
		}
	}

	return hr;
}

// ヒープの生成
long ModelLoader::CreateHeap(const std::string & fileName)
{
	//ヒープ設定用構造体
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = material[fileName]->size() + 1;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap[fileName]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMDの定数バッファ用ヒープの生成：失敗\n"));
	}

	return hr;
}

// 定数リソースの生成
long ModelLoader::CreateConRsc(ID3D12Resource ** rsc, const int & w)
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
	desc.Width            = w;
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&(*rsc)));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMDの定数バッファ用リソースの生成：失敗\n"));
	}

	return hr;
}

// インデックスの生成
long ModelLoader::CreateIndex(const std::string& fileName)
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
	desc.Width            = sizeof(unsigned short) * index[fileName]->size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//リソース生成
	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&i_rsc[fileName]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMDのインデックスリソースの生成：失敗\n"));
		return hr;
	}

	//送信範囲
	D3D12_RANGE range = { 0,0 };

	//マッピング
	hr = i_rsc[fileName]->Map(0, &range, (void**)(&data[fileName]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMDのインデックスマッピング：失敗\n"));
		return hr;
	}

	//頂点データのコピー
	memcpy(data[fileName], index[fileName]->data(), sizeof(unsigned short) * index[fileName]->size());

	//アンマップ
	i_rsc[fileName]->Unmap(0, nullptr);

	return hr;
}

// 読み込み
long ModelLoader::Load(const std::string & fileName)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nPMDの読み込み：失敗\n"));
		return S_FALSE;
	}

	//ヘッダー読み込み
	pmd::Header header = {};
	fread(&header, sizeof(header), 1, file);

	unsigned int num = 0;

	//頂点の読み込み
	fread(&num, sizeof(unsigned int), 1, file);
	vertex[fileName] = std::make_shared<std::vector<pmd::Vertex>>(num);
	for (auto itr = vertex[fileName]->begin(); itr != vertex[fileName]->end(); ++itr)
	{
		fread(&itr->pos,    sizeof(itr->pos),    1, file);
		fread(&itr->normal, sizeof(itr->normal), 1, file);
		fread(&itr->uv,     sizeof(itr->uv),     1, file);
		fread(&itr->bornNo, sizeof(itr->bornNo), 1, file);
		fread(&itr->weight, sizeof(itr->weight), 1, file);
		fread(&itr->edge,   sizeof(itr->edge),   1, file);
	}

	//頂点インデックス読み込み
	fread(&num, sizeof(unsigned int), 1, file);
	index[fileName] = std::make_shared<std::vector<unsigned short>>(num);
	for (auto itr = index[fileName]->begin(); itr != index[fileName]->end(); ++itr)
	{
		fread(&(*itr), sizeof(unsigned short), 1, file);
	}

	//マテリアル読み込み
	fread(&num, sizeof(unsigned int), 1, file);
	material[fileName] = std::make_shared<std::vector<pmd::Material>>(num);
	fread(material[fileName]->data(), sizeof(pmd::Material), material[fileName]->size(), file);

	//ボーン読み込み
	fread(&num, sizeof(unsigned short), 1, file);
	born[fileName] = std::make_shared<std::vector<pmd::Born>>(num);
	for (auto itr = born[fileName]->begin(); itr != born[fileName]->end(); ++itr)
	{
		fread(&itr->name,     sizeof(itr->name),     1, file);
		fread(&itr->pIndex,   sizeof(itr->pIndex),   1, file);
		fread(&itr->cIndex,   sizeof(itr->cIndex),   1, file);
		fread(&itr->type,     sizeof(itr->type),     1, file);
		fread(&itr->IKpIndex, sizeof(itr->IKpIndex), 1, file);
		fread(&itr->pos,      sizeof(itr->pos),      1, file);
	}

	//IKの読み込み
	fread(&num, sizeof(unsigned short), 1, file);
	pmd::IkBorn ik = {};
	ikBorn[fileName] = std::make_shared<std::map<std::string, pmd::IkBorn>>();
	for (unsigned int i = 0; i < num; ++i)
	{
		fread(&ik.index,     sizeof(ik.index),                         1, file);
		fread(&ik.target,    sizeof(ik.target),                        1, file);
		fread(&ik.chain,     sizeof(ik.chain),                         1, file);
		ik.child.resize(ik.chain);
		fread(&ik.iteration, sizeof(ik.iteration),                     1, file);
		fread(&ik.weight,    sizeof(ik.weight),                        1, file);
		fread(&ik.child[0],  sizeof(unsigned short) * ik.child.size(), 1, file);

		ikBorn[fileName]->insert(std::make_pair(born[fileName]->at(ik.index).name, ik));
	}

	//スキンの読み込み
	unsigned short skinNum = 0;
	fread(&skinNum, sizeof(skinNum), 1, file);
	//当然の権利のようにすっ飛ばすで 
	for (int i = 0; i < skinNum; ++i) {
		fseek(file, 20, SEEK_CUR);
		unsigned int vertNum = 0;
		fread(&vertNum, sizeof(vertNum), 1, file);
		fseek(file, 1, SEEK_CUR);
		fseek(file, 16 * vertNum, SEEK_CUR);
	}

	//表示用表情 
	unsigned char skinDispNum = 0;
	fread(&skinDispNum, sizeof(skinDispNum), 1, file);
	fseek(file, skinDispNum * sizeof(unsigned short), SEEK_CUR);

	//表示用ボーン名 
	unsigned char boneDispNum = 0;
	fread(&boneDispNum, sizeof(boneDispNum), 1, file);
	fseek(file, 50 * boneDispNum, SEEK_CUR);

	//表示ボーンリスト 
	unsigned int dispBoneNum = 0;
	fread(&dispBoneNum, sizeof(dispBoneNum), 1, file);
	fseek(file, 3 * dispBoneNum, SEEK_CUR);

	//英名対応フラグ 
	unsigned char englishFlg = 0;
	fread(&englishFlg, sizeof(englishFlg), 1, file);
	if (englishFlg) {
		//モデル名20バイト+256バイトコメント 
		fseek(file, 20 + 256, SEEK_CUR);
		//ボーン名20バイト*ボーン数 
		fseek(file, born[fileName]->size() * 20, SEEK_CUR);
		//(表情数-1)*20バイト。-1なのはベース部分ぶん 
		fseek(file, (skinNum - 1) * 20, SEEK_CUR);
		//ボーン数*50バイト。 
		fseek(file, boneDispNum * 50, SEEK_CUR);
	}

	//トゥーンパスの読み込み
	fread(toonPath.data(), sizeof(char) * 100, toonPath.size(), file);

	fclose(file);

	auto hr = LoadTex(fileName);
	hr = LoadToon(fileName);
	hr = CreateHeap(fileName);
	hr = CreateConRsc(&c_rsc[fileName], ((sizeof(pmd::Mat) + 0xff) &~0xff) * material[fileName]->size());
	hr = CreateConRsc(&b_rsc[fileName], ((sizeof(DirectX::XMMATRIX) * born[fileName]->size() + 0xff) &~0xff));
	hr = CreateIndex(fileName);

	return 0;
}
