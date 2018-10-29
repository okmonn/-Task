#pragma once
#include <WinSock2.h>
#include <vector>

class Client
{
public:
	// コンストラクタ
	Client();
	// デストラクタ
	~Client();

	// 受信
	void Recv(void);

	// 送信
	void Sent(void);

private:
	// 初期処理
	int Start(void);

	// ソケットの生成
	int Create(void);

	// サーバーとの接続
	int Connect(void);

	// 初期化
	void Init(void);


	// データ
	WSADATA data;

	// ソケット
	SOCKET sock;

	// アドレス
	struct sockaddr_in addr;

	// サーバーのアドレス配列
	std::vector<unsigned long>list;

	// 
	fd_set fds;

	// 
	fd_set readfds;
};
