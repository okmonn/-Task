#pragma once
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Device;
class List;
class Swap;

class Render
{
public:
	// コンストラクタ
	Render(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// デストラクタ
	~Render();

	// レンダーターゲットのセット
	void SetRender(ID3D12DescriptorHeap& depth);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	// リソースの取得
	std::vector<ID3D12Resource*> GetRsc(void) const {
		return rsc;
	}
	//リソースの取得
	ID3D12Resource* GetRsc(const unsigned int& index) const {
		return rsc[index];
	}
	// ヒープサイズの取得
	unsigned int GetSize(void) const {
		return size;
	}

private:
	// ヒープの生成
	long CreateHeap(void);

	// リソースの生成
	long CreateRsc(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドリスト
	std::weak_ptr<List>list;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース
	std::vector<ID3D12Resource*>rsc;

	// ヒープサイズ
	unsigned int size;
};
