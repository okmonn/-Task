#include "SceneManeger.h"

//インスタンス変数の初期化
SceneManeger* SceneManeger::s_Instance = nullptr;

// コンストラクタ
SceneManeger::SceneManeger()
{
}

// デストラクタ
SceneManeger::~SceneManeger()
{
}

// インスタンス化
void SceneManeger::Create(void)
{
	if (s_Instance == nullptr)
	{
		s_Instance = new SceneManeger;
	}
}

// 破棄
void SceneManeger::Destroy(void)
{
	if (s_Instance != nullptr)
	{
		delete s_Instance;
	}

	s_Instance = nullptr;
}
