#pragma once
#include "Scene.h"
#include <vector>
class Title :
	public Scene
{
public:
	// コンストラクタ
	Title(std::weak_ptr<Input>in);
	// デストラクタ
	~Title();

	// 描画
	void Draw(void);

	// 処理
	void UpData(void);

private:
	// フェードイン
	void FadeIn(void);
	// フェードアウト
	void FadeOut(void);

	// 画像ハンドル
	int image;

	// フレーム
	int flam;

	int cnt;

	// 関数ポインタ
	void (Title::*func)(void);
};

