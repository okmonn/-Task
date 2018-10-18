#pragma once

struct ID3D12Debug;

class Debug
{
	// コンストラクタ
	Debug();
	// デストラクタ
	~Debug();

private:
	// デバッガの生成
	long CreateDebug(void);


	// デバッグ
	ID3D12Debug* debug;
};
