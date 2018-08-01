#include "Union.h"
#include "../Window/Window.h"
#ifdef _DEBUG
#include "Debug.h"
#endif
#include "Device.h"
#include "Command/Queue.h"
#include "Command/List.h"
#include "Swap.h"
#include "Descriptor/Render.h"
#include "Descriptor/Depth.h"
#include "Descriptor/Constant.h"
#include "RootSignature.h"
#include <tchar.h>

#pragma comment (lib, "d3d12.lib")

// �R���X�g���N�^
Union::Union() : x(x), y(y)
{
	msg = {};
}

// �f�X�g���N�^
Union::~Union()
{
}

// �N���X�̃C���X�^���X��
void Union::Create(void)
{
	win      = std::make_shared<Window>(x, y);
#ifdef _DEBUG
	debug    = std::make_shared<Debug>();
#endif
	dev      = std::make_shared<Device>();
	queue    = std::make_shared<Queue>(dev);
	list     = std::make_shared<List>(dev);
	swap     = std::make_shared<Swap>(win, queue);
	render   = std::make_shared<Render>(dev, swap);
	depth    = std::make_shared<Depth>(win, dev, swap);
	constant = std::make_shared<Constant>(win, dev, swap);
	root     = std::make_shared<RootSignature>(dev, _T("Shader2D.hlsl"), "BasicVS", "BasicPS");
}

// �E�B���h�E�̃T�C�Y�Z�b�g
void Union::ChangeWindowSize(unsigned int x, unsigned int y)
{
	this->x = x;
	this->y = y;
}

// ���b�Z�[�W�̊m�F
bool Union::CheckMsg(void)
{
	if (msg.message == WM_QUIT)
	{
		return false;
	}
	else
	{
		//�Ăяo�����X���b�h�����L���Ă���E�B���h�E�ɑ��M���ꂽ���b�Z�[�W�ۗ̕�����Ă��镨���擾
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
			TranslateMessage(&msg);
			//1�̃E�B�h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o����
			DispatchMessage(&msg);
		}
	}

	return true;
}
