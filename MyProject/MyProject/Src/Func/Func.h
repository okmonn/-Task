#pragma once
#include "InputData.h"
#include <string>
#include <vector>

namespace func {
	// ビット変換
	int Bit(const int& i);

	// バイト変換
	int Byte(const int& i);

	// 対数
	int Log(const int& i, const int& n);

	// 累乗
	int Pow(const int& i, const int& n);

	// 文字列の確認
	bool CheckChar(const std::string& find, unsigned char * data, const unsigned int& dataNum = 4);
	bool CheckChar(const std::string& find, char * data, const unsigned int& dataNum = 4);

	// 文字列の代入
	void AddChar(const std::string& add, unsigned char* data, const unsigned int& dataNum = 4);

	// 文字列の検索
	std::string FindString(const std::string& path, const char& find, const unsigned int& offset = 1, const bool& end = true);

	// フォルダ名とファイル名の連結
	std::string FileLink(const std::string& path1, const std::string& path2);

	// ワイド文字列の変換
	std::wstring ChangeWString(const std::string& st);

	// ディレクトリのファイル名取得
	std::vector<std::string> GetDirFile(const std::string& dir);

	// ニュートン法
	float Newton(const float& input, const float& pos1X, const float& pos1Y, const float& pos2X, const float& pos2Y, const unsigned int& loop = 16);

	// 2分法
	float Bisection(const float& input, const float& pos1X, const float& pos1Y, const float& pos2X, const float& pos2Y, const unsigned int& loop = 32);

	// ウィンドウサイズのセット
	void SetWindowSize(const unsigned int& x = 640, const unsigned int& y = 480);

	// ウィンドウサイズXの取得
	float GetWinSizeX(void);

	// ウィンドウサイズYの取得
	float GetWinSizeY(void);

	// 初期化・スタート
	void Start(void);

	// WVPの更新
	void ChangeWVP(const float& eyeX, const float& eyeY, const float& eyeZ,
		const float& targetX, const float& targetY, const float& targetZ, const float& upX = 0.0f, const float& upY = 1.0f, const float& upZ = 0.0f);

	// メッセージの確認
	bool CheckMsg(void);

	// キー入力
	bool CheckKey(const int& i);

	// トリガー入力
	bool CheckTriger(const int& i);

	// サウンドの読み込み
	void LoadSnd(const std::string& fileName, int& i);

	// サウンドの再生
	void Play(int& i, const bool& loop = false);

	// サウンドの停止
	void Stop(int& i);

	// 画像の読み込み
	void LoadImg(const std::string& fileName, int& i);

	// PMDの読み込み
	void LoadPmd(const std::string& fileName, int& i);

	// アニメーションのアタッチ
	void Attach(const std::string& fileName, int& i);

	// 描画準備
	void Set(void);

	// ポイント描画
	void DrawPoint(const float& x, const float& y, const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// ライン描画
	void DrawLine(const float& x1, const float& y1, const float& x2, const float& y2, const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// トライアングル描画
	void DrawTriangle(const float& x1, const float& y1, const float& x2, const float& y2, const float& x3, const float& y3,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// ボックス描画
	void DrawBox(const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& r, const float& g, const float& b, const float& alpha = 1.0f);

	// 画像の描画
	void DrawImg(int& i, const float& x, const float& y, const int& turnX = 0, const int& turnY = 0, const float& alpha = 1.0f);

	// 画像の描画・サイズ指定
	void DrawSizeImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY, 
		const int& turnX = 0, const int& turnY = 0, const float& alpha = 1.0f);

	// 画像の描画・サイズ・範囲指定
	void DrawRectImg(int& i, const float& x, const float& y, const float& sizeX, const float& sizeY,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const int& turnX = 0, const int& turnY = 0, const float& alpha = 1.0f);

	// 画像の描画・4点指定
	void DrawFreelyImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2,
		const float & x3, const float & y3, const float & x4, const float & y4, const int & turnX = 0, const int & turnY = 0, const float & alpha = 1.0f);

	// 画像の描画・4点・範囲指定
	void DrawFreelyRectImg(int & i, const float & x1, const float & y1, const float & x2, const float & y2, const float & x3, const float & y3, const float & x4, const float & y4,
		const float& rectX, const float& rectY, const float& rectSizeX, const float& rectSizeY, const int & turnX = 0, const int & turnY = 0, const float & alpha = 1.0f);

	// PMDのアニメーション時間のリセット
	void ResetAnim(int& i);

	// アニメーション
	void Animation(int& i, const bool& loop = false, const float& animSpeed = 0.5f);

	// アニメーションの終了確認
	bool CheckEndAnim(int& i);

	// PMDの描画
	void DrawPmd(int& i);
	
	// 描画実行
	void Do(void);

	// サウンドの削除
	void DeleteSnd(int& i);

	// 画像の削除
	void DeleteImg(int& i);

	// モデルの削除
	void DeleteMdl(int& i);

	// 終了処理
	void End(void);
}
