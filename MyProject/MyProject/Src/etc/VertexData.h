#pragma once
#include <DirectXMath.h>

namespace draw {
	struct Vertex {
		//座標
		DirectX::XMFLOAT3 pos;
		//色
		DirectX::XMFLOAT4 color;
	};
}

namespace tex {
	struct Vertex {
		//座標
		DirectX::XMFLOAT3 pos;
		//UV
		DirectX::XMFLOAT2 uv;
		//アルファ値
		float alpha;
	};
}
