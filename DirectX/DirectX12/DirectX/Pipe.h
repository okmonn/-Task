#pragma once
#include "Obj.h"
#include <memory>

class Device;
class Swap;
class Root;
class Compiler;

class Pipe :
	public Obj
{
public:
	// コンストラクタ
	Pipe(const LPCWSTR& path, std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap, std::weak_ptr<Root>root, std::weak_ptr<Compiler>com);
	// デストラクタ
	~Pipe();

	// パイプラインの取得
	ID3D12PipelineState* Get(void) const {
		return pipe;
	}

private:
	// パイプラインの生成
	HRESULT CreatePipe(void);


	// パス
	LPCWSTR path;

	// デバイス
	std::weak_ptr<Device>dev;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// シェーダコンパイラー
	std::weak_ptr<Compiler>com;

	// パイプライン
	ID3D12PipelineState* pipe;
};

