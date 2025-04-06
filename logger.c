#include <stdio.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include "logger.h"

void write_access_log(struct sockaddr_in *addr, const char *request_line)
{
    // IP アドレスの取得
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), client_ip, INET_ADDRSTRLEN);

    // 現在時刻の取得
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_buf[64];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    // 追記モードでログファイルを開く（なければ新規作成）
    FILE *fp = fopen("./log/access.log", "a");
    if (fp == NULL)
    {
        perror("fopen");
        return;
    }

    // ログ書き込み
    fprintf(fp, "[%s] %s \"%s\"\n", time_buf, client_ip, request_line);
    fclose(fp);
}
