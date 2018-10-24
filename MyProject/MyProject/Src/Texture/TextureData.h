#pragma once
#include "../etc/VertexData.h"
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct D3D12_SUBRESOURCE_DATA;

// �摜�f�[�^
struct Tex {
	// �f�R�[�h�f�[�^
	unsigned char* decode;
	// �T�u�f�[�^
	std::weak_ptr<D3D12_SUBRESOURCE_DATA>sub;

	//�萔�q�[�v
	ID3D12DescriptorHeap* heap;
	//�萔���\�[�X
	ID3D12Resource* c_rsc;

	//���_�f�[�^
	std::vector<tex::Vertex>vertex;

	//���_���\�[�X
	ID3D12Resource* v_rsc;
	//���M�f�[�^
	unsigned int* data;
};
