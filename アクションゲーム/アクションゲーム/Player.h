#pragma once
#include "Typedef.h"
#include "Input.h"
#include "Camera.h"
#include <vector>
#include <map>
#include <memory>

class Game;

class Player
{
	friend Game;
public:
	// �R���X�g���N�^
	Player(std::weak_ptr<Input>in, std::weak_ptr<Camera>cam);
	// �f�X�g���N�^
	virtual ~Player();

	// ���X�^�[�X�N���[��
	void RasterScroll(int image, const Vector2D& pos, const Vector2D& rect, const Vector2D& size, float expansion = 1.0f, float rotation = 0.0f, float period = 5.0f, float vibration = 5.0f, bool trans = true, bool xturn = false, bool yturn = false);

	// �`��
	void Draw(void);

	// ����
	void UpData();

	// ���S���W�̃Z�b�g
	void SetCenter(Position& pos, bool r = false);
	// ��Ԃ̃Z�b�g
	void SetMode(std::string m, bool r = false);
	// ��Ԃ̎擾
	std::string GetMode(void);

	// ���W�̎擾
	Positionf GetPos(void);
	Positionf GetCamPos(void);
	// ���W�̃Z�b�g
	void SetPos(Positionf pos);
	void SetPos(float y);

	// ��Ԉꗗ�̎擾
	std::vector<std::string> GetAllMode(void);

	// ���]�t���O�̎擾
	bool GetReverse(void);

	// �������`�̐�
	int GetAttackNum(void);
	// �������`�̎擾
	Attack GetAttack(USHORT num);
	// ��������W�̎擾
	Positionf GetAttackPos(USHORT num, bool flag = false);
	// �������̎擾
	CutData GetCut(void);

	void SetBlock(bool b);
	bool GetBlock(void);

	void SetVel(Vector2Df v);
	void SetDamagePW(float pw);

	bool GetMuteki(void);

	int GetHp(void);

	bool GetDie(void);

	bool GetClear(void);

private:
	// ������̌���
	std::string FindString(const std::string path, const char find, int offset = 0, bool start = true);
	// �ǂݍ���
	void Load(void);

	// �ҋ@�̏���
	void Wait(void);
	// �����̏���
	void Walk();
	// �W�����v�̏���
	void Jump(void);
	// ���n�̏���
	void Ground(void);
	// �p���`�̏���
	void Punch(void);
	// �L�b�N�̏���
	void Kick(void);
	// �X���C�f�B���O�̏���
	void Sliding(void);
	// ���Ⴊ�݂̏���
	void Down(void);
	// �o��̏���
	void Climb(void);
	// �_���[�W�̏���
	void Damage(void);
	// ���S�̏���
	void Die(void);


	// �C���v�b�g�N���X
	std::weak_ptr<Input>in;

	// �J�����N���X
	std::weak_ptr<Camera>cam;

	// �摜�f�[�^
	int image;

	std::map<std::string, int>sound;

	// �t���[����
	UINT flam;

	// ���W
	Positionf pos;

	// ���[�h�z��
	std::map<std::string, ImageData>data;

	// ���
	std::vector<std::string>fmode;

	// �����f�[�^
	std::map<std::string, std::vector<CutData>>cut;

	// �������`�̃f�[�^
	std::map < std::string, std::map<int, std::vector<Attack>>>attack;

	// �z��ԍ�
	UINT index;

	// ���
	std::string mode;

	// ���S
	Position center;

	// ���]�t���O
	bool reverse;

	// ���[�v�t���O
	bool loop;

	// ���x
	Vector2Df vel;

	// �֐��|�C���^
	void (Player::*func)(void);

	// �ҋ@�t���O
	bool wait;

	// �W�����v�t���O
	bool fly;

	// ��`�T�C�Y
	int attackSize;

	// �C�����W
	Positionf camPos;

	bool bl;

	float d;

	bool m;

	int mTime;

	// �̗�
	int hp;

	bool die;

	bool clear;
};

