#pragma once
#include <DirectXMath.h>

namespace draw {
	struct Vertex {
		//���W
		DirectX::XMFLOAT3 pos;
		//�F
		DirectX::XMFLOAT4 color;
	};
}

namespace tex {
	struct Vertex {
		//���W
		DirectX::XMFLOAT3 pos;
		//UV
		DirectX::XMFLOAT2 uv;
		//�A���t�@�l
		float alpha;
	};
}
