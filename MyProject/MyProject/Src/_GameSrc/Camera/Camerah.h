#pragma once
#include "../etc/Vector2.h"
#include <memory>

class Obj;

class Camera
{
public:
	// �R���X�g���N�^
	Camera();
	// �f�X�g���N�^
	~Camera();

	// �^�[�Q�b�g�̃Z�b�g
	void SetTarget(std::weak_ptr<Obj>obj);

	// ����
	void UpData(const Vec2f& area);

	// ���[�J�����W�ɕϊ�
	Vec2f ChangeLocal(const Vec2f& pos);

	// ���W�̎擾
	Vec2f GetPos(void) const {
		return pos;
	}

private:
	// ���W�̔�r
	void ComparisonPos(float & pos, const float & areaSize, const int & winSize);


	// �^�[�Q�b�g�I�u�W�F�N�g
	std::weak_ptr<Obj>obj;

	// ���W
	Vec2f pos;
};
