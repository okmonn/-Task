#pragma once
class Interface
{
public:
	// コンストラクタ
	Interface();
	// デストラクタ
	virtual ~Interface();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	int top;
	int bottom;
};

