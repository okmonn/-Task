#include "Func.h"
#include "DirectX/Union.h"
#include <Windows.h>

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

// ビュー行列のセット
void func::ChangeView(const Vec3f & pos, const Vec3f & target, const Vec3f & up)
{
	Union::Get().ChangeView(pos, target, up);
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

// α値の設定
void func::SetAlpha(float alpha)
{
	Union::Get().SetAlpha(alpha);
}

// 点の描画
void func::DrawPoint(float x, float y, float r, float g, float b)
{
	Union::Get().DrawPoint({ x, y }, { r, g, b });
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

// PMD読み込み
void func::LoadPMD(unsigned int & index, const std::string & fileName)
{
	Union::Get().LoadPMD(index, fileName);
}

// PMD描画
void func::DrawPMD(unsigned int & index)
{
	Union::Get().DrawPMD(index);
}

// WAVEの読み込み
void func::LoadWave(unsigned int& index, const std::string fileName)
{
	Union::Get().LoadWave(index, fileName);
}

// WAVEの再生
void func::PlayWave(unsigned int& index)
{
	Union::Get().PlayWave(index);
}

// WAVEの停止
void func::StopWave(unsigned int& index)
{
	Union::Get().StopWave(index);
}

// MIDIデバイスの数取得
unsigned int func::GetMidiDevNum(void)
{
	return Union::Get().GetMidiNum();
}

// MIDIステータスバイトの取得
unsigned char func::GetMidiState(void)
{
	return Union::Get().GetMidiState();
}

// MIDIデータバイト1の取得
unsigned char func::GetMidiData1(void)
{
	return Union::Get().GetMidiData1();
}

// MIDIデータバイト2の取得
unsigned char func::GetMidiData2(void)
{
	return Union::Get().GetMidiData2();
}

// 指定したディレクトリのファイル名取得
std::vector<std::string> func::GetDirFile(const std::string & path)
{
	return Union::Get().GetDirFile(path);
}

// 指定したディレクトリのファイル名取得
std::vector<std::wstring> func::GetDirFile(const std::wstring & path)
{
	return Union::Get().GetDirFile(path);
}

// ログインユーザーネーム取得
std::wstring func::GetName(void)
{
	std::wstring name;
	DWORD size = 0;

	GetUserName(&name[0], &size);

	name.resize(size);
	GetUserName(&name[0], &size);

	return name;
}
