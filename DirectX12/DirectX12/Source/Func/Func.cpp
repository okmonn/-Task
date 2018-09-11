#include "Func.h"
#include "../Union/Union.h"

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

// ラインの描画
void func::DrawLine(float x1, float y1, float x2, float y2, float r, float g, float b)
{
	Union::Get().DrawLine({ x1, y1 }, { x2, y2 }, { r, g, b });
}

// ボックスの描画
void func::DrawBox(float x, float y, float sizeX, float sizeY, float r, float g, float b)
{
	Union::Get().DrawBox({ x, y }, { sizeX, sizeY },  { r, g, b });
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

// 画像の消去
void func::DeleteImg(unsigned int & index)
{
	Union::Get().DeleteImg(index);
}

// VMD読み込み
void func::LoadVMD(unsigned int & index, const std::string & fileName)
{
	Union::Get().LoadVMD(index, fileName);
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

// PMDのボーン回転
void func::RotateBorn(unsigned int & index, const std::string & name, const DirectX::XMMATRIX & matrix)
{
	Union::Get().RotateBorn(index, name, matrix);
}

// PMDのボーン回転
void func::RotateBorn(unsigned int & model, unsigned int & motion)
{
	Union::Get().RotateBorn(model, motion);
}

// PMDの消去
void func::DeletePMD(unsigned int & index)
{
	Union::Get().DeletePMD(index);
}

// WAVEの読み込み
void func::LoadWave(unsigned int& index, const std::string fileName)
{
	Union::Get().LoadWave(index, fileName);
}

// WAVEの再生
void func::PlayWave(unsigned int& index, bool loop)
{
	Union::Get().PlayWave(index, loop);
}

// WAVEの停止
void func::StopWave(unsigned int& index)
{
	Union::Get().StopWave(index);
}

// WAVEの消去
void func::DeleteWAVE(unsigned int & index)
{
	Union::Get().DeleteWAVE(index);
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
std::vector<std::tstring> func::GetDirFile(const std::tstring & path)
{
	return Union::Get().GetDirFile(path);
}

// ログインユーザーネーム取得
std::tstring func::GetName(void)
{
	std::tstring name;
	
	DWORD size = 0;

	GetUserName(&name[0], &size);

	name.resize(size);
	GetUserName(&name[0], &size);

	return name;
}
