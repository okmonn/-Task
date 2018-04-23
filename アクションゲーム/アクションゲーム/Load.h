#pragma once
#include <string>
class Load
{
public:
	// デストラクタ
	~Load();

	// インスタンス化
	static void Create(void);
	// 破棄
	static void Destroy(void);

	// インスタンス変数の取得
	Load* GetInstance(void)
	{
		return s_Instance;
	}

	// 読み込み
	bool LoadAct(std::string fileName);

private:
	// コンストラクタ
	Load();

	// インスタンス変数
	static Load* s_Instance;
};

