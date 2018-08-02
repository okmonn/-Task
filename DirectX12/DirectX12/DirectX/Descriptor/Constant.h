#pragma once
#include "Descriptor.h"
#include "../../etc/Typedef.h"

class Window;

class Constant :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Constant(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Constant();

	// WVP�̍X�V
	void UpDataWVP(void);

	// WVP�̎擾
	WVP GetWvp(void) const {
		return wvp;
	}

	// �萔�o�b�t�@�̃Z�b�g
	void SetConstant(UINT index = 0, UINT table = 0);

private:
	// WVP�̃Z�b�g
	void SetWorldViewProjection(void);

	// ���\�[�X�̐���
	HRESULT CreateResource(void);

	// ���\�[�X�r���[�̐���
	HRESULT CreateView(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// WVP
	WVP wvp;

	// ���M�f�[�^
	UINT8* data;
};

