#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <map>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;


namespace pmd {
#pragma pack(1)
	// �w�b�_�[
	struct Header {
		//�^�C�v
		unsigned char type[3];
		//�o�[�W����
		float ver;
		//���O
		unsigned char name[20];
		//�R�����g
		unsigned char comment[256];
	};
#pragma pack()

	// ���_�f�[�^
	struct Vertex {
		//���W
		DirectX::XMFLOAT3 pos;
		//�@��
		DirectX::XMFLOAT3 normal;
		//UV
		DirectX::XMFLOAT2 uv;
		//�{�[���ԍ�
		unsigned short bornNo[2];
		//�E�F�C�g
		unsigned char weight;
		//�֊s���t���O
		unsigned char edge;
	};

#pragma pack(1)
	// �}�e���A��
	struct Material {
		//��{�F
		DirectX::XMFLOAT3 diffuse;
		//�����x
		float alpha;
		//���ˋ��x
		float specularity;
		//���ːF
		DirectX::XMFLOAT3 specula;
		//���F
		DirectX::XMFLOAT3 mirror;
		//�g�D�[���ԍ�
		unsigned char toonIndex;
		//�֊s���t���O
		unsigned char edge;
		//�C���f�b�N�X��
		unsigned int indexNum;
		//�e�N�X�`���p�X
		unsigned char texPath[20];
	};
#pragma pack()

	// �{�[��
	struct Born {
		//���O
		char name[20];
		//�e�{�[���ԍ�
		unsigned short pIndex;
		//�q�{�[���ԍ�
		unsigned short cIndex;
		//�^�C�v
		unsigned char type;
		//IK�e�{�[���ԍ�
		unsigned short IKpIndex;
		//���W
		DirectX::XMFLOAT3 pos;
	};

	// IK�f�[�^
	struct IkBorn {
		//IK�{�[���C���f�b�N�X
		unsigned short index;
		//�^�[�Q�b�g�{�[��
		unsigned short target;
		//IK�̃`�F�C���̒���
		unsigned char chain;
		//
		unsigned short iteration;
		//�E�F�C�g
		unsigned int weight;
		//�q�{�[���̃C���f�b�N�X
		std::vector<unsigned short>child;
	};

	// �}�e���A��
	struct Mat {
		//��{�F
		DirectX::XMFLOAT3 diffuse;
		//�����x
		float alpha;
		//���ˋ��x
		float specularity;
		//���ːF
		DirectX::XMFLOAT3 specula;
		//���F
		DirectX::XMFLOAT3 mirror;
		//��Z�e�N�X�`���t���O
		int sph;
		//���Z�e�N�X�`���t���O
		int spa;
		//�e�N�X�`���t���O
		int tex;
	};
};
