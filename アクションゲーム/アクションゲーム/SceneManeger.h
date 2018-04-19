#pragma once
class SceneManeger
{
public:
	// デストラクタ
	~SceneManeger();

	// インスタンス化
	void Create(void);

	// 破棄
	void Destroy(void);

	// インスタンス変数取得
	static SceneManeger* GetInstance()
	{
		return s_Instance;
	}

private:
	// コンストラクタ
	SceneManeger();

	// インスタンス変数
	static SceneManeger* s_Instance;
};

