#pragma once
class Input
{
public:
	// コンストラクタ
	Input();
	// デストラクタ
	virtual ~Input();

	// 現在のキー状態のチェック
	const bool CheckTrigger(int key)const;
	// 現在のキー押下状態のチェック
	const bool CheckPress(int key)const;

	// 処理
	void UpData(void);

private:
	// 現在のキー状態
	int state;

	// 直前のキー状態
	int old_state;
};

