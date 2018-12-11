#include "Union.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../Depth/Depth.h"
#include "../Fence/Fence.h"
#include "../MultiPass/MultiPass.h"
#include "../ShadowMap/ShadowMap.h"
#include "../Root/RootMane.h"
#include "../Root/Root.h"
#include "../Pipe/PipeMane.h"
#include "../Pipe/Pipe.h"
#include "../Texture/Texture.h"
#include "../Primitive/Point.h"
#include "../Primitive/Line.h"
#include "../Primitive/Triangle.h"
#include "../Camera/Camera.h"
#include "../Primitive3D/Plane.h"
#include "../Pmd/Pmd.h"
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// �R���X�g���N�^
Union::Union(std::weak_ptr<Window>win) : 
	root(RootMane::Get()), pipe(PipeMane::Get()), win(win)
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	rootNo.clear();
	pipeNo.clear();
	point.clear();
	line.clear();
	triangle.clear();
	plane.clear();

	Create();
}

// �f�X�g���N�^
Union::~Union()
{
}

// �J�����̈ʒu�E�^�[�Q�b�g
void Union::SetCamera(const float & x, const float & y, const float & z, const float & targetX, const float & targetY, const float & targetZ)
{
	cam->ChangeView({ x, y, z }, { targetX, targetY, targetZ });
}

// ���[�g�V�O�l�`���̐���
void Union::CreateRoot(const std::string & name, const std::tstring & fileName)
{
	if (rootNo.find(name) != rootNo.end())
	{
		return;
	}

	rootNo[name] = 0;
	root.CreateRoot(rootNo[name], dev, fileName);
}

// ���[�g�V�O�l�`���̐���
void Union::CreateRoot(void)
{
	CreateRoot("multipass",   L"Shader/MultiPass.hlsl");
	CreateRoot("texture",     L"Shader/Texture.hlsl");
	CreateRoot("primitive",   L"Shader/Primitive.hlsl");
	CreateRoot("primitive3D", L"Shader/Primitive3D.hlsl");
	CreateRoot("model",       L"Shader/Model.hlsl");
	CreateRoot("shadow",      L"Shader/Shadow.hlsl");
}

// �p�C�v���C���̐���
void Union::CreatePipe(const std::string & name, const std::string & rootName, const D3D12_PRIMITIVE_TOPOLOGY_TYPE & type,
	const std::initializer_list<int> & index, const bool & depth)
{
	if (pipeNo.find(name) != pipeNo.end())
	{
		return;
	}

	pipeNo[name] = 0;
	pipe.CreatePipe(pipeNo[name], dev, swap, root.Get(rootNo[rootName]), type, index, depth);
}

// �p�C�v���C���̐���
void Union::CreatePipe(void)
{
	CreatePipe("multipass",  "multipass",   D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 2 },    false);
	CreatePipe("texture",    "texture",     D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 2 },    false);
	CreatePipe("point",      "primitive",   D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,    { 0, 3 },    false);
	CreatePipe("line",       "primitive",   D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,     { 0, 3 },    false);
	CreatePipe("triangle",   "primitive",   D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 3 },    false);
	CreatePipe("plane",      "primitive3D", D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 2, 3 }, false);
	CreatePipe("model",      "model",       D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 1, 2, 5, 6 }, true);
	CreatePipe("shadow",     "shadow",      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 1, 2, 5, 6 }, true);
}

// �N���X�̐���
void Union::Create(void)
{
	dev   = std::make_shared<Device>();
	queue = std::make_shared<Queue>(dev);
	list  = std::make_shared<List>(dev);
	swap  = std::make_shared<Swap>(win, queue);
	ren   = std::make_unique<Render>(dev, swap);
	depth = std::make_shared<Depth>(dev, win.lock()->GetX(), win.lock()->GetY());
	fence = std::make_shared<Fence>(dev, queue);

	multi = std::make_unique<MultiPass>(win, dev, list);
	shadow = std::make_shared<ShadowMap>(win, dev, list);

	CreateRoot();
	CreatePipe();

	cam = std::make_shared<Camera>(win, dev);
	cam->ChangeView({ 0.0f, 10.0f, -15.0f }, {0.0f, 10.0f, 0.0f});
	tex = std::make_unique<Texture>(win, dev, root.Get(rootNo["texture"]), pipe.Get(pipeNo["texture"]));
	pmd = std::make_unique<Pmd>(dev, cam, root.Get(rootNo["model"]), pipe.Get(pipeNo["model"]), root.Get(rootNo["shadow"]), pipe.Get(pipeNo["shadow"]));
}

// �摜�̓ǂݍ���
void Union::LoadImg(const std::string & fileName, int & i)
{
	tex->Load(fileName, i);
}

// �z��f�[�^����摜�𐶐�
void Union::CreateImg(const std::vector<unsigned char>& data, const unsigned int & width, const unsigned int & height, int & i)
{
	tex->CreateImg(data, width, height, i);
}

// �摜�̕`��
void Union::DrawImg(int & i, const float & x, const float & y, const float & sizeX, const float & sizeY, const float & rectX, const float & rectY, 
	const float & rectSizexX, const float & rectSizeY, const float & alpha, const bool & turnX, const bool & turnY)
{
	tex->Draw(list, i, { x, y }, { sizeX, sizeY }, { rectX, rectY }, { rectSizexX, rectSizeY }, alpha, turnX, turnY);
}

// �_�̕`��
void Union::DrawPoint(const float & x, const float & y, const float & r, const float & g, const float & b, const float & alpha)
{
	point.push_back(std::make_shared<Point>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["point"])));
	point.back()->SetVertex({ x, y }, { r, g, b }, alpha);
	point.back()->Draw(list);
}

// ���̕`��
void Union::DrawLine(const float & x1, const float & y1, const float & x2, const float & y2, 
	const float & r, const float & g, const float & b, const float & alpha)
{
	line.push_back(std::make_shared<Line>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["line"])));
	line.back()->SetVertex({ x1, y1 }, { x2, y2 }, { r, g, b }, alpha);
	line.back()->Draw(list);
}

// �O�p�`�̕`��
void Union::DrawTriangle(const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & r, const float & g, const float & b, const float & alpha)
{
	triangle.push_back(std::make_shared<Triangle>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["triangle"])));
	triangle.back()->SetVertex({ x1, y1 }, { x2, y2 }, { x3, y3 }, { r, g, b }, alpha);
	triangle.back()->Draw(list);
}

// �l�p�`�̕`��
void Union::DrawBox(const float & x, const float & y, const float & sizeX, const float & sizeY, const float & r, const float & g, const float & b, const float & alpha)
{
	triangle.push_back(std::make_shared<Triangle>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["triangle"])));
	triangle.back()->SetVertex({ x, y }, { x + sizeX, y }, { x, y + sizeY }, { r, g, b }, alpha);
	triangle.back()->Draw(list);

	triangle.push_back(std::make_shared<Triangle>(win, dev, root.Get(rootNo["primitive"]), pipe.Get(pipeNo["triangle"])));
	triangle.back()->SetVertex({ x + sizeX, y }, { x + sizeX, y + sizeY }, { x, y + sizeY }, { r, g, b }, alpha);
	triangle.back()->Draw(list);
}

// ���̕`��
void Union::DrawPlane(const float & x, const float & y, const float& z, const float & sizeX, const float & sizeY, const float & sizeZ,
	const float & r, const float & g, const float & b, const float & alpha)
{
	plane.push_back(std::make_shared<Plane>(dev, cam, root.Get(rootNo["primitive3D"]), pipe.Get(pipeNo["plane"])));
	plane.back()->SetVertex({ x, y, z }, { sizeX, sizeY, sizeZ }, { r, g, b }, alpha);
	plane.back()->Draw(list, shadow);
}

// PMD�̓ǂݍ���
void Union::LoadPmd(const std::string & fileName, int & i)
{
	pmd->Load(fileName, i);
}

// PMD��Y����]
void Union::RotatePmd(int & i, const float & angle)
{
	pmd->Rotate(i, angle);
}

// PMD�̕`��
void Union::DrawPmd(int & i)
{
	pmd->Draw(list, i);
}

// PMD�̉e�`��
void Union::DrawPmdShadow(int & i)
{
	pmd->DrawShadow(list, i);
}

// ���[�V�����̓K��
void Union::Attach(const std::string & fileName, int & i)
{
	pmd->Attach(fileName, i);
}

// �A�j���[�V����
void Union::Animation(int & i, const bool & loop, const float & animSpeed)
{
	pmd->Animation(i, loop, animSpeed);
}

// �e���̃N���A
void Union::ClearShadow(void)
{
	shadow->Set();
}

// �e�̎��s
void Union::ExecutionShadow(void)
{
	shadow->Execution();
}

// ��ʃN���A
void Union::Clear(void)
{
	/*list->Reset();

	list->SetView(win.lock()->GetX(), win.lock()->GetY());
	list->SetScissor(win.lock()->GetX(), win.lock()->GetY());

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, ren->Get());

	depth->Clear(list);
	ren->Clear(list, depth->GetHeap());*/


	multi->Set(depth);
}

// ���s
void Union::Execution(void)
{
	multi->Execution();

	list->Reset();

	list->SetView(win.lock()->GetX(), win.lock()->GetY());
	list->SetScissor(win.lock()->GetX(), win.lock()->GetY());

	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, ren->Get());

	depth->Clear(list);
	ren->Clear(list, depth->GetHeap());


	multi->Draw(list, root.Get(rootNo["multipass"]), pipe.Get(pipeNo["multipass"]), shadow);



	list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, ren->Get());

	list->GetList()->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};
	queue->Get()->ExecuteCommandLists(_countof(com), com);

	swap->Get()->Present(1, 0);

	fence->Wait();

	point.clear();
	line.clear();
	triangle.clear();
	plane.clear();
}