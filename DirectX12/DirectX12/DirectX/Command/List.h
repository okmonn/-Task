#pragma once
#include "Command.h"
#include <memory>

class List :
	public Command
{
public:
	// コンストラクタ
	List(std::weak_ptr<Device>dev);
	// デストラクタ
	~List();

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
	HRESULT CreateAllo(void);

	// コマンドリストの生成
	HRESULT CreateList(void);


	// コマンドアロケータ
	ID3D12CommandAllocator* allo;

	// コマンドリスト
	ID3D12GraphicsCommandList* list;
};

