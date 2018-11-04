#pragma once

// ウィンドウサイズ
#define WINDOW_X 640
#define WINDOW_Y 480

// 状態
enum class State {
	wait,
	walk,
	attack,
	jump,
	down,
	damage
};
