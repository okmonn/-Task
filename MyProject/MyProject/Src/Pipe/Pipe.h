#pragma once
#include <memory>

struct ID3D12PipelineState;
struct D3D12_INPUT_ELEMENT_DESC;
enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;

class Device;
class Swap;
class Root;

class Pipe
{
public:
	// コンストラクタ
	Pipe(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap, std::weak_ptr<Root>root);
	// デストラクタ
	~Pipe();

	// パイプラインの生成
	long CreatePipe(const D3D12_INPUT_ELEMENT_DESC& input, const unsigned int& num, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const bool& depth = false);

	// パイプラインの取得
	ID3D12PipelineState* Get(void) const {
		return pipe;
	}

private:
	// デバイス
	std::weak_ptr<Device>dev;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	ID3D12PipelineState* pipe;
};
