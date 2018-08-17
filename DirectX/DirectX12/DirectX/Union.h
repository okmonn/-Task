#pragma once
#include <d3d12.h>
#include <memory>

class Window;
class Input;
#ifdef _DEBUG
class Debug;
#endif
class Device;
class Queue;
class List;
class Swap;
class Render;
class Depth;
class Fence;
class Root;
class Pipe;
class Constant;
class Texture;

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

	// �E�B���h�E�̃T�C�Y�Z�b�g
	void ChangeWindowSize(UINT x, UINT y);

	// �N���X�̃C���X�^���X��
	void Create(void);

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

	// �r���[�|�[�g�̃Z�b�g
	void ViewPort(void);

	// �V�U�[�̃Z�b�g
	void Scissor(void);

	// �o���A�̃Z�b�g
	void Barrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter);

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

	// �o���A
	D3D12_RESOURCE_BARRIER barrier;

	// �E�B���h�E
	std::shared_ptr<Window>win;

	// �C���v�b�g
	std::shared_ptr<Input>input;

#ifdef _DEBUG
	std::shared_ptr<Debug>debug;
#endif

	// �f�o�C�X
	std::shared_ptr<Device>dev;

	// �R�}���h�L���[
	std::shared_ptr<Queue>queue;

	// �R�}���h���X�g
	std::shared_ptr<List>list;

	// �X���b�v�`�F�C��
	std::shared_ptr<Swap>swap;

	// �����_�[�^�[�Q�b�g
	std::shared_ptr<Render>render;

	// �[�x�X�e���V��
	std::shared_ptr<Depth>depth;

	// �t�F���X
	std::shared_ptr<Fence>fence;

	// ���[�g�V�O�l�`��
	std::shared_ptr<Root>root;

	// �p�C�v���C��
	std::shared_ptr<Pipe>pipe;

	// �萔�o�b�t�@
	std::shared_ptr<Constant>constant;

	// �e�N�X�`��
	std::shared_ptr<Texture>tex;
};
