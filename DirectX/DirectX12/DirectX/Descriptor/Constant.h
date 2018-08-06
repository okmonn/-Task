#pragma once
#include "Descriptor.h"
#include "../../etc/Typedef.h"
#include <vector>

class Window;

class Constant :
	public Descriptor
{
public:
	// コンストラクタ
	Constant(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~Constant();

	// WVPの更新
	void UpDataWVP(void);

	// Imageの更新
	void UpDataImage(void);

	// 定数バッファのセット
	void SetConstant(UINT rootIndex = 0, UINT index = 0);

	// リソースの取得
	ID3D12Resource* GetResoure(UINT index) const {
		return resource[index];
	}
	// リソースの取得
	std::vector<ID3D12Resource*> GetResoure() const {
		return resource;
	}

private:
	// WVPのセット
	void SetWVP(void);

	// ヒープの生成
	HRESULT CreateHeap(void);

	// リソースの生成
	HRESULT CreateResource(void);

	// リソースビューの生成
	HRESULT CreateView(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// リソース
	std::vector<ID3D12Resource*> resource;

	// WVP
	WVP wvp;

	// 画像データ
	Image image;

	// 送信データ
	std::vector<UINT8*> data;
};

