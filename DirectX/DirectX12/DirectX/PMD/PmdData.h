#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <map>

// 頂点バッファ
struct VertexData {
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW view;
	//リソース
	ID3D12Resource* resource;
	//送信データ
	UINT* data;
};

// 頂点インデックス
struct VertexIndex {
	//頂点インデックスビュー
	D3D12_INDEX_BUFFER_VIEW view;
	//リソース
	ID3D12Resource* resource;
	//送信データ
	UINT* data;
};

// 定数バッファ
struct Constant {
	//ヒープ
	ID3D12DescriptorHeap* heap;
	//リソース
	ID3D12Resource* resource;
	//送信データ
	UINT8* data;
};

#pragma pack(1)
// ヘッダー
struct Header {
	//タイプ
	UCHAR type[3];
	//バージョン 
	FLOAT ver;
	//名前
	UCHAR name[20];
	//コメント
	UCHAR comment[256];
	//頂点数
	UINT vertexNum;
};
#pragma pack()

// 頂点データ
struct PmdVertex {
	//座標
	DirectX::XMFLOAT3 pos;
	//法線
	DirectX::XMFLOAT3 normal;
	//UV
	DirectX::XMFLOAT2 uv;
	//ボーン番号
	USHORT bornNo[2];
	//ウェイト
	UCHAR weight;
	//輪郭線フラグ
	UCHAR edge;
};

#pragma pack(1)
// マテリアル
struct Material {
	//基本色
	DirectX::XMFLOAT3 diffuse;
	//透明度
	FLOAT alpha;
	//反射強度
	FLOAT specularity;
	//反射色
	DirectX::XMFLOAT3 specula;
	//環境色
	DirectX::XMFLOAT3 mirror;
	//トゥーン番号
	UCHAR toonIndex;
	//輪郭線フラグ
	UCHAR edge;
	//インデックス数
	UINT indexNum;
	//テクスチャパス
	CHAR texPath[20];
};
#pragma pack()

// ボーン
struct Born {
	//名前
	CHAR name[20];
	//親ボーン番号
	WORD pIndex;
	//子ボーン番号
	WORD cIndex;
	//タイプ
	BYTE type;
	//IK親ボーン番号
	WORD IKpIndex;
	//座標
	DirectX::XMFLOAT3 pos;
};

// ボーン座標
struct BornPos {
	//先頭座標
	DirectX::XMFLOAT3 head;
	//末尾座標
	DirectX::XMFLOAT3 tail;
};

// モデルデータ
struct Model {
	//頂点バッファ
	VertexData v;
	//頂点インデックス
	VertexIndex i;
	//定数バッファ
	Constant c;

	//ヘッダー
	Header header;
	//頂点データ
	std::vector<PmdVertex>vertex;
	//頂点インデックス
	std::vector<USHORT>index;
	//マテリアル
	std::vector<Material>material;
	//ボーン
	std::vector<Born>born;
	//ボーンノード
	std::vector<std::vector<USHORT>>node;
	//ボーン座標
	std::vector<BornPos>pos;
	//テクスチャアドレス
	std::vector<UINT>id;
};
