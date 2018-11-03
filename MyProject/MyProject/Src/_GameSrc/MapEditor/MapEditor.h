#pragma once
#include "../etc/Vector2.h"
#include <vector>
#include <string>

class MapEditor
{
	// �J�[�\��
	struct Cursor {
		//���[���h���W
		Vec2f pos;
		//���[�J�����W
		Vec2f local;
		//�T�C�Y
		Vec2f size;
		//�}�b�v�`�b�v�ԍ�
		unsigned int index;
	};

	// �J����
	struct Camera {
		//���W
		Vec2f pos;
	};

public:
	// �R���X�g���N�^
	MapEditor();
	// �f�X�g���N�^
	~MapEditor();

	// �}�b�v���̃Z�b�g
	void MapInfo(const Vec2f& mapSize, const Vec2& chipCnt, const Vec2& winSize = { 640, 480 });

	// �`��
	void Draw(void);
	
	// ����
	void UpData(void);

private:
	// �J�������W�̔�r
	void ComparisonPos(float& cam, const float& areaSize, const int& winSize);

	// �J�����̍X�V
	void UpDataCam(void);

	// ���[�J�����W�ɕϊ�
	Vec2f ChangeLocal(const Vec2f& pos);


	// �}�b�v�̃T�C�Y
	Vec2f mapSize;

	// �}�b�v�`�b�v�̐�
	Vec2 chipCnt;

	// �E�B���h�E�T�C�Y
	Vec2 winSize;

	// �J����
	Camera cam;

	// �J�[�\��
	Cursor cursor;

	// �^�C���Z�b�g�摜
	int tile;

	// �I�u�W�F�N�g�摜
	int obj;

	// �}�b�v�f�[�^
	std::vector<char> map;
};
