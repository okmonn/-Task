#pragma once
#include <memory>
#include <string>

class Window;
class Debug;
class Device;
class Queue;
class List;
class Swap;
class Fence;
class Render;
class Depth;
class Constant;
class Root;
class Pipe;
class Point;
class Line;
class Triangle;
class TextureLoader;
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

	// �E�B���h�E�T�C�Y�̃Z�b�g
	void SetWinSize(const unsigned int& x, const unsigned int& y);

	// �N���X�̃C���X�^���X
	void Start(void);

	// ���b�Z�[�W�̊m�F
	bool CheckMsg(void);

	// �摜�̓ǂݍ���
	void LoadImg(const std::string& fileName, int& i);

	// �`�揀��
	void Set(void);

	// �|�C���g�`��
	void DrawPoint(const float& x, const float& y, const float& r, const float& g, const float& b, const float& alpha);

	// ���C���`��
	void DrawLine(const float& x1, const float& y1,const float& x2, const float& y2,  const float& r, const float& g, const float& b, const float& alpha);

	// �g���C�A���O���`��
	void DrawTriangle(const float& x1, const float& y1, const float& x2, const float& y2, const float& x3, const float& y3,
		const float& r, const float& g, const float& b, const float& alpha);

	// �摜�̕`��
	void DrawImg(int& i, const float& x, const float& y, const float& alpha, const int& turnX, const int& turnY);

	// �`����s
	void Do(void);

	// �I��
	void End(void);

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

	// ���[�g�V�O�l�`���̃C���X�^���X
	void CreateRoot(void);

	// �p�C�v���C���̃C���X�^���X
	void CreatePipe(void);

	// �`��
	void Draw(void);

	// ���_�̃��Z�b�g
	void Reset(void);


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

	// �t�F���X
	std::shared_ptr<Fence>fen;

	// �����_�[�^�[�Q�b�g
	std::shared_ptr<Render>ren;

	// �[�x�X�e���V��
	std::shared_ptr<Depth>dep;

	// �萔�o�b�t�@
	std::shared_ptr<Constant>con;

	// ���[�g�V�O�l�`��
	std::shared_ptr<Root>drwRoot;
	std::shared_ptr<Root>texRoot;

	// �p�C�v���C��
	std::shared_ptr<Pipe>pntPipe;
	std::shared_ptr<Pipe>linPipe;
	std::shared_ptr<Pipe>triPipe;
	std::shared_ptr<Pipe>texPipe;

	// �|�C���g
	std::shared_ptr<Point>pnt;

	// ���C��
	std::shared_ptr<Line>lin;

	// �g���C�A���O��
	std::shared_ptr<Triangle>tri;

	// �e�N�X�`�����[�_�[
	std::shared_ptr<TextureLoader>texLoad;

	// �e�N�X�`��
	std::shared_ptr<Texture>tex;
};
