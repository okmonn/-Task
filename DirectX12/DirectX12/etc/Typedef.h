#pragma once
#include <DirectXMath.h>

// ラジアン変換
#define RAD(X) (X) * DirectX::XM_PI / 180.0f
// 弧度法変換
#define ANGLE(X) (X) * 180.0f / DirectX::XM_PI

// ワールドビュープロジェクション
struct WVP {
	//ワールド
	DirectX::XMMATRIX world;
	//ビュープロジェクション
	DirectX::XMMATRIX viewProjection;
};
