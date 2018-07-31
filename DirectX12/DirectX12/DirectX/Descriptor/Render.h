#pragma once
#include "Descriptor.h"

class Render : 
	public Descriptor
{
public:
	// コンストラクタ
	Render(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap);
	// デストラクタ
	~Render();

private:
	// リソースの生成
	HRESULT Create(void);
};

