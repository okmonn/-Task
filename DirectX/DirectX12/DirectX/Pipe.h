#pragma once
#include "Obj.h"
#include <memory>

class Device;
class Swap;
class Root;

class Pipe :
	public Obj
{
public:
	// コンストラクタ
	Pipe(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap, std::weak_ptr<Root>root);
	// デストラクタ
	~Pipe();

	// パイプラインの取得
	ID3D12PipelineState* Get(void) const {
		return pipe;
	}

private:
	// パイプラインの生成
	HRESULT CreatePipe(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	ID3D12PipelineState* pipe;
};

