#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <map>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;


namespace pmd {
#pragma pack(1)
	// ヘッダー
	struct Header {
		//タイプ
		unsigned char type[3];
		//バージョン
		float ver;
		//名前
		unsigned char name[20];
		//コメント
		unsigned char comment[256];
	};
#pragma pack()

	// 頂点データ
	struct Vertex {
		//座標
		DirectX::XMFLOAT3 pos;
		//法線
		DirectX::XMFLOAT3 normal;
		//UV
		DirectX::XMFLOAT2 uv;
		//ボーン番号
		unsigned short bornNo[2];
		//ウェイト
		unsigned char weight;
		//輪郭線フラグ
		unsigned char edge;
	};

#pragma pack(1)
	// マテリアル
	struct Material {
		//基本色
		DirectX::XMFLOAT3 diffuse;
		//透明度
		float alpha;
		//反射強度
		float specularity;
		//反射色
		DirectX::XMFLOAT3 specula;
		//環境色
		DirectX::XMFLOAT3 mirror;
		//トゥーン番号
		unsigned char toonIndex;
		//輪郭線フラグ
		unsigned char edge;
		//インデックス数
		unsigned int indexNum;
		//テクスチャパス
		unsigned char texPath[20];
	};
#pragma pack()

	// ボーン
	struct Born {
		//名前
		char name[20];
		//親ボーン番号
		unsigned short pIndex;
		//子ボーン番号
		unsigned short cIndex;
		//タイプ
		unsigned char type;
		//IK親ボーン番号
		unsigned short IKpIndex;
		//座標
		DirectX::XMFLOAT3 pos;
	};

	// IKデータ
	struct IkBorn {
		//IKボーンインデックス
		unsigned short index;
		//ターゲットボーン
		unsigned short target;
		//IKのチェインの長さ
		unsigned char chain;
		//
		unsigned short iteration;
		//ウェイト
		unsigned int weight;
		//子ボーンのインデックス
		std::vector<unsigned short>child;
	};

	// ボーンノード
	struct BornNode {
		//配列番号
		unsigned int index;
		//ボーン始点
		DirectX::XMFLOAT3 start;
		//子ボーン
		std::vector<BornNode*>child;
	};

	// マテリアル
	struct Mat {
		//基本色
		DirectX::XMFLOAT3 diffuse;
		//透明度
		float alpha;
		//反射強度
		float specularity;
		//反射色
		DirectX::XMFLOAT3 specula;
		//環境色
		DirectX::XMFLOAT3 mirror;
		//乗算テクスチャフラグ
		int sphFlag;
		//加算テクスチャフラグ
		int spaFlag;
		//テクスチャフラグ
		int texFlag;
	};
};

namespace vmd {
	// モーションデータ
	struct Motion {
		//フレーム
		unsigned long flam;
		//位置
		DirectX::XMFLOAT3 pos;
		//クォータニオン
		DirectX::XMFLOAT4 rotation;
		//補完
		DirectX::XMFLOAT2 a;
		DirectX::XMFLOAT2 b;
	};
};
