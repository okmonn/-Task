#pragma once
#include "Descriptor.h"
#include "../../etc/Typedef.h"

class Window;

class Constant :
	public Descriptor
{
public:
	// コンストラクタ
	Constant(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// デストラクタ
	~Constant();

	// WVPの更新
	void UpDataWVP(void);

	// WVPの取得
	WVP GetWvp(void) const {
		return wvp;
	}

	// 定数バッファのセット
	void SetConstant(UINT index = 0, UINT table = 0);

private:
	// WVPのセット
	void SetWorldViewProjection(void);

	// リソースの生成
	HRESULT CreateResource(void);

	// リソースビューの生成
	HRESULT CreateView(void);


	// ウィンドウ
	std::weak_ptr<Window>win;

	// WVP
	WVP wvp;

	// 送信データ
	UINT8* data;
};

