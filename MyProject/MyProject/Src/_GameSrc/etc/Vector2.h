#pragma once
#include <cmath>

template<typename T>
class Vector2
{
public:
	// �R���X�g���N�^
	Vector2(){}
	Vector2(const T& x, const T& y) :
	x(x), y(y){}
	Vector2(const T& i){
		Vector2(i, i);
	}
	Vector2(const Vector2& vec) {
		Vector2(vec.x, vec.y);
	}

	// �v�f
	T x;
	T y;

	// �I�[�o�[���[�h
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
		x = (i != (T)0) ? x / i : x;
		y = (i != (T)0) ? y / i : y;
	}

	void operator=(const Vector2& vec) {
		x = vec.x; y = vec.y;
	}
	void operator+(const Vector2& vec) {
		x += vec.x; y += vec.y;
	}
	void operator-(const Vector2& vec) {
		x -= vec.x; y -= vec.y;
	}
	void operator*(const Vector2& vec) {
		x *= vec.x; y *= vec.y;
	}
	void operator/(const Vector2& vec) {
		x = (vec.x != (T)0) ? x / vec.x : x;
		y = (vec.y != (T)0) ? y / vec.y : y;
	}

	bool operator==(const T& i) {
		return (x == i && y == i);
	}
	bool operator!=(const T& i) {
		return !(x == i && y == i);
	}

	bool operator==(const Vector2& vec) {
		return (x == vec.x && y == vec.y);
	}
	bool operator!=(const Vector2& vec) {
		return !(x == vec.x && y == vec.y);
	}

	// �֗��֐�
	// �x�N�g���̒���
	T Length(void) const {
		return (T)(std::sqrt((double)LengthSquare()));
	}

	// �x�N�g���̒�����2��
	T LengthSquare(void) const {
		return Dot(*this);
	}

	// ����
	T Dot(const Vector2& vec) const {
		return (x * vec.x + y * vec.y);
	}

	// ����
	T Distance(const Vector2& vec) const {
		return (vec - *this).Length();
	}

	// �Ȃ��p
	T Radian(const Vector2& vec) const {
		return (T)(std::acos((double)(Dot(vec) / (Length() * vec.Length()))));
	}

	// �Ȃ��p
	T Angle(const Vector2& vec) const {
		return (T)(((float)Radian(vec) * 180.0f / 3.14159565f));
	}

	// ���K��
	T Normalize(void) const {
		return (*this / Length());
	}
};

typedef Vector2<int>   Vec2;
typedef Vector2<float> Vec2f;
