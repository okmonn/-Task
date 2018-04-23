#pragma once

// ��ʃT�C�Y
#define WINDOW_X 768
#define WINDOW_Y 448

// �񎟌����W
template<typename T>
struct Vector2
{
	T x;
	T y;

	Vector2 operator+(const Vector2& vec)
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
typedef Vector2<int>Position;

struct Rect
{
	//���S���W
	Position center;

	//����
	int width;

	// ����
	int height;

	Rect() : center({0,0}), width(0), height(0)
	{}
	Rect(int x, int y, int w, int h) : center({ x, y }), width(w), height(h)
	{}
	Rect(Position& pos, int w, int h) : center(pos), width(w), height(h)
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
		width = w;
		height = h;
		center = { (x + width / 2), (y + height / 2) };
	}
	
	//���S�ʒu�̎擾
	Vector2D GetCenter(void)
	{
		return center;
	}

	//�����W�̎擾
	int GetLeft(void)
	{
		return center.x - width / 2;
	}
	//����W�̎擾
	int GetTop(void)
	{
		return center.y - height / 2;
	}
	//�E���W�̎擾
	int GetRight(void)
	{
		return center.x + width / 2;
	}
	//�����W�̎擾
	int GetBottom(void)
	{
		return center.y + height / 2;
	}
	//�����̎擾
	int GetWidth(void)
	{
		return width;
	}
	//�c���̎擾
	int GetHeight(void)
	{
		return height;
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