#pragma once
#include "../etc/Vector2.h"
#include <vector>

class MapEditor
{
public:
	// �R���X�g���N�^
	MapEditor();
	// �f�X�g���N�^
	~MapEditor();

	// �}�b�v���̃Z�b�g
	void MapInfo(const Vec2f& mapSize, const Vec2& chipCnt);

	// �`��
	void Draw(void);
	
	// ����
	void UpData(void);

private:
	// �J�[�\�����W
	Vec2f pos;

	// �J�[�\���T�C�Y
	Vec2f size;

	// �}�b�v�̃T�C�Y
	Vec2f mapSize;

	// �}�b�v�`�b�v�̐�
	Vec2 chipCnt;

	// �}�b�v�`�b�v�̃C���f�b�N�X
	unsigned int index;

	// �}�b�v�f�[�^
	std::vector<char> map;
};
