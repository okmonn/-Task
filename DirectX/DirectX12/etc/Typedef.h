#pragma once
#include <DirectXMath.h>

// 円周率
#define PI 3.141592f

// ラジアン変換
#define RAD(X) (X) * PI / 180.0f

// 頂点
struct Vertex {
	//座標
	DirectX::XMFLOAT3 pos;
	//ＵＶ
	DirectX::XMFLOAT2 uv;
};

// 空間行列
struct WVP
{
	//ワールド
	DirectX::XMMATRIX world;
	//ビュープロジェクション
	DirectX::XMMATRIX viewProjection;
};

// 画像データ
struct Image {
	//ウィンドウサイズ
	DirectX::XMFLOAT2 window;
};
