#pragma once
#include <DirectXMath.h>

// 円周率
#define PI 3.14159265359f

// ラジアン変換
#define RAD(X) (X) * PI / 180.0f

// ポイント頂点
struct PointVertex {
	//座標
	DirectX::XMFLOAT3 pos;
	//色
	DirectX::XMFLOAT4 color;
};

// テクスチャ頂点
struct TexVertex {
	//座標
	DirectX::XMFLOAT3 pos;
	//UV
	DirectX::XMFLOAT2 uv;
	//α値
	float alpha;
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
	// ウィンドウサイズ
	DirectX::XMFLOAT2 window;
};
