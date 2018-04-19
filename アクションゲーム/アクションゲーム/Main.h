#pragma once

#ifdef EXPORT_MAIN
#define EXTERN_MAIN
#else
#define EXTERN_MAIN extern
#endif

// システムの初期化
bool System(void);

// 描画
void Draw(void);

// 処理
void UpData(void);

// 終了処理
void Destroy(void);