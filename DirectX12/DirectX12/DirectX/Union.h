#pragma once
#include <d3d12.h>
#include <memory>

class Window;
#ifdef _DEBUG
class Debug;
#endif
class Device;
class Queue;
class List;
class Swap;
class Render;
class Depth;
class Constant;
class RootSignature;
class Compiler;
class Pipe;
class Fence;
class Barrier;

class Union
{
public:
	// �f�X�g���N�^
	~Union();

	// �C���X�^���X�ϐ��̎擾
	static Union& Get(void) {
		static Union instance;
		return instance;
	}

	// �N���X�̃C���X�^���X��
	void Create(void);

	// �E�B���h�E�̃T�C�Y�Z�b�g
	void ChangeWindowSize(UINT x, UINT y);

	// ���b�Z�[�W�̊m�F
	bool CheckMsg(void);

	// �`�揀��
	void Set(void);

	// ���s
	void Do(void);

private:
	// �R���X�g���N�^
	Union();
	Union(const Union&) {
	}
	void operator=(const Union&) {
	}


	// ��ʃT�C�YX
	UINT x;

	// ��ʃT�C�YY
	UINT y;

	// ���b�Z�[�W
	MSG msg;

	// �r���[�|�[�g
	D3D12_VIEWPORT viewPort;

	// �V�U�[
	RECT scissor;


	std::shared_ptr<Window>win;
#ifdef _DEBUG
	// �f�o�b�O
	std::shared_ptr<Debug>debug;
#endif
	std::shared_ptr<Device>dev;
	std::shared_ptr<Queue>queue;
	std::shared_ptr<List>list;
	std::shared_ptr<Swap>swap;
	std::shared_ptr<Render>render;
	std::shared_ptr<Depth>depth;
	std::shared_ptr<Constant>constant;
	std::shared_ptr<RootSignature>root;
	std::shared_ptr<Pipe>pipe;
	std::shared_ptr<Fence>fence;
	std::shared_ptr<Barrier>barrier;
};

