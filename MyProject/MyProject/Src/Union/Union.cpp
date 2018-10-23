#define _ENABLE_EXTENDED_ALIGNED_STORAGE
#include "Union.h"
#include "../Window/Window.h"
#include "../Debuger/Debug.h"
#include "../Input/Input.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Fence/Fence.h"
#include "../Render/Render.h"
#include "../Depth/Depth.h"
#include "../Constant/Constant.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Point/Point.h"
#include "../Line/Line.h"
#include "../Triangle/Triangle.h"
#include "../Texture/Texture.h"
#include "../etc/Release.h"

#pragma comment(lib, "d3d12.lib")

// �C���v�b�g�G�������g
const D3D12_INPUT_ELEMENT_DESC inputs[] = {
	//0
	{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//1
	{ "NORMAL",   0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//2
	{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//3
	{ "COLOR",    0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//4
	{ "ALPHA",    0, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT,          0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//5
	{ "BORN",     0, DXGI_FORMAT::DXGI_FORMAT_R16G16_UINT,        0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	//6
	{ "WEIGHT",   0, DXGI_FORMAT::DXGI_FORMAT_R8_UINT,            0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

// �R���X�g���N�^
Union::Union() :
	x(0), y(0)
{
}

// �f�X�g���N�^	
Union::~Union()
{
	End();
}

// �E�B���h�E�T�C�Y�̃Z�b�g
void Union::SetWinSize(const unsigned int & x, const unsigned int & y)
{
	this->x = x;
	this->y = y;
}

// ���[�g�V�O�l�`���̃C���X�^���X
void Union::CreateRoot(void)
{
	drwRoot = std::make_shared<Root>(dev, L"Src/Shader/Draw.hlsl");
	texRoot = std::make_shared<Root>(dev, L"Src/Shader/Texture.hlsl");
}

// �p�C�v���C���̃C���X�^���X
void Union::CreatePipe(void)
{
	pntPipe = std::make_shared<Pipe>(dev, swap, drwRoot);
	linPipe = std::make_shared<Pipe>(dev, swap, drwRoot);
	triPipe = std::make_shared<Pipe>(dev, swap, drwRoot);
	{
		D3D12_INPUT_ELEMENT_DESC input[] = {
			inputs[0], inputs[3]
		};
		pntPipe->CreatePipe(*input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
		linPipe->CreatePipe(*input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);
		triPipe->CreatePipe(*input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}

	texPipe = std::make_shared<Pipe>(dev, swap, texRoot);
	{
		D3D12_INPUT_ELEMENT_DESC input[] = {
			inputs[0], inputs[2], inputs[4]
		};
		texPipe->CreatePipe(*input, _countof(input), D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}
}

// �N���X�̃C���X�^���X
void Union::Start(void)
{
	win   = std::make_shared<Window>(x, y);
	deg   = std::make_shared<Debug>();
	input = std::make_shared<Input>(win);
	dev   = std::make_shared <Device>();
	que   = std::make_shared<Queue>(dev);
	list  = std::make_shared<List>(dev);
	swap  = std::make_shared<Swap>(win, que);
	fen   = std::make_shared<Fence>(dev, que);
	ren   = std::make_shared<Render>(dev, list, swap);
	dep   = std::make_shared<Depth>(dev, list);
	con   = std::make_shared<Constant>(dev, list);

	CreateRoot();

	CreatePipe();

	pnt = std::make_shared<Point>(dev, list, con, drwRoot, pntPipe);
	lin = std::make_shared<Line>(dev, list, con, drwRoot, linPipe);
	tri = std::make_shared<Triangle>(dev, list, con, drwRoot, triPipe);

	tex     = std::make_shared<Texture>(dev, list, con, texRoot, texPipe);
}

// ���b�Z�[�W�̊m�F
bool Union::CheckMsg(void)
{
	static MSG msg = {};

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

// �L�[����
bool Union::CheckKey(const int & i)
{
	return input->CheckKey(i);
}

// �g���K�[����
bool Union::CheckTriger(const int & i)
{
	return input->Triger(i);
}

// �摜�̓ǂݍ���
void Union::LoadImg(const std::string & fileName, int& i)
{
	tex->Load(fileName, i);
}

// �`�揀��
void Union::Set(void)
{
	list->Reset(nullptr);

	con->UpDataWvp(0.0f);

	list->SetViewport();

	list->SetScissor();

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
		swap, ren);

	dep->SetDepth();

	ren->SetRender(*dep->GetHeap());
}

// �|�C���g�`��
void Union::DrawPoint(const float & x, const float & y, const float & r, const float & g, const float & b, const float & alpha)
{
	pnt->AddVertexPoint(x, y, r, g, b, alpha);
}

// ���C���`��
void Union::DrawLine(const float & x1, const float & y1, const float & x2, const float & y2, const float & r, const float & g, const float & b, const float & alpha)
{
	lin->AddVertexPoint(x1, y1, x2, y2, r, g, b, alpha);
}

// �g���C�A���O���`��
void Union::DrawTriangle(const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, 
	const float & r, const float & g, const float & b, const float & alpha)
{
	tri->AddVertexPoint(x1, y1, x2, y2, x3, y3, r, g, b, alpha);
}

// �摜�̕`��
void Union::DrawImg(int& i, const float& x, const float& y, const float& alpha, const int& turnX, const int& turnY)
{
	tex->Draw(i, x, y, alpha, turnX, turnY);
}

// �摜�̕`��E�T�C�Y�w��
void Union::DrawSizeImg(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY, const float & alpha, const int & turnX, const int & turnY)
{
	tex->DrawSize(i, x, y, sizeX, sizeY, alpha, turnX, turnY);
}

// �摜�̕`��E�T�C�Y�E�͈͎w��
void Union::DrawRectImg(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY, 
	const float & rectX, const float & rectY, const float & rectSizeX, const float & rectSizeY, const float & alpha, const int & turnX, const int & turnY)
{
	tex->DrawRectSize(i, x, y, sizeX, sizeY, rectX, rectY, rectSizeX, rectSizeY, alpha, turnX, turnY);
}

// �摜�̕`��E4�_�w��
void Union::DrawFreelyImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2,
	const float & x3, const float & y3, const float & x4, const float & y4, const float & alpha, const int & turnX, const int & turnY)
{
	tex->FreelyDraw(i, x1, y1, x2, y2, x3, y3, x4, y4, alpha, turnX, turnY);
}

// �摜�̕`��E4�_�E�͈͎w��
void Union::DrawFreelyRectImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & x4, const float & y4, const float & rectX, const float & rectY, const float & rectSizeX, const float & rectSizeY, const float & alpha, const int & turnX, const int & turnY)
{
	tex->FreelyDrawRect(i, x1, y1, x2, y2, x3, y3, x4, y4, rectX, rectY, rectSizeX, rectSizeY, alpha, turnX, turnY);
}

// �`��
void Union::Draw(void)
{
	pnt->Draw();
	lin->Draw();
	tri->Draw();
}

// ���_�̃��Z�b�g
void Union::Reset(void)
{
	pnt->Reset();
	lin->Reset();
	tri->Reset();
}

// �`����s
void Union::Do(void)
{
	Draw();

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
		swap, ren);

	list->Close();

	//�R�}���h���s.
	ID3D12CommandList* ppCmdLists[] = {
		list->GetList(),
	};
	que->Execute(ppCmdLists, _countof(ppCmdLists));

	Reset();

	swap->Present();

	fen->Wait();
}

// �I��
void Union::End(void)
{
	pnt.reset();
	pntPipe.reset();
	texPipe.reset();
	drwRoot.reset();
	texRoot.reset();
	con.reset();
	dep.reset();
	ren.reset();
	fen.reset();
	swap.reset();
	list.reset();
	que.reset();
	dev.reset();
	deg.reset();
	win.reset();
}