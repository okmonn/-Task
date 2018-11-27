#pragma once
#include <memory>

struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12Resource;

enum D3D12_RESOURCE_STATES : int;

class Union;
class Device;
class Swap;
class Render;
class MultiRender;

class List
{
public:
	// コンストラクタ
	List(std::weak_ptr<Device>dev);
	// デストラクタ
	~List();

	// リセット
	void Reset(ID3D12PipelineState* pipe);

	// ルートシグネチャのセット
	void SetRoot(ID3D12RootSignature& root);

	// パイプラインのセット
	void SetPipe(ID3D12PipelineState& pipe);

	// ビューポートのセット
	void SetViewport(void);
	void SetViewport(const unsigned& w, const unsigned& h);

	// シザーのセット
	void SetScissor(void);
	void SetScissor(const unsigned& w, const unsigned& h);

	// バリアのセット
	void SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, std::weak_ptr<Swap>swap, std::weak_ptr<Render>render);

	// バリアのセット
	void SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, std::weak_ptr<MultiRender>multi);

	// バリアのセット
	void SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, ID3D12Resource* rsc);

	// コマンドを閉じる
	void Close(void);

	// コマンドアロケータの取得
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}
	// コマンドリストの取得
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// コマンドアロケータの生成
	long CreateAllo(void);

	// コマンドリストの生成
	long CreateList(void);


	// ユニオン
	Union& un;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドアロケータ
	ID3D12CommandAllocator* allo;

	// コマンドリスト
	ID3D12GraphicsCommandList* list;
};
