#pragma once
#include "../etc/tstring.h"
#include "../etc/Vector2.h"
#include "../etc/Vector3.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

namespace fs = std::experimental::filesystem;

class Window;
class Xaudio2;
class MIDI_IN;
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
class Compiler;
class Pipe;
class Constant;
class Point;
class Line;
class Box;
class Texture;
class VMD;
class PMD;

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

	// �r���[�s��̃Z�b�g
	void ChangeView(const Vec3f& pos, const Vec3f& target, const Vec3f& up);

	// �`�揀��
	void Set(void);

	// ���s
	void Do(void);

	// �L�[����
	bool CheckKey(UINT index);

	// �g���K�[�L�[����
	bool TriggerKey(UINT index);

	// ���l�̕ύX
	void SetAlpha(float alpha) {
		this->alpha = alpha;
	}

	// �_�̕`��
	void DrawPoint(const Vec2f& pos, const Vec3f& color);

	// ���C���̕`��
	void DrawLine(const Vec2f& pos1, const Vec2f& pos2, const Vec3f& color);

	// �{�b�N�X�̕`��
	void DrawBox(const Vec2f& pos, const Vec2f& size, const Vec3f& color);

	// �摜�ǂݍ���
	void LoadImg(UINT& index, const std::string& fileName);

	// �`��
	void Draw(UINT& index, const Vec2f& pos, UINT turnX, UINT turnY);

	// �`��E�T�C�Y�w��
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, UINT turnX, UINT turnY);

	// �`��E�T�C�Y�w��E����
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, const Vec2f& rect, const Vec2f& rectSize, UINT turnX, UINT turnY);

	// �`��E���R�ό`
	void FreelyDraw(UINT& index, const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Vec2f& pos4, UINT turnX, UINT turnY);

	// �`��E�����E���R�ό`
	void FreelyDraw(UINT& index, const Vec2f& rectPos, const Vec2f& rectSize, const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Vec2f& pos4, UINT turnX, UINT turnY);

	// �摜�̏���
	void DeleteImg(UINT& index);

	// VMD�ǂݍ���
	void LoadVMD(UINT& index, const std::string& fileName);

	// PMD�ǂݍ���
	void LoadPMD(UINT& index, const std::string& fileName);

	// PMD�`��
	void DrawPMD(UINT& index);

	// PMD�̃{�[����]
	void RotateBorn(UINT& index, const std::string& name, const DirectX::XMMATRIX& matrix);
	void RotateBorn(UINT& model, UINT& motion);

	// PMD�̏���
	void DeletePMD(UINT& index);

	// WAVE�ǂݍ���
	void LoadWave(UINT& index, const std::string& fileName);

	// WAVE�̍Đ�
	void PlayWave(UINT& index, bool loop);

	// WAVE�̍Đ���~
	void StopWave(UINT& index);

	// WAVE�̏���
	void DeleteWAVE(UINT& index);

	// MIDI�f�o�C�X���̎擾
	UINT GetMidiNum(void);

	// MIDI�X�e�[�^�X�o�C�g�̎擾
	UCHAR GetMidiState(void);

	// MIDI�f�[�^�o�C�g1�̎擾
	UCHAR GetMidiData1(void);

	// MIDI�f�[�^�o�C�g2�̎擾
	UCHAR GetMidiData2(void);

	// �f�B���N�g���̃t�@�C����
	std::vector<std::tstring> GetDirFile(const std::tstring& point);

private:
	// �R���X�g���N�^
	Union();
	Union(const Union&) {
	}
	void operator=(const Union&) {
	}

	// ���[�g�V�O�l�`���̐���
	void CreateRoot(void);

	// �p�C�v���C���̐���
	void CreatePipeLine(void);

	// �r���[�|�[�g�̃Z�b�g
	void ViewPort(void);

	// �V�U�[�̃Z�b�g
	void Scissor(void);

	// �o���A�̃Z�b�g
	void Barrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter);

	// �t�@�C����Ԃ�
	std::tstring GetFile(const fs::path& p);


	// ��ʃT�C�YX
	UINT x;

	// ��ʃT�C�YY
	UINT y;

	// �A���t�@�l
	float alpha;

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

	// �I�[�f�B�I
	std::shared_ptr<Xaudio2>audio;

	// MIDI����
	std::shared_ptr<MIDI_IN>in;

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
	std::shared_ptr<Root>pointRoot;
	std::shared_ptr<Root>texRoot;
	std::shared_ptr<Root>modelRoot;

	// �V�F�[�_�R���p�C��
	std::shared_ptr<Compiler>com;

	// �p�C�v���C��
	std::shared_ptr<Pipe>pointPipe;
	std::shared_ptr<Pipe>linePipe;
	std::shared_ptr<Pipe>boxPipe;
	std::shared_ptr<Pipe>texPipe;
	std::shared_ptr<Pipe>modelPipe;

	// �萔�o�b�t�@
	std::shared_ptr<Constant>constant;

	// �_
	std::shared_ptr<Point>point;

	// ���C��
	std::shared_ptr<Line>line;

	// �{�b�N�X
	std::shared_ptr<Box>box;

	// �e�N�X�`��
	std::shared_ptr<Texture>tex;

	// VMD
	std::shared_ptr<VMD>vmd;

	// PMD
	std::shared_ptr<PMD>pmd;
};