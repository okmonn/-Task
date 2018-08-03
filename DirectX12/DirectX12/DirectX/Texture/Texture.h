#pragma once
#include "../Obj.h"
#include "../../etc/Typedef.h"
#include <memory>
#include <map>
#include <string>

class Window;
class Device;
class List;
class Swap;

#define VERTEX_MAX 4

class Texture :
	public Obj
{
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
	void SetHeap(UINT* index, UINT table = 2);

	// �`��
	void Draw(UINT& index);

private:
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

	// WIC�f�[�^
	std::map<UINT*, WIC>wic;
};

namespace func {
	// ���j�R�[�h�ϊ�
	std::wstring ChangeUnicode(const CHAR* str);
}

