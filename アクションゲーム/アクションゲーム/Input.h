#pragma once
class Input
{
public:
	// コンストラクタ
	Input();
	// デストラクタ
	virtual ~Input();

	// キーの押した瞬間のチェック
	const bool CheckTrigger(void)const;

	// 処理
	void UpData(void);

private:

};

