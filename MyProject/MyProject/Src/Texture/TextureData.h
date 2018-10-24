#pragma once
#include "../etc/VertexData.h"
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct D3D12_SUBRESOURCE_DATA;

namespace texture {
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

	// �F
	struct Color {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		Color() :r(0), g(0), b(0), a(0) {
		}
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
			r(r), g(g), b(b), a(a) {
		}
	};

	// �_�~�[�e�N�X�`��
	struct Dummy {
		//�q�[�v
		ID3D12DescriptorHeap* heap;
		//���\�[�X
		ID3D12Resource* rsc;
		//�摜�f�[�^
		std::vector<Color>data;
	};
}
