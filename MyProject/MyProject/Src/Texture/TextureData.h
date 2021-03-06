#pragma once
#include "../etc/VertexData.h"
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct D3D12_SUBRESOURCE_DATA;

namespace texture {
	// 画像データ
	struct Tex {
		// デコードデータ
		unsigned char* decode;
		// サブデータ
		std::weak_ptr<D3D12_SUBRESOURCE_DATA>sub;

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

	// 色
	struct Color {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		Color() :r(0), g(0), b(0), a(0) {
		}
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
			r(r), g(g), b(b), a(a) {
		}
	};

	// ダミーテクスチャ
	struct Dummy {
		//ヒープ
		ID3D12DescriptorHeap* heap;
		//リソース
		ID3D12Resource* rsc;
		//画像データ
		std::vector<Color>data;
	};
}
