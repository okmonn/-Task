#pragma once

// 画面サイズ
#define WINDOW_X 768
#define WINDOW_Y 448

// 二次元座標
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
	//中心座標
	Position center;

	//横幅
	int width;

	// 立幅
	int height;

	Rect() : center({0,0}), width(0), height(0)
	{}
	Rect(int x, int y, int w, int h) : center({ x, y }), width(w), height(h)
	{}
	Rect(Position& pos, int w, int h) : center(pos), width(w), height(h)
	{}
	//中心位置の更新
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
	
	//中心位置の取得
	Vector2D GetCenter(void)
	{
		return center;
	}

	//左座標の取得
	int GetLeft(void)
	{
		return center.x - width / 2;
	}
	//上座標の取得
	int GetTop(void)
	{
		return center.y - height / 2;
	}
	//右座標の取得
	int GetRight(void)
	{
		return center.x + width / 2;
	}
	//下座標の取得
	int GetBottom(void)
	{
		return center.y + height / 2;
	}
	//横幅の取得
	int GetWidth(void)
	{
		return width;
	}
	//縦幅の取得
	int GetHeight(void)
	{
		return height;
	}

	//描画
	void Draw(unsigned int color = 0xffffffff)
	{

	}
	//描画
	void Draw(const Vector2D& offset, unsigned int color = 0xffffffff)
	{

	}
};