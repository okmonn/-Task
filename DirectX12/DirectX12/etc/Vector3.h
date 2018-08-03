#pragma once
#include <complex>

/* ３次元ベクトル */
template<typename T>
class Vector3
{
public:
	// X成分
	T x;

	// Y成分
	T y;

	// Z成分
	T z;

	// コンストラクタ
	Vector3() {
	};
	// コンストラクタ
	constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {
	};

	// ベクトルの長さ
	T Length(void) const {
		return (T)std::sqrt(LengthSquare());
	}

	// ベクトルの長さの2乗
	constexpr T LengthSquare(void) const {
		return (Dot(*this));
	}

	// ベクトルとの内積
	constexpr T Dot(const Vector3& vec) const {
		return (x * vec.x + y * vec.y + z * vec.z);
	}

	// ベクトルとの外積
	constexpr Vector3 Cross(const Vector3& vec) const {
		return { (y * vec.z) - (z * vec.y), (z * vec.x - x * vec.z), (x * vec.y - y * vec.x) };
	}

	// ベクトルとの距離
	T Distance(const Vector3& vec) const {
		return (vec - *this).Length();
	}

	// ベクトルとのラジアン角度
	T Radian(const Vector3& vec) const {
		return (T)std::acos(Dot(vec) / (Length() * vec.Length()));
	}

	// ベクトルとの弧度法角度
	T Angle(const Vector3& vec) const {
		return (T)((float)Radian(vec) * 180.0f / 3.141592f);
	}

	// 正規化
	Vector3 Normalized(void) const {
		return (*this / Length());
	}

	// ゼロ確認
	constexpr bool IsZero(void) const {
		return (x == (T)0 && y == (T)0 && z == (T)0);
	}

	// デストラクタ
	~Vector3() {
	};

	// オペレータオーバーロード
	constexpr Vector3 operator+(void) const {
		return *this;
	}
	constexpr Vector3 operator-(void) const {
		return { -x, -y, -z };
	}
	constexpr Vector3 operator+(const Vector3& vec) const {
		return { x + vec.x, y + vec.y, z + vec.z };
	}
	constexpr Vector3 operator+(const T i) const {
		return { x + i, y + i, z + i };
	}
	constexpr Vector3 operator-(const Vector3& vec) const {
		return { x - vec.x, y - vec.y, z - vec.z };
	}
	constexpr Vector3 operator-(const T i) const {
		return { x - i, y - i, z - i };
	}
	constexpr Vector3 operator*(const Vector3& vec) const {
		return { x * vec.x, y * vec.y, z * vec.z };
	}
	constexpr Vector3 operator*(const T i) const {
		return { x * i, y * i, z * i };
	}
	constexpr Vector3 operator/(const Vector3& vec) const {
		return { x / vec.x, y / vec.y, z / vec.z };
	}
	constexpr Vector3 operator/(const T i) const {
		return { x / i, y / i, z / i };
	}
	void operator=(const Vector3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	void operator=(const T i) {
		x = i;
		y = i;
		z = i;
	}
	void operator+=(const Vector3& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}
	void operator+=(const T i) {
		x += i;
		y += i;
		z += i;
	}
	void operator-=(const Vector3& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}
	void operator-=(const T i) {
		x -= i;
		y -= i;
		z -= i;
	}
	void operator*=(const Vector3& vec) {
		x *= vec.x;
		y *= vec.y;
		z *= vec.z;
	}
	void operator*=(const T i) {
		x *= i;
		y *= i;
		z *= i;
	}
	void operator/=(const Vector3& vec) {
		x /= vec.x;
		y /= vec.y;
		z /= vec.z;
	}
	void operator/=(const T i) {
		x /= i;
		y /= i;
		z /= i;
	}
	constexpr bool operator==(const Vector3& vec) const {
		return (x == vec.x && y == vec.y && z == vec.z);
	}
	constexpr bool operator==(const T i) const {
		return (x == i && y == i && z == i);
	}
	constexpr bool operator!=(const Vector3& vec) const {
		return !(x == vec.x && y == vec.y && z == vec.z);
	}
	constexpr bool operator!=(const T i) const {
		return !(x == i && y == i && z == i);
	}
};

template<typename T>
inline constexpr Vector3<T> operator+(T i, Vector3<T>& vec) {
	return { vec.x + i, vec.y + i, vec.z + i };
}

template<typename T>
inline constexpr Vector3<T> operator-(T i, Vector3<T>& vec) {
	return { vec.x - i, vec.y - i, vec.z - i };
}

template<typename T>
inline constexpr Vector3<T> operator*(T i, Vector3<T>& vec) {
	return { vec.x * i, vec.y * i, vec.z * i };
}

template<typename T>
inline constexpr Vector3<T>operator/(T i, Vector3<T>& vec) {
	return { vec.x / i, vec.y / i, vec.z / i };
}

// Vector3<int>の型変換
typedef Vector3<int>Vec3;
// Vector3<float>の型変換
typedef Vector3<float>Vec3f;