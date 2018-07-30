#pragma once
#include "../Obj.h"
#include <memory>

class Device;
class Allocator;

class List :
	public Obj
{
public:
	// コンストラクタ
	List(std::weak_ptr<Device>dev, std::weak_ptr<Allocator>allo);
	// デストラクタ
	~List();

	// コマンドリストの取得
	ID3D12GraphicsCommandList* Get(void) const {
		return list;
	}

private:
	// コマンドリストの生成
	HRESULT Create(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドアロケータ
	std::weak_ptr<Allocator>allo;

	// コマンドリスト
	ID3D12GraphicsCommandList* list;
};

