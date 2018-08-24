#pragma once
#include <DirectXMath.h>

// 円周率
#define PI 3.14159265359f

// ラジアン変換
#define RAD(X) (X) * PI / 180.0f

// 頂点
struct Vertex {
	//座標
	DirectX::XMFLOAT3 pos;
	//UV
	DirectX::XMFLOAT2 uv;
};

// 空間行列
struct WVP
{
	//ワールド
	DirectX::XMMATRIX world;
	//ビュー
	DirectX::XMMATRIX view;
	//プロジェクション
	DirectX::XMMATRIX projection;
};
