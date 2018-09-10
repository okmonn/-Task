#pragma once
#include <DirectXMath.h>
#include <string>

// VMDヘッダー
struct VmdHeader {
	//ヘッダー
	char header[30];
	//モデル名
	char modelName[20];
};

#pragma pack(1)
// モーションデータ
struct VmdMotion {
	//ボーン名
	char bornName[15];
	//フレーム
	unsigned int flam;
	//位置
	DirectX::XMFLOAT3 pos;
	//クォータニオン
	DirectX::XMFLOAT4 rotation;
	//ベジェデータ
	unsigned char interpolation[64];
};
#pragma pack()

// データ
struct Motion {
	// ボーン名
	std::string name;
	// クォータニオン
	DirectX::XMVECTOR quaternion;
};