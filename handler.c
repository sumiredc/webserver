#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "handler.h"
#include "utils.h"

#define BUF_SIZE 8192

void html_response(int client_fd, char *buffer)
{
    char method[8], path[256];
    sscanf(buffer, "%s %s", method, path); // buffuer から method と path を取得

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "./html/public%s", path); // path に該当する filepath に置き換え

    // 末尾がスラッシュの場合、/index.html を探す
    if (path[strlen(path) - 1] == '/')
    {
        strncat(filepath, "index.html", sizeof(filepath) - strlen(filepath) - 1);
    }

    int status;
    char message[256];
    long filesize;
    char header[512];

    FILE *fp = fopen(filepath, "rb"); // パスに該当するファイルの存在確認
    if (fp == NULL)
    {
        // 該当ファイルが見つからないので Not Found をセットする
        strcpy(filepath, "./html/errors/not-found.html");
        fp = fopen(filepath, "rb");

        status = 404;
        strcpy(message, "Not Found");
    }
    else
    {
        status = 200;
        strcpy(message, "OK");
    }

    char *content_type = get_content_type(filepath); // Content-Type の値を取得

    fseek(fp, 0, SEEK_END); // ファイルの末尾へ移動
    filesize = ftell(fp);   // 現在位置を取得 = ファイルサイズ
    rewind(fp);             // ファイルの先頭へ移動

    // Header を生成
    snprintf(header, sizeof(header),
             "HTTP/1.1 %d %s\r\n"
             "Content-Length: %ld\r\n"
             "Content-Type: %s\r\n"
             "\r\n",
             status, message, filesize, content_type);

    write(client_fd, header, strlen(header)); // Header を書き込み

    char filebuf[1024];
    size_t n;

    // ファイルを読み込みながら書き込み
    while ((n = fread(filebuf, 1, sizeof(filebuf), fp)) > 0)
    {
        write(client_fd, filebuf, n);
    }

    fclose(fp); // ファイルを閉じる
}

void handle_client(int client_fd)
{
    const char *ok = "HTTP/1.1 200 OK\r\n\r\n";
    const char *created = "HTTP/1.1 201 Created\r\n\r\n";
    const char *no_content = "HTTP/1.1 204 No Content\r\n\r\n";
    const char *bad_request = "HTTP/1.1 400 Bad Request\r\n\r\n";

    char buffer[BUF_SIZE];
    read(client_fd, buffer, BUF_SIZE - 1);

    if (strncmp(buffer, "GET ", 4) == 0)
    {
        html_response(client_fd, buffer);
        close(client_fd);
        return;
    }
    else if (strncmp(buffer, "POST ", 5) == 0)
    {
        write(client_fd, created, strlen(created));
        close(client_fd);
        return;
    }
    else if (strncmp(buffer, "PUT ", 4) == 0)
    {
        write(client_fd, ok, strlen(ok));
        close(client_fd);
        return;
    }
    else if (strncmp(buffer, "PATCH ", 6) == 0)
    {
        write(client_fd, ok, strlen(ok));
        close(client_fd);
        return;
    }
    else if (strncmp(buffer, "DELETE ", 7) == 0)
    {
        write(client_fd, no_content, strlen(no_content));
        close(client_fd);
        return;
    }

    write(client_fd, bad_request, strlen(bad_request));
    close(client_fd);
    return;
}
