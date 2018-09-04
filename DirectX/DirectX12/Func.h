#pragma once
#include "etc/Vector2.h"
#include "etc/Vector3.h"
#include "Input/InputData.h"
#include <string>
#include <vector>

typedef unsigned int UINT;
typedef unsigned char UCHAR;

namespace func {
	// ウィンドウサイズのセット
	void SetWindowSize(unsigned int x, unsigned int y);

	// 初期化
	void Init(void);

	// メッセージの確認
	bool CheckMsg(void);

	// ビュー行列のセット
	void ChangeView(const Vec3f& pos, const Vec3f& target, const Vec3f& up);

	// 描画準備
	void Set(void);

	// 実行
	void Do(void);

	// キー入力
	bool CheckKey(unsigned int index);

	// トリガーキー入力
	bool TriggerKey(unsigned int index);

	// α値の設定
	void SetAlpha(float alpha);

	// 点の描画
	void DrawPoint(float x, float y, float r, float g, float b);

	// ボックスの描画
	void DrawBox(float x, float y, float sizeX, float sizeY, float r, float g, float b);

	// 画像読み込み
	void LoadImg(unsigned int& index, const std::string& fileName);

	// 描画
	void Draw(unsigned int& index, float x, float y, bool turnX = false, bool turnY = false);

	// 描画・サイズ指定
	void Draw(unsigned int& index, float x, float y, float sizeX, float sizeY, bool turnX = false, bool turnY = false);

	// 描画・サイズ指定・分割
	void Draw(unsigned int& index, float x, float y, float sizeX, float sizeY,
		float rectX, float rectY, float rectSizeX, float rectSizeY, bool turnX = false, bool turnY = false);

	// PMD読み込み
	void LoadPMD(unsigned int& index, const std::string& fileName);

	// PMD描画
	void DrawPMD(unsigned int& index);

	// WAVEの読み込み
	void LoadWave(unsigned int& index, const std::string fileName);

	// WAVEの再生
	void PlayWave(unsigned int& index);

	// WAVEの停止
	void StopWave(unsigned int& index);

	// MIDIデバイスの数取得
	unsigned int GetMidiDevNum(void);

	// MIDIステータスバイトの取得
	unsigned char GetMidiState(void);

	// MIDIデータバイト1の取得
	unsigned char GetMidiData1(void);

	// MIDIデータバイト2の取得
	unsigned char GetMidiData2(void);

	// 指定したディレクトリのファイル名取得
	std::vector<std::string> GetDirFile(const std::string& path);
	std::vector<std::wstring> GetDirFile(const std::wstring& path);

	// ログインユーザーネーム取得
	std::wstring GetName(void);
}
