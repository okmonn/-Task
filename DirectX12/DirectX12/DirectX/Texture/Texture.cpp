#include "Texture.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Swap.h"
#include "WICTextureLoader12.h"
#include <tchar.h>

// �R���X�g���N�^
Texture::Texture(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap) : win(win), dev(dev), list(list), swap(swap)
{
	wic.clear();
	CoInitialize(nullptr);
}

// �f�X�g���N�^
Texture::~Texture()
{
}

// �q�[�v�̐���
HRESULT Texture::CreateHeap(UINT* index)
{
	//�萔�o�b�t�@�ݒ�p�\���̂̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = swap.lock()->GetBack();
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&wic[index].constant.heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�e�N�X�`���p�̃q�[�v�����F���s\n"));
	}

	//�q�[�v�T�C�Y���擾
	wic[index].constant.size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(desc.Type);

	return result;
}

// ���\�[�X�̐���
HRESULT Texture::CreateResourcce(UINT * index)
{
	//�q�[�v�ݒ�p�\���̂̐ݒ�
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.VisibleNodeMask      = 1;

	//���\�[�X�ݒ�p�\���̂̐ݒ�
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width            = ((sizeof(Image) + 0xff) &~0xff);
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&wic[index].constant.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�e�N�X�`���p�̃��\�[�X�����F���s\n"));
	}

	return result;
}

// �o�b�t�@�r���[�̐���
HRESULT Texture::CreateView(UINT * index)
{
	//�萔�o�b�t�@�r���[�ݒ�p�\���̂̐ݒ�
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = wic[index].constant.resource->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(Image) + 0xff) &~0xff;

	//�萔�o�b�t�@�r���[����
	dev.lock()->Get()->CreateConstantBufferView(&desc, wic[index].constant.heap->GetCPUDescriptorHandleForHeapStart());

	//���M�͈�
	D3D12_RANGE range = { 0, 0 };

	//�}�b�s���O
	result = wic[index].constant.resource->Map(0, &range, (void**)(&wic[index].constant.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�e�N�X�`���p�̃}�b�v�F���s\n"));
	}

	//�R�s�[
	memcpy(wic[index].constant.data, &wic[index].image, sizeof(Image));

	return result;
}

// �V�F�[�_���\�[�X�r���[�̐���
HRESULT Texture::CreateShaderView(UINT * index)
{
	//�V�F�[�_���\�[�X�r���[�ݒ�p�\���̂̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = wic[index].resource->GetDesc().Format;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE handle = wic[index].constant.heap->GetCPUDescriptorHandleForHeapStart();

	//�V�F�[�_�[���\�[�X�r���[�̐���
	dev.lock()->Get()->CreateShaderResourceView(wic[index].resource, &desc, handle);

	return result;
}

// ���_���\�[�X�̐���
HRESULT Texture::CreateVertexResource(UINT * index)
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
	desc.Width            = sizeof(wic[index].vertex.vertex);//((sizeof(v[index].vertex) + 0xff) &~0xff);
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���\�[�X����
	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&wic[index].vertex.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�e�N�X�`���̒��_�o�b�t�@�p���\�[�X�����F���s\n"));
	}

	//���M�͈�
	D3D12_RANGE range = { 0,0 };

	//�}�b�s���O
	result = wic[index].vertex.resource->Map(0, &range, reinterpret_cast<void**>(&wic[index].vertex.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWIC���_�p�}�b�s���O�F���s\n"));
	}

	//���_�f�[�^�̃R�s�[
	memcpy(wic[index].vertex.data, &wic[index].vertex.vertex, sizeof(wic[index].vertex.vertex));

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	wic[index].vertex.view.BufferLocation = wic[index].vertex.resource->GetGPUVirtualAddress();
	wic[index].vertex.view.SizeInBytes    = sizeof(wic[index].vertex.vertex);
	wic[index].vertex.view.StrideInBytes  = sizeof(Vertex);

	return result;
}

// WIC�ǂݍ���
HRESULT Texture::LoadWIC(UINT & index, std::string fileName)
{
	UINT* n = &index;

	std::wstring path = okmonn::ChangeUnicode(fileName.c_str());

	result = DirectX::LoadWICTextureFromFile(dev.lock()->Get(), path.c_str(), &wic[n].resource, wic[n].decode, wic[n].sub);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWIC�e�N�X�`���̓ǂݍ��݁F���s\n"));
	}

	wic[n].image.window.x = static_cast<FLOAT>(win.lock()->GetX());
	wic[n].image.window.y = static_cast<FLOAT>(win.lock()->GetY());

	result = CreateHeap(n);
	result = CreateResourcce(n);
	result = CreateView(n);
	result = CreateShaderView(n);
	result = CreateVertexResource(n);

	return result;
}

// �q�[�v�̃Z�b�g
void Texture::SetHeap(UINT * index, UINT i)
{
	//�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE handle = wic[index].constant.heap->GetGPUDescriptorHandleForHeapStart();

	//�q�[�v�̃Z�b�g
	list.lock()->GetList()->SetDescriptorHeaps(1, &wic[index].constant.heap);

	//�f�B�X�N���v�^�[�e�[�u���̃Z�b�g
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(i, handle);
}

// �`�揀��
void Texture::SetDraw(UINT * index)
{
	//���\�[�X�ݒ�p�\����
	D3D12_RESOURCE_DESC desc = {};
	desc = wic[index].resource->GetDesc();

	//�{�b�N�X�ݒ�p�\���̂̐ݒ�
	D3D12_BOX box = {};
	box.back   = 1;
	box.bottom = desc.Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = (UINT)desc.Width;
	box.top    = 0;

	//�T�u���\�[�X�ɏ�������
	result = wic[index].resource->WriteToSubresource(0, &box, wic[index].decode.get(), wic[index].sub.RowPitch, wic[index].sub.SlicePitch);
	if (FAILED(result))
	{
		OutputDebugString(_T("WIC�T�u���\�[�X�ւ̏������݁F���s\n"));
	}

	wic[index].image.size.x = static_cast<FLOAT>(desc.Width);
	wic[index].image.size.y = static_cast<FLOAT>(desc.Height);
}

void Texture::DrawGraph(UINT & index, FLOAT x, FLOAT y, BOOL turn)
{
	
}

// ���j�R�[�h�ϊ�
std::wstring okmonn::ChangeUnicode(const CHAR * str)
{
	//�������̎擾
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str, -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//�ϊ�
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str, -1, &wstr[0], byteSize);

	return wstr;
}
