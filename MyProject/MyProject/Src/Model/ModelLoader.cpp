#include "ModelLoader.h"
#include "PmdData.h"
#include "../Device/Device.h"
#include "../Texture/Texture.h"
#include "../Func/Func.h"
#include "../etc/Release.h"
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

// �R���X�g���N�^
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

// �f�X�g���N�^
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

// �e�N�X�`���̓ǂݍ���
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
			//��Z�e�N�X�`��
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
				//��Z�e�N�X�`��
				if (func::CheckChar("sph", material[fileName]->at(n).texPath, 20) == true)
				{
					path = func::FindString(fileName, '/') + tmp;
					sph[fileName]->emplace(n, 0);
					hr = texture.lock()->Load(path, sph[fileName]->at(n));
				}
				//���Z�e�N�X�`��
				else if (func::CheckChar("spa", material[fileName]->at(n).texPath, 20) == true)
				{
					path = func::FindString(fileName, '/') + tmp;
					spa[fileName]->emplace(n, 0);
					hr = texture.lock()->Load(path, spa[fileName]->at(n));
				}
				//�ʏ�e�N�X�`��
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

// �g�D�[���e�N�X�`���̓ǂݍ���
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

// �q�[�v�̐���
long ModelLoader::CreateHeap(const std::string & fileName)
{
	//�q�[�v�ݒ�p�\����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = material[fileName]->size() + 1;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap[fileName]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMD�̒萔�o�b�t�@�p�q�[�v�̐����F���s\n"));
	}

	return hr;
}

// �萔���\�[�X�̐���
long ModelLoader::CreateConRsc(ID3D12Resource ** rsc, const int & w)
{
	//�v���p�e�B�ݒ�p�\����
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//���\�[�X�ݒ�p�\����
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
		OutputDebugString(_T("\nPMD�̒萔�o�b�t�@�p���\�[�X�̐����F���s\n"));
	}

	return hr;
}

// �C���f�b�N�X�̐���
long ModelLoader::CreateIndex(const std::string& fileName)
{
	//�v���p�e�B�ݒ�p�\���̂̐ݒ�
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//���\�[�X�ݒ�p�\���̂̐ݒ�
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

	//���\�[�X����
	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&i_rsc[fileName]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMD�̃C���f�b�N�X���\�[�X�̐����F���s\n"));
		return hr;
	}

	//���M�͈�
	D3D12_RANGE range = { 0,0 };

	//�}�b�s���O
	hr = i_rsc[fileName]->Map(0, &range, (void**)(&data[fileName]));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nPMD�̃C���f�b�N�X�}�b�s���O�F���s\n"));
		return hr;
	}

	//���_�f�[�^�̃R�s�[
	memcpy(data[fileName], index[fileName]->data(), sizeof(unsigned short) * index[fileName]->size());

	//�A���}�b�v
	i_rsc[fileName]->Unmap(0, nullptr);

	return hr;
}

// �ǂݍ���
long ModelLoader::Load(const std::string & fileName)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nPMD�̓ǂݍ��݁F���s\n"));
		return S_FALSE;
	}

	//�w�b�_�[�ǂݍ���
	pmd::Header header = {};
	fread(&header, sizeof(header), 1, file);

	unsigned int num = 0;

	//���_�̓ǂݍ���
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

	//���_�C���f�b�N�X�ǂݍ���
	fread(&num, sizeof(unsigned int), 1, file);
	index[fileName] = std::make_shared<std::vector<unsigned short>>(num);
	for (auto itr = index[fileName]->begin(); itr != index[fileName]->end(); ++itr)
	{
		fread(&(*itr), sizeof(unsigned short), 1, file);
	}

	//�}�e���A���ǂݍ���
	fread(&num, sizeof(unsigned int), 1, file);
	material[fileName] = std::make_shared<std::vector<pmd::Material>>(num);
	fread(material[fileName]->data(), sizeof(pmd::Material), material[fileName]->size(), file);

	//�{�[���ǂݍ���
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

	//IK�̓ǂݍ���
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

	//�X�L���̓ǂݍ���
	unsigned short skinNum = 0;
	fread(&skinNum, sizeof(skinNum), 1, file);
	//���R�̌����̂悤�ɂ�����΂��� 
	for (int i = 0; i < skinNum; ++i) {
		fseek(file, 20, SEEK_CUR);
		unsigned int vertNum = 0;
		fread(&vertNum, sizeof(vertNum), 1, file);
		fseek(file, 1, SEEK_CUR);
		fseek(file, 16 * vertNum, SEEK_CUR);
	}

	//�\���p�\�� 
	unsigned char skinDispNum = 0;
	fread(&skinDispNum, sizeof(skinDispNum), 1, file);
	fseek(file, skinDispNum * sizeof(unsigned short), SEEK_CUR);

	//�\���p�{�[���� 
	unsigned char boneDispNum = 0;
	fread(&boneDispNum, sizeof(boneDispNum), 1, file);
	fseek(file, 50 * boneDispNum, SEEK_CUR);

	//�\���{�[�����X�g 
	unsigned int dispBoneNum = 0;
	fread(&dispBoneNum, sizeof(dispBoneNum), 1, file);
	fseek(file, 3 * dispBoneNum, SEEK_CUR);

	//�p���Ή��t���O 
	unsigned char englishFlg = 0;
	fread(&englishFlg, sizeof(englishFlg), 1, file);
	if (englishFlg) {
		//���f����20�o�C�g+256�o�C�g�R�����g 
		fseek(file, 20 + 256, SEEK_CUR);
		//�{�[����20�o�C�g*�{�[���� 
		fseek(file, born[fileName]->size() * 20, SEEK_CUR);
		//(�\�-1)*20�o�C�g�B-1�Ȃ̂̓x�[�X�����Ԃ� 
		fseek(file, (skinNum - 1) * 20, SEEK_CUR);
		//�{�[����*50�o�C�g�B 
		fseek(file, boneDispNum * 50, SEEK_CUR);
	}

	//�g�D�[���p�X�̓ǂݍ���
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
