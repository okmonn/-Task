#pragma once
#include "../Obj.h"
#include "../../etc/Vector2.h"
#include "../../etc/Typedef.h"
#include <memory>
#include <map>
#include <string>

class Window;
class Device;
class List;
class Swap;

// 頂点データの最大数
#define VERTEX_MAX 4

class Texture :
	public Obj
{
	// 定数バッファ
	struct Constant {
		//ヒープ
		ID3D12DescriptorHeap* heap;
		//リソース
		ID3D12Resource* resource;
		//送信データ
		UINT8* data;
	};

	// 画像データ
	struct Image {
		//ウィンドウサイズ
		DirectX::XMFLOAT2 window;
	};

	// 頂点データ
	struct VertexData {
		//頂点
		Vertex vertex[VERTEX_MAX];
		//リソース
		ID3D12Resource* resource;
		//送信データ
		UINT* data;
		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW view;
	};

	// WICデータ
	struct WIC {
		//頂点データ
		VertexData v;
		//ヒープ
		ID3D12DescriptorHeap* heap;
		//リソース
		ID3D12Resource* resource;
		//デコード
		std::unique_ptr<uint8_t[]>decode;
		//サブ
		D3D12_SUBRESOURCE_DATA sub;
	};

public:
	// コンストラクタ
	Texture(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// デストラクタ
	~Texture();

	// WIC読み込み
	HRESULT LoadWIC(UINT& index, std::string fileName);

	// 描画準備
	void SetDraw(UINT* index);

	// ヒープのセット
	void SetHeap(UINT table = 1);
	void SetHeap(UINT* index, UINT table = 2);

	// 描画
	void Draw(UINT& index);
	// 描画
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size);

private:
	// ヒープの生成
	HRESULT CreateHeap(void);

	// リソースの生成
	HRESULT CreateResource(void);

	// 定数バッファビューの生成
	HRESULT CreateConstantView(void);

	// ヒープの生成
	HRESULT CreateHeap(UINT* index);
	
	// リソースの生成
	HRESULT CreateResourceView(UINT* index);

	// 頂点バッファの生成
	HRESULT CreateVertex(UINT* index);

	// 解放
	void Liberation(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// 定数バッファ
	Constant constant;

	// 画像データ
	Image image;

	// WICデータ
	std::map<UINT*, WIC>wic;
};

namespace func {
	// ユニコード変換
	std::wstring ChangeUnicode(const CHAR* str);
}

