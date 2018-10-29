#pragma once
#include <winsock2.h>
#include <map>

class Server
{
public:
	// コンストラクタ
	Server();
	// デストラクタ
	~Server();

private:
	// 初期処理
	int Start(void);

	// ソケットの生成
	int Create(void);


	// データ
	WSADATA data;

	// サーバーソケット
	SOCKET sock;

	// アドレス
	struct sockaddr_in addr;

	// セレクトで使用
	fd_set fds;

	// セレクトで使用
	fd_set readfds;

	// タイムアウト
	struct timeval time;

	// クライアントソケット
	std::map<unsigned long, SOCKET>client;

	// クライアントアドレス
	std::map<unsigned long, struct sockaddr_in>clientAddr;
};
