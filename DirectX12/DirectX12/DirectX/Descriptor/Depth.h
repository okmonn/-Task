#pragma once
#include "Descriptor.h"

class Window;

class Depth :
	public Descriptor
{
public:
	// コンストラクタ
	Depth(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap);
	// デストラクタ
	~Depth();

private:
	// リソース生成
	HRESULT CreateResource(void);

	// リソースビューの生成
	HRESULT CreateView(void);


	// ウィンドウ
	std::weak_ptr<Window>win;
};

