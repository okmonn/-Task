#pragma once
#include "../Descriptor.h"

class Window;

class Depth :
	public Descriptor
{
public:
	// コンストラクタ
	Depth(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~Depth();

	// 深度ステンシルのセット
	void SetDepth(void);

	// リソースの取得
	ID3D12Resource* GetResource(void) const {
		return resource;
	}

private:
	// リソースの生成
	HRESULT CreateResource(void);

	// リソースビューの生成
	HRESULT CreateView(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// リソース
	ID3D12Resource * resource;
};

