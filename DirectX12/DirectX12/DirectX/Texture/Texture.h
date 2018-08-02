#pragma once
#include "../Obj.h"
#include "../../etc/Typedef.h"
#include <memory>
#include <map>

class Window;
class Device;
class List;
class Swap;

class Texture : 
	public Obj
{
	//頂点データ
	struct VertexData {
		//頂点
		Vertex vertex[4];
		//リソース
		ID3D12Resource* resource;
		//送信
		UINT* data;
		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW view;
	};

	//画像データ
	struct Image {
		//画面サイズ
		DirectX::XMFLOAT2 window;
		//画像サイズ
		DirectX::XMFLOAT2 size;
		//画像UV
		DirectX::XMFLOAT2 uv;
	};

	//定数バッファ
	struct Constant {
		//ヒープ
		ID3D12DescriptorHeap* heap;
		//リソース
		ID3D12Resource* resource;
		//送信データ
		UINT8* data;
		//サイズ
		UINT size;
	};

	//WICデータ
	struct WIC {
		//画像データ
		Image image;
		//頂点データ
		VertexData vertex;
		//定数バッファ
		Constant constant;
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

	// ヒープのセット
	void SetHeap(UINT* index, UINT i = 2);

	// 描画準備
	void SetDraw(UINT* index);

	void DrawGraph(UINT& index, FLOAT x, FLOAT y, BOOL turn = FALSE);

private:
	// ヒープの生成
	HRESULT CreateHeap(UINT* index);

	// リソースの生成
	HRESULT CreateResourcce(UINT* index);

	// バッファビューの生成
	HRESULT CreateView(UINT* index);

	// シェーダリソースビューの生成
	HRESULT CreateShaderView(UINT* index);

	// 頂点リソースの生成
	HRESULT CreateVertexResource(UINT* index);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// WICデータ
	std::map<UINT*, WIC>wic;
};

namespace okmonn {
	// ユニコード変換
	std::wstring ChangeUnicode(const CHAR* str);
}
