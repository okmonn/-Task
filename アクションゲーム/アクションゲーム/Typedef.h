#pragma once
#include <string>

// ��ʃT�C�Y
#define WINDOW_X 768
#define WINDOW_Y 448

// �񎟌����W
template<typename T>
struct Vector2
{
	Vector2() : x(0), y(0) {};
	Vector2(T x, T y) : x(x), y(y) {};

	T x;
	T y;

	Vector2 operator+(const Vector2& vec) const
	{
		Vector2<T> tmp;
		tmp.x = x + vec.x;
		tmp.y = y + vec.y;
		return tmp;
	}

	void operator+=(const T a)
	{
		x += a;
		y += a;
	}
};
typedef Vector2<int>Vector2D;
typedef Vector2D Position;
typedef Vector2<float>Vector2Df;
typedef Vector2Df Positionf;

//��
struct Size
{
	//����
	int width;
	//�c��
	int height;
};

struct Rect
{
	//���S���W
	Position center;

	//��
	Size size;

	Rect() : center({ 0,0 }), size({0,0})
	{}
	Rect(int x, int y, int w, int h) : center({ x, y }), size({ w,h })
	{}
	Rect(Position& pos, int w, int h) : center(pos), size({ w,h })
	{}
	//���S�ʒu�̍X�V
	void SetCenter(int x, int y)
	{
		center = { x, y };
	}
	void SetCenter(Position& pos)
	{
		center = pos;
	}

	//
	void SetLeftTopWidthHeight(int x, int y, int w, int h)
	{
		size = { w, h };
		center = { (x + size.width / 2), (y + size.height / 2) };
	}
	
	//���S�ʒu�̎擾
	Vector2D GetCenter(void)
	{
		return center;
	}

	//�����W�̎擾
	int GetLeft(void)
	{
		return center.x - size.width / 2;
	}
	//����W�̎擾
	int GetTop(void)
	{
		return center.y - size.height / 2;
	}
	//�E���W�̎擾
	int GetRight(void)
	{
		return center.x + size.width / 2;
	}
	//�����W�̎擾
	int GetBottom(void)
	{
		return center.y + size.height / 2;
	}
	//�����̎擾
	int GetWidth(void)
	{
		return size.width;
	}
	//�c���̎擾
	int GetHeight(void)
	{
		return size.height;
	}

	//�`��
	void Draw(unsigned int color = 0xffffffff)
	{

	}
	//�`��
	void Draw(const Vector2D& offset, unsigned int color = 0xffffffff)
	{

	}
};

//�����f�[�^
struct CutData
{
	//�؂蔲���Z�`
	Rect rect;
	//���S���W
	Position center;
	//�A�j���[�V��������
	int flam;
};
// �摜�w�b�_�[�\����
struct ImageHeader
{
	//������̒���
	int pathNum;
	//�t�@�C���p�X
	std::string path;
	//�f�[�^��
	int dataNum;
};
// �摜�f�[�^�\����
struct ImageData
{
	//������̒���
	int nameNum;
	//�A�N�V������
	std::string name;
	//�A�j���[�V������
	int animCnt;
};