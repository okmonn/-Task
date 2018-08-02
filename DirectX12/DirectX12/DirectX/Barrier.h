#pragma once
#include "Obj.h"
#include <memory>

class List;
class Swap;
class Render;

class Barrier : 
	public Obj
{
public:
	// コンストラクタ
	Barrier(std::weak_ptr<List>list, std::weak_ptr<Swap>swap, std::weak_ptr<Render>render);
	// デストラクタ
	~Barrier();

	// バリアのセット
	void SetBarrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter);

private:
	// コマンドリスト
	std::weak_ptr<List>list;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// レンダーターゲット
	std::weak_ptr<Render>render;

	// バリア
	D3D12_RESOURCE_BARRIER barrier;
};

