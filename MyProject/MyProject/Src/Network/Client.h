#pragma once
#include <WinSock2.h>
#include <string>
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
	void Send(void);

	// サーバーの認識フラグの取得
	bool GetFlag(void) const {
		return connecting;
	}

private:
	// 初期処理
	int Start(void);

	// ソケットの生成
	int Create(void);

	// サーバーとの接続
	int Connect(void);

	// 初期化
	void Init(void);

	// ソケットを閉じる
	void Close(void);


	// データ
	WSADATA data;

	// ソケット
	SOCKET sock;

	// アドレス
	struct sockaddr_in addr;

	// サーバーのアドレス配列
	std::vector<unsigned long>list;

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

	// サーバーの認識フラグ
	bool connecting;
};
