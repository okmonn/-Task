#pragma once
#include "Scene.h"

class Teach :
	public Scene
{
public:
	// �R���X�g���N�^
	Teach();
	// �f�X�g���N�^
	~Teach();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// ���f��
	int model;

	// WVP
	float x = 0.f;
	float y = 10.f;
	float z = -15.f;
};
