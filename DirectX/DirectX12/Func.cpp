#include "Func.h"
#include "DirectX/Union.h"

// ウィンドウサイズのセット
void func::SetWindowSize(unsigned int x, unsigned int y)
{
	Union::Get().ChangeWindowSize(x, y);
}

// 初期化
void func::Init(void)
{
	Union::Get().Create();
}

// メッセージの確認
bool func::CheckMsg(void)
{
	return Union::Get().CheckMsg();
}

// 描画準備
void func::Set(void)
{
	Union::Get().Set();
}

// 実行
void func::Do(void)
{
	Union::Get().Do();
}

// キー入力
bool func::CheckKey(unsigned int index)
{
	return Union::Get().CheckKey(index);
}

// トリガーキー入力
bool func::TriggerKey(unsigned int index)
{
	return Union::Get().TriggerKey(index);
}

// 画像読み込み
void func::LoadImg(unsigned int & index, const std::string & fileName)
{
	Union::Get().LoadImg(index, fileName);
}

// 描画
void func::Draw(unsigned int & index, float x, float y, bool turnX, bool turnY)
{
	Union::Get().Draw(index, { x, y }, turnX, turnY);
}

// 描画・サイズ指定
void func::Draw(unsigned int & index, float x, float y, float sizeX, float sizeY, bool turnX, bool turnY)
{
	Union::Get().Draw(index, { x, y }, { sizeX, sizeY }, turnX, turnY);
}

// 描画・サイズ指定・分割
void func::Draw(unsigned int & index, float x, float y, float sizeX, float sizeY, float rectX, float rectY, float rectSizeX, float rectSizeY, bool turnX, bool turnY)
{
	Union::Get().Draw(index, { x, y }, { sizeX, sizeY }, { rectX, rectY }, { rectSizeX, rectSizeY }, turnX, turnY);
}
