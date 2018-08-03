#pragma once
#include "../Obj.h"
#include "../../etc/Vector2.h"
#include "../../etc/Typedef.h"
#include <memory>
#include <map>
#include <string>

class Window;
class Device;
class List;
class Swap;

// ���_�f�[�^�̍ő吔
#define VERTEX_MAX 4

class Texture :
	public Obj
{
	// �萔�o�b�t�@
	struct Constant {
		//�q�[�v
		ID3D12DescriptorHeap* heap;
		//���\�[�X
		ID3D12Resource* resource;
		//���M�f�[�^
		UINT8* data;
	};

	// �摜�f�[�^
	struct Image {
		//�E�B���h�E�T�C�Y
		DirectX::XMFLOAT2 window;
	};

	// ���_�f�[�^
	struct VertexData {
		//���_
		Vertex vertex[VERTEX_MAX];
		//���\�[�X
		ID3D12Resource* resource;
		//���M�f�[�^
		UINT* data;
		//���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW view;
	};

	// WIC�f�[�^
	struct WIC {
		//���_�f�[�^
		VertexData v;
		//�q�[�v
		ID3D12DescriptorHeap* heap;
		//���\�[�X
		ID3D12Resource* resource;
		//�f�R�[�h
		std::unique_ptr<uint8_t[]>decode;
		//�T�u
		D3D12_SUBRESOURCE_DATA sub;
	};

public:
	// �R���X�g���N�^
	Texture(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Texture();

	// WIC�ǂݍ���
	HRESULT LoadWIC(UINT& index, std::string fileName);

	// �`�揀��
	void SetDraw(UINT* index);

	// �q�[�v�̃Z�b�g
	void SetHeap(UINT table = 1);
	void SetHeap(UINT* index, UINT table = 2);

	// �`��
	void Draw(UINT& index);
	// �`��
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size);

private:
	// �q�[�v�̐���
	HRESULT CreateHeap(void);

	// ���\�[�X�̐���
	HRESULT CreateResource(void);

	// �萔�o�b�t�@�r���[�̐���
	HRESULT CreateConstantView(void);

	// �q�[�v�̐���
	HRESULT CreateHeap(UINT* index);
	
	// ���\�[�X�̐���
	HRESULT CreateResourceView(UINT* index);

	// ���_�o�b�t�@�̐���
	HRESULT CreateVertex(UINT* index);

	// ���
	void Liberation(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// �X���b�v�`�F�C��
	std::weak_ptr<Swap>swap;

	// �萔�o�b�t�@
	Constant constant;

	// �摜�f�[�^
	Image image;

	// WIC�f�[�^
	std::map<UINT*, WIC>wic;
};

namespace func {
	// ���j�R�[�h�ϊ�
	std::wstring ChangeUnicode(const CHAR* str);
}

