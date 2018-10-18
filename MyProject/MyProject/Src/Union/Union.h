#pragma once
#include <memory>

class Window;
class Debug;
class Device;
class Queue;
class List;
class Swap;
class Render;
class Depth;
class Constant;

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

	// �E�B���h�E�T�C�Y�̃Z�b�g
	void SetWinSize(const unsigned int& x, const unsigned int& y);

	// �N���X�̃C���X�^���X
	void Create(void);

	// �E�B���h�E�T�C�YX�̎擾
	unsigned int GetWinX(void) const {
		return x;
	}
	// �E�B���h�E�T�C�YY�̎擾
	unsigned int GetWinY(void) const {
		return y;
	}

private:
	// �R���X�g���N�^
	Union();
	Union(const Union&) {
	}
	void operator=(const Union&) {
	}


	// �E�B���h�E�T�C�YX
	unsigned int x;

	// �E�B���h�E�T�C�YY
	unsigned int y;

	// �E�B���h�E
	std::shared_ptr<Window>win;

	// �f�o�b�K
	std::shared_ptr<Debug>deg;

	// �f�o�C�X
	std::shared_ptr<Device>dev;

	// �R�}���h�L���[
	std::shared_ptr<Queue>que;

	// �R�}���h���X�g
	std::shared_ptr<List>list;

	// �X���b�v�`�F�C��
	std::shared_ptr<Swap>swap;

	// �����_�[�^�[�Q�b�g
	std::shared_ptr<Render>ren;

	// �[�x�X�e���V��
	std::shared_ptr<Depth>dep;

	// �萔�o�b�t�@
	std::shared_ptr<Constant>con;
};
