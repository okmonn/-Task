#pragma once
#include <winsock2.h>
#include <string>
#include <vector>

class Server
{
public:
	// コンストラクタ
	Server();
	// デストラクタ
	~Server();

	// クライアントの接続
	void Accept(void);

	// 受信
	void Recv(void);

	// 送信
	void Send(void);

private:
	// テキストの読み込み
	int LoadText(const std::string& fileName);

	// 初期処理
	int Start(void);

	// ソケットの生成
	int Create(void);

	// アドレスとの関連付け
	int Bind(void);

	// クライアントとの接続待機
	int Listen(void);

	// 初期化
	void Init(void);

	// クライアントとの接続終了
	void Close(void);


	// ポート番号
	std::string port;

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

	// 受信
	char r[256];

	// 送信
	char s[256];

	// クライアントソケット
	std::vector<SOCKET>client;

	// クライアントアドレス
	std::vector<struct sockaddr_in>clientAddr;
};
