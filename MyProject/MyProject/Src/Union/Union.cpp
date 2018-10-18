#include "Union.h"
#include "../Window/Window.h"
#include "../Debuger/Debug.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../Depth/Depth.h"
#include "../Constant/Constant.h"

#pragma comment(lib, "d3d12.lib")

// �R���X�g���N�^
Union::Union() :
	x(0), y(0)
{

}

// �f�X�g���N�^	
Union::~Union()
{
}

// �E�B���h�E�T�C�Y�̃Z�b�g
void Union::SetWinSize(const unsigned int & x, const unsigned int & y)
{
	this->x = x;
	this->y = y;
}

// �N���X�̃C���X�^���X
void Union::Create(void)
{
	win  = std::make_shared<Window>(x, y);
	deg  = std::make_shared<Debug>();
	dev  = std::make_shared <Device>();
	que  = std::make_shared<Queue>(dev);
	list = std::make_shared<List>(dev);
	swap = std::make_shared<Swap>(win, que);
	ren  = std::make_shared<Render>(dev, list, swap);
	dep  = std::make_shared<Depth>(dev, list);
	con  = std::make_shared<Constant>(dev, list);
}