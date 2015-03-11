//
//  httphandler.c
//  HttpServer
//
//  Created by Alexey Halaidzhy on 08.03.15.
//  Copyright (c) 2015 Alexey Halaidzhy. All rights reserved.
//

#include "httphandler.h"
#include "common.h"
#include "time.h"
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/uio.h>

typedef enum {GET, HEAD, UNSUPPORTED} http_method;
typedef enum {_09, _10, _11} http_version;
typedef enum {html, css, js, jpg, jpeg, png, gif, swf, other} file_type;

struct http_request {
    http_method method;
    char path[PATH_MAX + 1];
    http_version version;
};

int ishex(int x) {
    return	(x >= '0' && x <= '9')	||
            (x >= 'a' && x <= 'f')	||
            (x >= 'A' && x <= 'F');
}

int decode(const char *s, char *dec) {
    char *o;
    const char *end = s + strlen(s);
    int c;
    
    for (o = dec; s <= end; o++) {
        c = *s++;
        if (c == '+') {
            c = ' ';
        }
        else if (c == '%' && (	!ishex(*s++)	||
                              !ishex(*s++)	||
                              !sscanf(s - 2, "%2x", &c))) {
            return -1;
        }
        
        if (dec) {
            *o = c;
        }
    }
    
    return (int)o - (int)dec;
}

char *get_version(http_version version) {
    switch (version) {
        case _09:
            return "HTTP/0.9 ";
            
        case _10:
            return "HTTP/1.0 ";
            
        case _11:
            return "HTTP/1.1 ";
    }
}

char *get_status(http_status code) {
    switch (code) {
        case _200:
            return "200 OK\r\n";

        case _400:
            return "400 Bad Request\r\n";

        case _403:
            return "403 Forbidden\r\n";

        case _404:
            return "404 Not Found\r\n";
    
        case _405:
            return "405 Method Not Allowed\r\n";
    }
}

char *get_date_header() {
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
    char res[1000];
    strcpy(res, "Date: ");
    return strcat(res, buf);
}

char *get_server_header() {
    return "Server: Highload-Khalaydzhi-server (Technopark@Mail.Ru)\r\n";
}

char *get_content_length(size_t len) {
    char str_len[256];
    snprintf(str_len, sizeof(str_len), "%zu\r\n", len);
    char res[256];
    strcpy(res, "Content-Length: ");
    return strcat(res, str_len);;
}

file_type get_file_type(char *file_path) {
    char *pos = file_path;
    char *dot = strchr(file_path, '.');
    if (dot) {
        pos = dot + 1;
        while ((dot = strchr(pos, '.'))) {
            pos = dot + 1;
        }
        if (!strcmp(pos, "html")) {
            return html;
        }
        if (!strcmp(pos, "css")) {
            return css;
        }
        if (!strcmp(pos, "js")) {
            return js;
        }
        if (!strcmp(pos, "jpg")) {
            return jpg;
        }
        if (!strcmp(pos, "jpeg")) {
            return jpeg;
        }
        if (!strcmp(pos, "png")) {
            return png;
        }
        if (!strcmp(pos, "gif")) {
            return gif;
        }
        if (!strcmp(pos, "swf")) {
            return swf;
        }
    }
    return other;
}

char *get_content_type(file_type type) {
    switch (type) {
        case html:
            return "Content-Type: text/html\r\n";

        case css:
            return "Content-Type: text/css\r\n";

        case js:
            return "Content-Type: application/javascript\r\n";

        case jpg:
            return "Content-Type: image/jpeg\r\n";

        case jpeg:
            return "Content-Type: image/jpeg\r\n";

        case png:
            return "Content-Type: image/png\r\n";

        case gif:
            return "Content-Type: image/gif\r\n";

        case swf:
            return "Content-Type: application/x-shockwave-flash\r\n";

        default:
            return "Content-Type: application/text\r\n";
    }
}

char *get_connection_header() {
    return "Connection: close\r\n";
}

void generate_response(struct http_request *req, struct http_response *resp, char **headers) {
    *headers = (char *)calloc(1000, sizeof(char));
    strcat(*headers, get_version(req->version));
    if (resp->status != _200) {
        strcat(*headers, get_status(resp->status));
    }
    else {
        char temp[PATH_MAX];
        strcpy(temp, ROOTDIR);
        strcat(temp, req->path);
        if (!access(temp, F_OK) && temp[strlen(temp) - 1] == '/') {
            struct stat s;
            int fd = open(temp, O_RDONLY);
            fstat(fd, &s);
            close(fd);
            if (S_ISDIR(s.st_mode)) {
                strcpy(temp, ROOTDIR);
                strcat(temp, req->path);
                strcat(temp, "index.html");
                if (!access(temp, R_OK)) {
                    strcat(req->path, "index.html");
                }
                else {
                    resp->status = _403;
                    strcat(*headers, get_status(resp->status));
                }
            }
            else {
                resp->status = _404;
                strcat(*headers, get_status(resp->status));
            }
        }
        if (resp->status == _200) {
            strcpy(temp, ROOTDIR);
            strcat(temp, req->path);
            if (!access(temp, R_OK)) {
                struct stat s;
                int fd = open(temp, O_RDONLY);
                fstat(fd, &s);
                if (S_ISREG(s.st_mode)) {
                    strcat(*headers, get_status(resp->status));
                    if (req->method == GET) {
                        resp->data_fd = fd;
                        if (DEBUG_MODE) {
                            printf("File desctiptor: %d\n", resp->data_fd);
                        }
                        resp->data_size = s.st_size;
                        strcat(*headers, get_content_length(s.st_size));
                        file_type type = get_file_type(req->path);
                        strcat(*headers, get_content_type(type));
                    }
                    else {
                        strcat(*headers, get_content_length(s.st_size));
                        close(fd);
                    }
                }
                else {
                    close(fd);
                    resp->status = _404;
                    strcat(*headers, get_status(resp->status));
                }
            }
            else if (access(temp, F_OK)) {
                resp->status = _404;
                strcat(*headers, get_status(resp->status));
            }
            else {
                resp->status = _403;
                strcat(*headers, get_status(resp->status));
            }
        }
    }
    if (resp->status != _200) {
        strcat(*headers, get_content_length(0));
    }
    strcat(*headers, get_date_header());
    strcat(*headers, get_server_header());
    strcat(*headers, get_connection_header());
    strcat(*headers, "\r\n");
}

off_t do_sendfile(int out_fd, int in_fd, off_t offset, off_t count) {
    off_t bytes_sent;
    off_t total_bytes_sent = 0;
    printf("arght: %d", in_fd);
    if (in_fd != -1) {
        while (total_bytes_sent < count) {
            off_t len = count - total_bytes_sent;
            if ((bytes_sent = sendfile(in_fd, out_fd, offset, &len, NULL, 0)) <= 0) {
                if (errno == EINTR || errno == EAGAIN) {
                    //  Interrupted system call/try again
                    //  Just skip to the top of the loop abd try again
                    continue;
                }
                printf("Sendfile error, errno = %d", errno);
                close(in_fd);
                return -1;
            }
            total_bytes_sent += bytes_sent;
        }
        close(in_fd);
    }
    else {
        printf("HI");
        send(out_fd, "", 0, 0);
    }
    return total_bytes_sent;
}

char *delete_query_string(char *url) {
    char *result = calloc(strlen(url), sizeof(char));
    char *pos = strchr(url, '?');
    if (pos) {
        strncpy(result, url, pos - url);
        result[pos - url] = '\0';
    }
    else {
        strcpy(result, url);
    }
    return result;
}

void **get_response(char *request) {
    char *token, *first_line, *decoded_path;
    struct http_request req;
    strcpy(req.path, "");
    struct http_response *resp = calloc(1, sizeof(struct http_response));
    first_line = strtok(request, "\r\n");
    token = strtok(first_line, " ");
    resp->status = _200;
    if (!strcmp(token, "GET")) {
        req.method = GET;
    }
    else if (!strcmp(token, "HEAD")) {
        req.method = HEAD;
    }
    else {
        req.method = UNSUPPORTED;
        resp->status = _405;
    }
    token = strtok(NULL, " ");
    decoded_path = (char *)calloc(strlen(token) + 1, sizeof(char));
    if (decode(token, decoded_path) < 0 || token[0] != '/') {
        //  bad string
        resp->status = _400;
    }
    else {
        if (strstr(decoded_path, "../")) {
            resp->status = _403;
        }
        else {
            char *res = delete_query_string(decoded_path);
            strcpy(req.path, res);
            free(res);
        }
    }
    free(decoded_path);
    token = strtok(NULL, " ");
    if (!strcmp(token, "HTTP/0.9")) {
        req.version = _09;
    }
    else if (!strcmp(token, "HTTP/1.0")) {
        req.version = _10;
    }
    else if (!strcmp(token, "HTTP/1.1")) {
        req.version = _11;
    }
    else {
        resp->status = _400;
    }
    char *response_headers = NULL;
    resp->data_fd = -1;
    generate_response(&req, resp, &response_headers);
    void **result = (void **)malloc(2 * sizeof(void *));
    result[0] = response_headers;
    result[1] = resp;
    return result;
}
