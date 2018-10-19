#pragma once
#include "../etc/VertexData.h"
#include <vector>

namespace DirectX {
	struct TexMetadata;
	class ScratchImage;
}

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

// 画像データ
struct Tex {
	//メタデータ
	DirectX::TexMetadata* meta;
	//スクラッチイメージ
	DirectX::ScratchImage* img;

	//定数ヒープ
	ID3D12DescriptorHeap* heap;
	//定数リソース
	ID3D12Resource* c_rsc;

	//頂点データ
	std::vector<tex::Vertex>vertex;

	//頂点リソース
	ID3D12Resource* v_rsc;
	//送信データ
	unsigned int* data;
};
