#pragma once
#include <DirectXMath.h>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Union;
class Device;
class List;

class Constant
{
	// WVP
	struct WVP {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT2   winSize;
	};

public:
	// コンストラクタ
	Constant(std::weak_ptr<Device>dev, std::weak_ptr<List>list);
	// デストラクタ
	~Constant();

	// WVPの変更
	void ChangeWvp(const float& eyeX, const float& eyeY, const float& eyeZ,
		const float& targetX, const float& targetY, const float& targetZ, const float& upX = 0.0f, const float& upY = 1.0f, const float& upZ = 0.0f);

	// WVPの更新
	void UpDataWvp(const float& angle);

	// 定数バッファのセット
	void SetConstant(const unsigned int& index = 0);

private:
	// WVPの初期化
	void InitWvp(void);

	// ヒープの生成
	long CreateHeap(void);

	// リソースの生成
	long CreateRsc(void);

	// リソースビューの生成
	void CreateView(void);

	// マップ
	long Map(void);

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

	// 送信データ
	unsigned int* data;

	// WVP
	WVP wvp;
};
