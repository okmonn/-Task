#pragma once
#include <WinSock2.h>
#include <vector>

class Sock
{
public:
	// コンストラクタ
	Sock();
	// デストラクタ
	virtual ~Sock();

protected:
	// スタートアップ
	int StartUp(void);

	// ソケットの生成
	int CreateSock(void);

	// 通信の終了
	void Finish(void);


	// データ
	WSADATA data;

	// 自分のソケット
	SOCKET sock;

	// 接続先アドレス
	sockaddr_in addr;

	// セレクトで使用
	fd_set fds;

	// セレクトで使用
	fd_set readfds;

	// タイムアウト
	timeval time;
};
