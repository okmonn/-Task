#pragma once
#include "../Descriptor/Descriptor.h"
#include "../etc/Vector2.h"
#include "../etc/Typedef.h"
#include <vector>
#include <map>
#include <string>

class Root;
class Pipe;
class Constant;

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
		std::vector<TexVertex>vertex;
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

	// 白テクスチャ
	struct White {
		//画像データ
		std::vector<UCHAR>image;
		//定数バッファ
		Con con;
	};

public:
	// コンストラクタ
	Texture(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<Constant>con);
	// デストラクタ
	~Texture();

	// WIC読み込み
	HRESULT LoadWIC(UINT& index, const std::string& fileName);

	// 白テクスチャの作成
	HRESULT CreateWhiteTex(UINT& index);

	// 描画準備
	HRESULT SetDraw(UINT& index);

	// ディスクリプターのセット
	void SetDescriptor(UINT& index);

	// 描画
	void Draw(UINT& index, const Vec2f& pos, float alpha, UINT turnX, UINT turnY);

	// 描画・サイズ指定
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, float alpha, UINT turnX, UINT turnY);

	// 描画・サイズ指定・分割
	void Draw(UINT& index, const Vec2f& pos, const Vec2f& size, const Vec2f& rectPos, const Vec2f& rectSize, float alpha, UINT turnX, UINT turnY);

	// 描画・自由変形
	void FreelyDraw(UINT& index, const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Vec2f& pos4, float alpha, UINT turnX, UINT turnY);

	// 描画・分割・自由変形
	void FreelyDraw(UINT& index, const Vec2f& rectPos, const Vec2f& rectSize, const Vec2f& pos1, const Vec2f& pos2, const Vec2f& pos3, const Vec2f& pos4, float alpha, UINT turnX, UINT turnY);

	// 削除
	void Delete(UINT& index);

private:
	// 定数バッファヒープの生成
	HRESULT CreateHeap(UINT* index);

	// シェーダリソースビューの生成
	HRESULT CreateShaderView(UINT* index);

	// 頂点リソースの生成
	HRESULT CreateResource(UINT* index);


	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// 定数バッファ
	std::weak_ptr<Constant>con;

	// WICデータ
	std::map<UINT*, WIC>wic;

	// 白テクスチャ
	std::map<UINT*, White>white;
};

namespace func {
	// ユニコード変換
	std::wstring ChangeUnicode(const CHAR* str);
}
