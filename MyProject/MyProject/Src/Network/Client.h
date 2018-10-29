#pragma once
#include <winsock2.h>
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

	// 終了フラグの取得
	bool GetFlag(void) const {
		return end;
	}

private:
	// テキストの読み込み
	int LoadText(const std::string& fileName);

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


	// サーバーPC名
	std::string serverName;

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

	// 終了フラグ
	bool end;

	// 受信
	char r[256];

	// 送信
	char s[256];

	// アドレスリスト
	std::vector<unsigned long>list;
};
