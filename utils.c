#include <string.h>
#include "utils.h"

char *get_content_type(char *filename)
{
    const char *ext = strrchr(filename, '.');
    if (!ext)
    {
        return "application/octet-stream";
    }
    else if (strcmp(ext, ".html") == 0)
    {
        return "text/html";
    }
    else if (strcmp(ext, ".css") == 0)
    {
        return "text/css";
    }
    else if (strcmp(ext, ".js") == 0)
    {
        return "application/javascript";
    }
    else if (strcmp(ext, ".png") == 0)
    {
        return "image/png";
    }
    else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
    {
        return "image/jpeg";
    }
    else if (strcmp(ext, ".json") == 0)
    {
        return "application/json";
    }

    return "application/octet-stream";
}
