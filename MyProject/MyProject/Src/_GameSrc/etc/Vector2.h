#pragma once

template<typename T>
class Vector2
{
public:
	// コンストラクタ
	Vector2(){}
	Vector2(const T& x, const T& y) :
	x(x), y(y){}
	Vector2(ocnst T& i){
		Vector2(i, i);
	}

	// 要素
	T x;
	T y;

	// オーバーロード
	void operator=(const T& i) {
		x = i; y = i;
	}
	void operator+(const T& i) {
		x += i; y += i;
	}
	void operator-(const T& i) {
		x -= i; y -= i;
	}
	void operator*(const T& i) {
		x *= i; y *= i;
	}
	void operator/(const T& i) {
		x = (i != 0) ? x / i : x;
		y = (i != 0) ? y / i : y;
	}

	void operator=(const Vector2& vec) {
		x = vec.x; y = vec.y;
	}
	void operator+(const Vector2& vec) {
		x += vec.x; y += vec.y;
	}
};
