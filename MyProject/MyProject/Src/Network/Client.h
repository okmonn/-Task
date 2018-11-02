#pragma once
#include "Sock.h"

class Client :
	public Sock
{
public:
	// コンストラクタ
	Client();
	// デストラクタ
	~Client();

	// 送信
	int Send(const char* data, int size);

	// 受信
	int Recv(std::vector<char>& data);

private:
	// 接続
	int Connect(void);

	// 通信の準備
	int SetUp(void);

	// 初期化
	void Init(void);
};
