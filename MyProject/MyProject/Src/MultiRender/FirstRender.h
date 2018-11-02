#pragma once
#include "MultiRender.h"

class FirstRender :
	public MultiRender
{
public:
	// コンストラクタ
	FirstRender(std::weak_ptr<Device>dev, std::weak_ptr<Render>render);
	// デストラクタ
	~FirstRender();

private:
	// 初期化
	void Init(void);

	// レンダーターゲットビューの生成
	void CreateRtv(void);

	// シェーダーリソースビューの生成
	void CreateSrv(void);
};
