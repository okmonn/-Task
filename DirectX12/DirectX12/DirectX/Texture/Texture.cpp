#include "Texture.h"
#include "WICTextureLoader12.h"
#include "../../Window/Window.h"
#include "../Device.h"
#include "../Command/List.h"
#include "../Swap.h"
#include <tchar.h>

// �R���X�g���N�^
Texture::Texture(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap) : 
	win(win), dev(dev), list(list), swap(swap)
{
	constant = {};
	image = {};
	image.window = { static_cast<FLOAT>(this->win.lock()->GetX()), static_cast<FLOAT>(this->win.lock()->GetY()) };
	wic.clear();


	/*CreateHeap();
	CreateResource();
	CreateConstantView();*/

	CoInitialize(nullptr);
}

// �f�X�g���N�^
Texture::~Texture()
{
	if (constant.resource != nullptr)
	{
		constant.resource->Unmap(0, nullptr);
	}
	Release(constant.resource);
	Release(constant.heap);
	Liberation();
}

// WIC�ǂݍ���
HRESULT Texture::LoadWIC(UINT & index, std::string fileName)
{
	UINT* n = &index;

	std::wstring path = func::ChangeUnicode(fileName.c_str());

	result = DirectX::LoadWICTextureFromFile(dev.lock()->Get(), path.c_str(), &wic[n].resource, wic[n].decode, wic[n].sub);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWIC�e�N�X�`���̓ǂݍ��݁F���s\n"));
		return result;
	}

	result = CreateHeap(n);
	result = CreateResourceView(n);
	result = CreateVertex(n);

	return result;
}

// �q�[�v�̐���
HRESULT Texture::CreateHeap(void)
{
	//�q�[�v�ݒ�p�\���̂̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = swap.lock()->GetBack();
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//�q�[�v����
	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&constant.heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�e�N�X�`���̒萔�o�b�t�@�̃q�[�v�����F���s\n"));
	}

	return result;
}

// ���\�[�X�̐���
HRESULT Texture::CreateResource(void)
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
	desc.Width            = (sizeof(Image) + 0xff) &~0xff;
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constant.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�e�N�X�`���̒萔�o�b�t�@�̃��\�[�X�����F���s\n"));
	}

	return result;
}

// �萔�o�b�t�@�r���[�̐���
HRESULT Texture::CreateConstantView(void)
{
	//�萔�o�b�t�@�r���[�ݒ�p�\���̂̐ݒ�
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = constant.resource->GetGPUVirtualAddress();
	desc.SizeInBytes    = (sizeof(Image) + 0xff) &~0xff;

	//�萔�o�b�t�@�r���[����
	dev.lock()->Get()->CreateConstantBufferView(&desc, constant.heap->GetCPUDescriptorHandleForHeapStart());

	//���M�͈�
	D3D12_RANGE range = { 0, 0 };

	//�}�b�s���O
	result = constant.resource->Map(0, &range, (void**)(&constant.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\n�e�N�X�`���̒萔���\�[�X�̃}�b�s���O�F���s\n"));
	}

	//�R�s�[
	memcpy(constant.data, &image, sizeof(DirectX::XMFLOAT2));

	return result;
}

// �q�[�v�̐���
HRESULT Texture::CreateHeap(UINT* index)
{
	//�q�[�v�ݒ�p�\���̂̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = swap.lock()->GetBack();
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//�q�[�v����
	result = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&wic[index].heap));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWIC�̃q�[�v�����F���s\n"));
	}

	return result;
}

// ���\�[�X�̐���
HRESULT Texture::CreateResourceView(UINT * index)
{
	//�V�F�[�_���\�[�X�r���[�ݒ�p�\���̂̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format                    = wic[index].resource->GetDesc().Format;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	//�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE handle = wic[index].heap->GetCPUDescriptorHandleForHeapStart();

	dev.lock()->Get()->CreateShaderResourceView(wic[index].resource, &desc, handle);

	return result;
}

// ���_�o�b�t�@�̐���
HRESULT Texture::CreateVertex(UINT * index)
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
	desc.Width            = sizeof(wic[index].v.vertex);
	desc.Height           = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels        = 1;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//���\�[�X����
	result = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&wic[index].v.resource));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWIC�̒��_���\�[�X�����F���s\n"));
	}

	//���M�͈�
	D3D12_RANGE range = { 0,0 };

	//�}�b�s���O
	result = wic[index].v.resource->Map(0, &range, reinterpret_cast<void**>(&wic[index].v.data));
	if (FAILED(result))
	{
		OutputDebugString(_T("\nWIC�̒��_�}�b�s���O�F���s\n"));
	}

	//���_�f�[�^�̃R�s�[
	memcpy(wic[index].v.data, &wic[index].v.vertex, sizeof(wic[index].v.vertex));

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	wic[index].v.view.BufferLocation = wic[index].v.resource->GetGPUVirtualAddress();
	wic[index].v.view.SizeInBytes    = sizeof(wic[index].v.vertex);
	wic[index].v.view.StrideInBytes  = sizeof(Vertex);

	return result;
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
	box.right  = static_cast<UINT>(desc.Width);
	box.top    = 0;

	//�T�u���\�[�X�ɏ�������
	result = wic[index].resource->WriteToSubresource(0, &box, wic[index].decode.get(), wic[index].sub.RowPitch, wic[index].sub.SlicePitch);
	if (FAILED(result))
	{
		OutputDebugString(_T("WIC�̃T�u���\�[�X�ւ̏������݁F���s\n"));
	}
}

// �q�[�v�̃Z�b�g
void Texture::SetHeap(UINT table)
{
	//�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE handle = constant.heap->GetGPUDescriptorHandleForHeapStart();

	//�q�[�v�̃Z�b�g
	list.lock()->GetList()->SetDescriptorHeaps(1, &constant.heap);

	//�f�B�X�N���v�^�[�e�[�u���̃Z�b�g
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(table, handle);
}

// �q�[�v�̃Z�b�g
void Texture::SetHeap(UINT * index, UINT table)
{
	//�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE handle = wic[index].heap->GetGPUDescriptorHandleForHeapStart();

	//�q�[�v�̃Z�b�g
	list.lock()->GetList()->SetDescriptorHeaps(1, &wic[index].heap);

	//�f�B�X�N���v�^�[�e�[�u���̃Z�b�g
	list.lock()->GetList()->SetGraphicsRootDescriptorTable(table, handle);
}

// �`��
void Texture::Draw(UINT & index)
{
	UINT* n = &index;

	//����
	wic[n].v.vertex[0] = { {-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f} };
	//�E��
	wic[n].v.vertex[1] = { { 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f} };
	//����
	wic[n].v.vertex[2] = { {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} };
	//�E��
	wic[n].v.vertex[3] = { { 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} };

	//���_�f�[�^�̍X�V
	memcpy(wic[n].v.data, &wic[n].v.vertex, (sizeof(wic[n].v.vertex)));

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	wic[n].v.view.BufferLocation = wic[n].v.resource->GetGPUVirtualAddress();
	wic[n].v.view.SizeInBytes    = sizeof(wic[n].v.vertex);
	wic[n].v.view.StrideInBytes  = sizeof(Vertex);

	//���_�o�b�t�@�r���[�̃Z�b�g
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &wic[n].v.view);

	//�g�|���W�[�ݒ�
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SetDraw(n);
	SetHeap(n);

	//�`��
	list.lock()->GetList()->DrawInstanced(VERTEX_MAX, 1, 0, 0);
}

// �`��
void Texture::Draw(UINT & index, const Vec2f & pos, const Vec2f & size)
{
	UINT* n = &index;

	memcpy(constant.data, &image, sizeof(Image));

	SetHeap();

	////����
	//wic[n].v.vertex[0] = { { -1.0f,  1.0f, 0.0f },{ 0.0f, 0.0f } };
	////�E��
	//wic[n].v.vertex[1] = { { 1.0f,  1.0f, 0.0f },{ 1.0f, 0.0f } };
	////����
	//wic[n].v.vertex[2] = { { -1.0f, -1.0f, 0.0f },{ 0.0f, 1.0f } };
	////�E��
	//wic[n].v.vertex[3] = { { 1.0f, -1.0f, 0.0f },{ 1.0f, 1.0f } };

	//����
	wic[n].v.vertex[0] = { { pos.x,  pos.y, 0.0f },{ 0.0f, 0.0f } };
	//�E��
	wic[n].v.vertex[1] = { { pos.x + size.x,  pos.y, 0.0f },{ 1.0f, 0.0f } };
	//����
	wic[n].v.vertex[2] = { { pos.x, pos.y + size.y, 0.0f },{ 0.0f, 1.0f } };
	//�E��
	wic[n].v.vertex[3] = { { pos.x + size.x, pos.y + size.y, 0.0f },{ 1.0f, 1.0f } };

	//���_�f�[�^�̍X�V
	memcpy(wic[n].v.data, &wic[n].v.vertex, (sizeof(wic[n].v.vertex)));

	//���_�o�b�t�@�ݒ�p�\���̂̐ݒ�
	wic[n].v.view.BufferLocation = wic[n].v.resource->GetGPUVirtualAddress();
	wic[n].v.view.SizeInBytes = sizeof(wic[n].v.vertex);
	wic[n].v.view.StrideInBytes = sizeof(Vertex);

	//���_�o�b�t�@�r���[�̃Z�b�g
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &wic[n].v.view);

	//�g�|���W�[�ݒ�
	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SetDraw(n);
	SetHeap(n);

	//�`��
	list.lock()->GetList()->DrawInstanced(VERTEX_MAX, 1, 0, 0);
}

// ���
void Texture::Liberation(void)
{
	for (auto itr = wic.begin(); itr != wic.end(); ++itr)
	{
		if (itr->second.v.resource != nullptr)
		{
			itr->second.v.resource->Unmap(0, nullptr);
		}
		Release(itr->second.v.resource);
		itr->second.decode.release();
		Release(itr->second.resource);
		Release(itr->second.heap);
	}
}


// ���j�R�[�h�ϊ�
std::wstring func::ChangeUnicode(const CHAR * str)
{
	//�������̎擾
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str, -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//�ϊ�
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str, -1, &wstr[0], byteSize);

	return wstr;
}
