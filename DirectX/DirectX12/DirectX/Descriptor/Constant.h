#pragma once
#include "Descriptor.h"
#include "../../etc/Typedef.h"
#include <vector>

class Window;

class Constant :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Constant(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// �f�X�g���N�^
	~Constant();

	// WVP�̍X�V
	void UpDataWVP(void);

	// Image�̍X�V
	void UpDataImage(void);

	// �萔�o�b�t�@�̃Z�b�g
	void SetConstant(UINT rootIndex = 0, UINT index = 0);

	// ���\�[�X�̎擾
	ID3D12Resource* GetResoure(UINT index) const {
		return resource[index];
	}
	// ���\�[�X�̎擾
	std::vector<ID3D12Resource*> GetResoure() const {
		return resource;
	}

private:
	// WVP�̃Z�b�g
	void SetWVP(void);

	// �q�[�v�̐���
	HRESULT CreateHeap(void);

	// ���\�[�X�̐���
	HRESULT CreateResource(void);

	// ���\�[�X�r���[�̐���
	HRESULT CreateView(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// ���\�[�X
	std::vector<ID3D12Resource*> resource;

	// WVP
	WVP wvp;

	// �摜�f�[�^
	Image image;

	// ���M�f�[�^
	std::vector<UINT8*> data;
};

