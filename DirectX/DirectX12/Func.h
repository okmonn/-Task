#pragma once
#include <dinput.h>
#include <string>

namespace func {
	// ウィンドウサイズのセット
	void SetWindowSize(unsigned int x, unsigned int y);

	// 初期化
	void Init(void);

	// メッセージの確認
	bool CheckMsg(void);

	// 描画準備
	void Set(void);

	// 実行
	void Do(void);

	// キー入力
	bool CheckKey(unsigned int index);

	// トリガーキー入力
	bool TriggerKey(unsigned int index);

	// 画像読み込み
	void LoadImg(unsigned int& index, const std::string& fileName);

	// 描画
	void Draw(unsigned int& index, float x, float y, bool turnX = false, bool turnY = false);

	// 描画・サイズ指定
	void Draw(unsigned int& index, float x, float y, float sizeX, float sizeY, bool turnX = false, bool turnY = false);

	// 描画・サイズ指定・分割
	void Draw(unsigned int& index, float x, float y, float sizeX, float sizeY, 
		float rectX, float rectY, float rectSizeX, float rectSizeY, bool turnX = false, bool turnY = false);
}
