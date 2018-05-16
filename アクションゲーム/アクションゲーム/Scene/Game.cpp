#include "Game.h"
#include "DxLib.h"
#include "../Load.h"
#include "../EnemyMane.h"

const int SIZE_X = 768;
const int SIZE_Y = 448;

// コンストラクタ
Game::Game()
{
}

// コンストラクタ
Game::Game(const Game&)
{
}

// デストラクタ
Game::~Game()
{
}

void Game::Init(void)
{
	//グラフィックモードの設定
	SetGraphMode(SIZE_X, SIZE_Y, 32);

	//true:window　false:ﾌﾙｽｸﾘｰﾝ
	ChangeWindowMode(true);

	//windowテキストの設定
	SetWindowText(_T("1601271_岡繁男"));

	//Dxlibの初期化
	if (DxLib_Init() == -1)
	{
		return;
	}

	//ひとまずﾊﾞｯｸﾊﾞｯﾌｧに描画
	SetDrawScreen(DX_SCREEN_BACK);


	Create();
}

// クラスのインスタンス化
void Game::Create(void)
{
	//ロードクラス
	Load::Create();

	// 敵の管理クラス
	EnemyMane::Create();

	//背景クラス
	back = std::make_shared<BackGround>();

	//UIクラス
	ui = std::make_shared<Interface>();

	//インプット
	in = std::make_shared<Input>();

	//プレイヤークラス
	pl = std::make_shared<Player>(in);

	//敵クラス
	e_list.push_back(EnemyMane::GetInstance()->CreateDeadman(300, 330, pl));
	e_list.push_back(EnemyMane::GetInstance()->CreateDeadman(500, 330, pl));

	//地面クラス
	ground = std::make_shared<Ground>(pl);
}

// それぞれのクラスの描画
void Game::Draw(void)
{
	//画面消去
	ClsDrawScreen();

	back->Draw();
	ui->Draw();
	pl->Draw();
	for (auto itr = e_list.begin(); itr != e_list.end(); ++itr)
	{
		(*itr)->Draw();
	}
	ground->Draw();

#ifdef _DEBUG
	DrawFormatString(600, 10, GetColor(0, 255, 0), "%d", e_list.size());
#endif
	//裏画面を表画面に瞬間コピー
	ScreenFlip();
}

// それぞれのクラスの処理
void Game::UpData(void)
{
	Draw();

	in->UpData();

	pl->UpData();

	if (pl->GetPos().x == 300)
	{
		if (e_list.size() < 10)
		{
			e_list.push_back(EnemyMane::GetInstance()->CreateDeadman(50, 330, pl));
		}
	}

	for (auto itr = e_list.begin(); itr != e_list.end();)
	{
		(*itr)->UpData();
		if ((*itr)->GetDie() == true)
		{
			itr = e_list.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	ground->UpData();
}

// メインループ
void Game::Run(void)
{
	//ループ処理
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_RETURN) == 0)
	{
		UpData();
	}

	Destroy();
}

// 終了処理
void Game::Destroy(void)
{
	EnemyMane::Destroy();
	Load::Destroy();

	//Dxlibの終了
	DxLib_End();
}
