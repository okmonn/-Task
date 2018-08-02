#pragma once
#include "Descriptor.h"

class Render : 
	public Descriptor
{
public:
	// コンストラクタ
	Render(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// デストラクタ
	~Render();

	// レンダーターゲットのセット
	void SetRender(ID3D12DescriptorHeap* depth, UINT index = 0);

private:
	// リソースの生成
	HRESULT Create(void);
};

