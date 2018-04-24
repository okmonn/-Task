#pragma once
#include <string>

// 画面サイズ
#define WINDOW_X 768
#define WINDOW_Y 448

// 二次元座標
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

//幅
struct Size
{
	//横幅
	int width;
	//縦幅
	int height;
};

struct Rect
{
	//中心座標
	Position center;

	//幅
	Size size;

	Rect() : center({ 0,0 }), size({0,0})
	{}
	Rect(int x, int y, int w, int h) : center({ x, y }), size({ w,h })
	{}
	Rect(Position& pos, int w, int h) : center(pos), size({ w,h })
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
		size = { w, h };
		center = { (x + size.width / 2), (y + size.height / 2) };
	}
	
	//中心位置の取得
	Vector2D GetCenter(void)
	{
		return center;
	}

	//左座標の取得
	int GetLeft(void)
	{
		return center.x - size.width / 2;
	}
	//上座標の取得
	int GetTop(void)
	{
		return center.y - size.height / 2;
	}
	//右座標の取得
	int GetRight(void)
	{
		return center.x + size.width / 2;
	}
	//下座標の取得
	int GetBottom(void)
	{
		return center.y + size.height / 2;
	}
	//横幅の取得
	int GetWidth(void)
	{
		return size.width;
	}
	//縦幅の取得
	int GetHeight(void)
	{
		return size.height;
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

//分割データ
struct CutData
{
	//切り抜き短形
	Rect rect;
	//中心座標
	Position center;
	//アニメーション時間
	int flam;
};
// 画像ヘッダー構造体
struct ImageHeader
{
	//文字列の長さ
	int pathNum;
	//ファイルパス
	std::string path;
	//データ数
	int dataNum;
};
// 画像データ構造体
struct ImageData
{
	//文字列の長さ
	int nameNum;
	//アクション名
	std::string name;
	//アニメーション数
	int animCnt;
};