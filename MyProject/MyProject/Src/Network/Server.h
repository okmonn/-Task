#pragma once
#include <WinSock2.h>
#include <string>
#include <vector>

class Server
{
public:
	// コンストラクタ
	Server();
	// デストラクタ
	~Server();

	// クライアントとの接続
	void Accept(void);

	// 受信
	void Recv(void);

	// 送信
	void Send(void);

private:
	// 初期処理
	int Start(void);

	// ソケットの生成
	int Create(void);

	// アドレスとの関連
	int Bind(void);

	// クライアントとの接続待機
	int Listen(void);

	// 初期化
	void Init(void);

	// クライアントとの接続終了
	void Close(void);


	// データ
	WSADATA data;

	// サーバーソケット
	SOCKET sock;

	// アドレス
	struct sockaddr_in addr;

	// クライアントソケット
	std::vector<SOCKET>client;

	// クライアントアドレス
	std::vector<struct sockaddr_in>clientAddr;

	// セレクトで使用
	fd_set fds;

	// セレクトで使用
	fd_set readfds;

	// タイムアウト
	struct timeval time;

	// 受信文字
	std::string r;

	// 送信文字
	std::string s;
};
