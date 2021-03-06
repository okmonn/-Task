#include "GameMane.h"
#include "../Scene/Teach.h"
#include "../../Func/Func.h"

// ウィンドウサイズ
#define WINDOW_X 640
#define WINDOW_Y 480

// コンストラクタ
GameMane::GameMane()
{
}

// デストラクタ
GameMane::~GameMane()
{
}

// シーン移行
void GameMane::ChangeScene(Scene * scene)
{
	this->scene.reset(scene);
}

// 初期化
void GameMane::Init(void)
{
	func::SetWindowSize(WINDOW_X, WINDOW_Y);
	func::Start();

	ChangeScene(new Teach());
}

// 描画
void GameMane::Draw(void)
{
	func::Set();

	scene->Draw();

	func::Do();
}

// 処理
void GameMane::UpData(void)
{
	scene->UpData();

	Draw();
}

// 処理
void GameMane::Run(void)
{
	Init();

	while (func::CheckMsg() && func::CheckKey(INPUT_ESCAPE) != true)
	{
		UpData();
	}

	scene.reset();

	func::End();
}
