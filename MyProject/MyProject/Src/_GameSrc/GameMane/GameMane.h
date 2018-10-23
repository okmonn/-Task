#pragma once
#include <memory>

class Scene;

class GameMane 
{
public:
	// デストラクタ
	~GameMane();

	// インスタンス変数の取得
	static GameMane& Get(void) {
		static GameMane instance;
		return instance;
	}

	// シーン移行
	void ChangeScene(Scene* scene);

	// 処理
	void Run(void);

private:
	// コンストラクタ
	GameMane();
	GameMane(const GameMane&) {
	}
	void operator=(const GameMane&) {
	}

	// 初期化
	void Init(void);

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

	
	// シーン
	std::shared_ptr<Scene>scene;
};
