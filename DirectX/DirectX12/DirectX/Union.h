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
class Fence;

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

	// �E�B���h�E
	std::shared_ptr<Window>win;

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
};

