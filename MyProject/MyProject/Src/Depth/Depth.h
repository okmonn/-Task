#pragma once
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Union;
class Device;
class List;

class Depth
{
public:
	// コンストラクタ
	Depth(std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~Depth();

	// 深度ステンシルのセット
	void SetDepth(void);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	// リソースの取得
	ID3D12Resource* GetRsc(void) const {
		return rsc;
	}

private:
	// ヒープの生成
	long CreateHeap(void);

	// リソースの生成
	long CreateRsc(void);

	// リソースビューの生成
	void CreateView(void);

	// 初期化
	void Init(void);


	// ユニオン
	Union& un;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース
	ID3D12Resource* rsc;
};
