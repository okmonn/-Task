#pragma once
#include "../etc/Vector2.h"
#include <memory>

class Object;

class Camera
{
public:
	// �R���X�g���N�^
	Camera();
	// �f�X�g���N�^
	~Camera();

	// �^�[�Q�b�g�̃Z�b�g
	void SetTarget(std::weak_ptr<Object>obj);

	// ����
	void UpData(const Vec2f& area);

	// ���[�J�����W�ɕϊ�
	Vec2f ChangeLocal(const Vec2f& pos);

	// ���W�̎擾
	Vec2f GetPos(void) const {
		return pos;
	}
	// �g�嗦�̎擾
	float GetLarge(void) const {
		return large;
	}

private:
	// ���W�̔�r
	void ComparisonPos(float & pos, const float & areaSize, const int & winSize);


	// �^�[�Q�b�g�I�u�W�F�N�g
	std::weak_ptr<Object>obj;

	// ���W
	Vec2f pos;

	// �g�嗦
	float large;
};
