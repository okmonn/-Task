#pragma once
#include "../Obj.h"
#include "../../etc/Typedef.h"
#include <memory>
#include <map>

class Window;
class Device;
class List;
class Swap;

class Texture : 
	public Obj
{
	//���_�f�[�^
	struct VertexData {
		//���_
		Vertex vertex[4];
		//���\�[�X
		ID3D12Resource* resource;
		//���M
		UINT* data;
		//���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW view;
	};

	//�摜�f�[�^
	struct Image {
		//��ʃT�C�Y
		DirectX::XMFLOAT2 window;
		//�摜�T�C�Y
		DirectX::XMFLOAT2 size;
		//�摜UV
		DirectX::XMFLOAT2 uv;
	};

	//�萔�o�b�t�@
	struct Constant {
		//�q�[�v
		ID3D12DescriptorHeap* heap;
		//���\�[�X
		ID3D12Resource* resource;
		//���M�f�[�^
		UINT8* data;
		//�T�C�Y
		UINT size;
	};

	//WIC�f�[�^
	struct WIC {
		//�摜�f�[�^
		Image image;
		//���_�f�[�^
		VertexData vertex;
		//�萔�o�b�t�@
		Constant constant;
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

	// �q�[�v�̃Z�b�g
	void SetHeap(UINT* index, UINT i = 2);

	// �`�揀��
	void SetDraw(UINT* index);

	void DrawGraph(UINT& index, FLOAT x, FLOAT y, BOOL turn = FALSE);

private:
	// �q�[�v�̐���
	HRESULT CreateHeap(UINT* index);

	// ���\�[�X�̐���
	HRESULT CreateResourcce(UINT* index);

	// �o�b�t�@�r���[�̐���
	HRESULT CreateView(UINT* index);

	// �V�F�[�_���\�[�X�r���[�̐���
	HRESULT CreateShaderView(UINT* index);

	// ���_���\�[�X�̐���
	HRESULT CreateVertexResource(UINT* index);


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

namespace okmonn {
	// ���j�R�[�h�ϊ�
	std::wstring ChangeUnicode(const CHAR* str);
}
