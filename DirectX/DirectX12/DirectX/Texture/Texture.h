#pragma once
#include "../Descriptor/Descriptor.h"
#include "../../etc/Vector2.h"
#include "../../etc/Typedef.h"
#include <vector>
#include <map>
#include <string>

class Texture :
	public Descriptor
{
	// 定数バッファ
	struct Con {
		//ヒープ
		ID3D12DescriptorHeap* heap;
		//リソース
		ID3D12Resource* resource;
	};

	// WICデータ
	struct WIC {
		//定数バッファ
		Con con;
		//頂点データ
		std::vector<Vertex>vertex;
		//リソース
		ID3D12Resource* resource;
		//デコード
		std::unique_ptr<uint8_t[]>decode;
		//サブ
		D3D12_SUBRESOURCE_DATA sub;
		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW view;
		//送信データ
		UINT8* data;
	};

public:
	// コンストラクタ
	Texture(std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~Texture();

	// WIC読み込み
	HRESULT LoadWIC(UINT& index, const std::string& fileName);

	// 描画
	void Draw(UINT& index, const Vec2f& pos, UINT turnX = 0, UINT turnY = 0);

	// 描画・サイズ指定
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, UINT turnX = 0, UINT turnY = 0);

	// 描画・サイズ指定・分割
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, const Vec2f& rectPos, const Vec2f& rectSize, UINT turnX = 0, UINT turnY = 0);

private:
	// 定数バッファヒープの生成
	HRESULT CreateHeap(UINT* index);

	// シェーダリソースビューの生成
	HRESULT CreateShaderView(UINT* index);

	// 頂点リソースの生成
	HRESULT CreateResource(UINT* index);


	// WICデータ
	std::map<UINT*, WIC>wic;
};

namespace func {
	// ユニコード変換
	std::wstring ChangeUnicode(const CHAR* str);
}
