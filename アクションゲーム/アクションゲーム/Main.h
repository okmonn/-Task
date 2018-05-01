#pragma once

#ifdef EXPORT_MAIN
#define EXTERN_MAIN
#else
#define EXTERN_MAIN extern
#endif

#include "BackGround.h"
#include "Input.h"
#include "Player.h"
#include "Ground.h"
#include <memory>

// システムの初期化
bool System(void);

// インスタンス化
void Create(void);

// 描画
void Draw(void);

// 処理
void UpData(void);

// 終了処理
void Destroy(void);

// 背景クラス
EXTERN_MAIN std::shared_ptr<BackGround>back;

// インプット
EXTERN_MAIN std::shared_ptr<Input>in;

// プレイヤークラス
EXTERN_MAIN std::shared_ptr<Player>pl;

// 地面クラス
EXTERN_MAIN std::shared_ptr<Ground>ground;