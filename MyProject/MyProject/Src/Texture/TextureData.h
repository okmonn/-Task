#pragma once
#include "../etc/VertexData.h"
#include <vector>

namespace DirectX {
	struct TexMetadata;
	class ScratchImage;
}

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

// �摜�f�[�^
struct Tex {
	//���^�f�[�^
	DirectX::TexMetadata* meta;
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage* img;

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
