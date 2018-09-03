#include "PMD.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Pipe.h"
#include "../Texture/Texture.h"
#include "PmdData.h"
#include <algorithm>
#include <tchar.h>

UINT n = 0;

// �R���X�g���N�^
PMD::PMD(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Pipe>pipe, std::weak_ptr<Texture>tex) :
	pipe(pipe), tex(tex), mat({})
{
	this->dev = dev;
	this->list = list;
	size = this->dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	model.clear();

	LoadPMD(n, "Model/�����~�N.pmd");
}

// �f�X�g���N�^
PMD::~PMD()
{
	for (auto itr = model.begin(); itr != model.end(); ++itr)
	{
		if (itr->second.v.resource != nullptr)
		{
			itr->second.v.resource->Unmap(0, nullptr);
		}
		if (itr->second.c.resource != nullptr)
		{
			itr->second.c.resource->Unmap(0, nullptr);
		}
		Release(itr->second.v.resource);
		Release(itr->second.i.resource);
		Release(itr->second.c.resource);
		Release(itr->second.c.heap);
	}
}

// �e�N�X�`���̓ǂݍ���
HRESULT PMD::LoadTex(UINT * index, const std::string & fileName)
{
	for (UINT i = 0; i < model[index].material.size(); ++i)
	{
		if (model[index].material[i].texPath[0] != '\0')
		{
			result = tex.lock()->LoadWIC(model[index].id[i], func::FileLink(fileName, model[index].material[i].texPath).c_str());
			if (FAILED(result))
			{
				OutputDebugString(_T("\nPMD�e�N�X�`���̓ǂݍ��݁F���s\n"));
				return result;
			}
		}
	}

	return result;
}

// �萔�o�b�t�@�q�[�v�̐���
HRESULT PMD::CreateHeap(UINT * index)
{
	//�q�[�v�ݒ�p�\����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = model[index].material.size();
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&model[index].c.heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMD�̒萔�o�b�t�@�p�q�[�v�̐����F���s\n"));
	}

	return result;
}

// �萔�o�b�t�@���\�[�X�̐���
HRESULT PMD::CreateResource(UINT * index)
{
	if (FAILED(CreateHeap(index)))
	{
		return result;
	}


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
		OutputDebugString(_T("\nPMD�̒萔�o�b�t�@�p���\�[�X�̐����F���s\n"));
	}

	return result;
}

// �萔�o�b�t�@�r���[�̐���
HRESULT PMD::CreateConView(UINT * index)
{
	if (FAILED(CreateResource(index)))
	{
		return result;
	}

	//�萔�o�b�t�@�r���[�ݒ�p�\����
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.SizeInBytes = ((sizeof(Mat) + 0xff) &~0xff);

	//GPU�A�h���X
	D3D12_GPU_VIRTUAL_ADDRESS address = model[index].c.resource->GetGPUVirtualAddress();
	//�擪�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE handle = model[index].c.heap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < model[index].material.size(); ++i)
	{
		desc.BufferLocation = address;

		dev.lock()->Get()->CreateConstantBufferView(&desc, handle);

		address += desc.SizeInBytes;
		handle.ptr += size;
	}

	//���M�͈�
	D3D12_RANGE range = { 0, 0 };

	//�}�b�s���O
	result = model[index].c.resource->Map(0, &range, (void**)(&model[index].c.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMD�̒萔�o�b�t�@�̃}�b�s���O�F���s\n"));
		return result;
	}

	memcpy(model[index].c.data, &mat, sizeof(Mat));

	return result;
}

// ���_�o�b�t�@�̐���
HRESULT PMD::CreateVeretxBuffer(UINT * index)
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
	desc.Width            = sizeof(PmdVertex) * model[index].vertex.size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���\�[�X����
	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model[index].v.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMD�̒��_���\�[�X�̐����F���s\n"));
		return result;
	}

	//���M�͈�
	D3D12_RANGE range = { 0,0 };

	//�}�b�s���O
	result = model[index].v.resource->Map(0, &range, (void**)(&model[index].v.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMD�̒��_�}�b�s���O�F���s\n"));
		return result;
	}

	//���_�f�[�^�̃R�s�[
	memcpy(model[index].v.data, model[index].vertex.data(), sizeof(PmdVertex) * model[index].vertex.size());

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	model[index].v.view.BufferLocation = model[index].v.resource->GetGPUVirtualAddress();
	model[index].v.view.SizeInBytes    = sizeof(PmdVertex) * model[index].vertex.size();
	model[index].v.view.StrideInBytes  = sizeof(PmdVertex);

	return result;
}

// ���_�C���f�b�N�X�̐���
HRESULT PMD::CreateIndexBuffer(UINT * index)
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
	desc.Width            = sizeof(USHORT) * model[index].index.size();
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���\�[�X����
	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&model[index].i.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMD�̃C���f�b�N�X���\�[�X�̐����F���s\n"));
		return result;
	}

	//���M�͈�
	D3D12_RANGE range = { 0,0 };

	//�}�b�s���O
	result = model[index].i.resource->Map(0, &range, (void**)(&model[index].i.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nPMD�̃C���f�b�N�X�}�b�s���O�F���s\n"));
		return result;
	}

	//���_�f�[�^�̃R�s�[
	memcpy(model[index].i.data, model[index].index.data(), sizeof(USHORT) * model[index].index.size());

	//�A���}�b�v
	model[index].i.resource->Unmap(0, nullptr);

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	model[index].i.view.BufferLocation = model[index].i.resource->GetGPUVirtualAddress();
	model[index].i.view.SizeInBytes    = sizeof(USHORT) * model[index].index.size();
	model[index].i.view.Format         = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;

	return result;
}


// PMD�ǂݍ���
HRESULT PMD::LoadPMD(UINT & index, const std::string & fileName)
{
	UINT* n = &index;

	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nPMD�̓ǂݍ��݁F���s\n"));
		return S_FALSE;
	}

	//�w�b�_�[�ǂݍ���
	fread(&model[n].header, sizeof(model[n].header), 1, file);

	//���_�f�[�^�ǂݍ���
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

	//���_�C���f�b�N�X�ǂݍ���
	UINT num = 0;
	fread(&num, sizeof(num), 1, file);
	model[n].index.resize(num);
	for (auto& i : model[n].index)
	{
		fread(&i, sizeof(USHORT), 1, file);
	}

	//�}�e���A���ǂݍ���
	fread(&num, sizeof(num), 1, file);
	model[n].material.resize(num);
	fread(model[n].material.data(), sizeof(Material), model[n].material.size(), file);

	//�{�[���ǂݍ���
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

	model[n].id.resize(model[n].material.size());

	if (FAILED(LoadTex(n, func::FindFirstString(fileName, '/'))))
	{
		return result;
	}
	if (FAILED(CreateConView(n)))
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

// �`��
void PMD::Draw(void)
{
}

// ������̌����E�擪���甲���o��
std::string func::FindFirstString(const std::string & path, const CHAR & find, UINT offset, bool end)
{
	UINT pos = (end == false) ? path.find_first_of(find) : path.find_last_of(find);
	pos += offset;

	std::string tmp = path.substr(0, pos);

	return tmp;
}

// ������̌����E�������ꏊ���甲���o��
std::string func::FindEndString(const std::string & path, const CHAR & find, UINT offset, bool end)
{
	UINT pos = (end == true) ? path.find_last_of(find) : path.find_first_of(find);
	pos += offset;

	std::string tmp = path.substr(pos, path.size());

	return tmp;
}

// �t�H���_�[���ƃt�@�C�����̘A��
std::string func::FileLink(const std::string & path1, const std::string & path2)
{
	INT index1 = path1.rfind('/');
	INT index2 = path1.rfind('\\');
	INT index = max(index1, index2);

	std::string folder = path1.substr(0, index) + "/" + path2;

	return folder;
}
