#pragma once
#include "Sock.h"

class Server :
	public Sock
{
public:
	// コンストラクタ
	Server();
	// デストラクタ
	~Server();

	// クライアントの接続
	int Accept(void);

	// 送信
	int Send(const char* data, int size);

	// 受信
	int Recv(std::vector<char>& data, const unsigned int& index = 0);

private:
	// 通信の準備
	int SetUp(void);

	// アドレスとの関連付け
	int Bind(void);

	// クライアントとの接続待機
	int Listen(void);

	// 初期化
	void Init(void);

	// クライアントとの接続終了
	int Close(const unsigned int& index = 0);


	// クライアントソケット
	std::vector<SOCKET>client;

	// クライアントのアドレス
	std::vector<sockaddr_in>clientAddr;
};
