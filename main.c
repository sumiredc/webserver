#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "handler.h"

#define PORT 8080
#define ERR_SOCKET -1

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;          // IPv4 用のソケットアドレス構造体を宣言
    socklen_t addrlen = sizeof(addr); // アドレス情報取得時に渡す際に必要なサイズを事前に取得

    addr.sin_family = AF_INET;         // IPv4
    addr.sin_addr.s_addr = INADDR_ANY; // 任意の IR アドレスを受け付ける
    addr.sin_port = htons(PORT);       // 待機するポート番号の指定

    server_fd = socket(AF_INET, SOCK_STREAM, 0); // ソケットの作成 → ソケットのファイルディスクリプタが返却される
    if (server_fd == ERR_SOCKET)
    {
        printf("failed to create socket\n");
        return 1;
    }

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)); // ソケットにIPアドレスとポート番号を紐づける
    listen(server_fd, 5);                                    // ソケットを「接続待ち状態」にする(第2引数で接続待ちキューの数を指定する)
    printf("Listening on port %d...\n", PORT);

    while (1)
    {
        // accept: クライアントの接続を受け入れる（接続があるまで待機状態になる）
        // → クライアントが接続すると、新しいソケットを作成してファイルディスクリプタを返却する
        client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);

        if (client_fd >= 0)
        {
            handle_client(client_fd);
        }
    }

    close(server_fd);

    return 0;
}
