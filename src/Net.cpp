#include "Net.h"
#include <iostream>
#include <vector>
using namespace std;



int open_clientfd(const char*hostname, const char*port)
{
    int clientfd;
    struct addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;

    int ans = getaddrinfo(hostname, port, &hints, &res);

    if( ans != 0 )
    {
        fprintf(stderr, "getaddrinfo error:%s\n", gai_strerror(ans));
        exit(1);
    }
    
    for(p = res; p; p = p->ai_next)
    {
        if( (clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0 )
            continue;
        if( connect(clientfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
        close(clientfd);
    }

    freeaddrinfo(res);
    if(!p) return -1;
    else return clientfd;
}

int open_listentfd(const char*port)
{
    int listenfd, optval=1;
    struct addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;
    int ans = getaddrinfo(nullptr, port, &hints, &res);

    if( ans != 0 )
    {
        fprintf(stderr, "getaddrinfo error:%s\n", gai_strerror(ans));
        exit(1);
    }

    for(p = res; p; p = p->ai_next)
    {
        if( (listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0 )
            continue;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                (const void*)&optval, sizeof(int));
        if( bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
        close(listenfd);
    }

    freeaddrinfo(res);
    if(!p)
    {
        fprintf(stderr, "cannot find a suitable socket\n");
        return -1;
    }
    
    if( (ans=listen(listenfd, MAX_CONNECT)) < 0)
    {
        close(listenfd);
        fprintf(stderr, "open_listenfd error:%s", gai_strerror(ans));
        return -1;
    }
    return listenfd;
}



RIO::RIO(int fd)
{
    rio_fd = fd;
    rio_bufptr = rio_buf;
    rio_cnt = 0;
}

Request RIO::read_request()
{
    string start_line = rio_RecvLineb();
    vector<string> headers = read_requesthdrs();
    Request request(start_line, headers);
    
    string content = read_requestCnts(atoi(request["Content-Length"].c_str()));
    request.setContent(content);

    return request;
}

vector<string> RIO::read_requesthdrs()
{
    vector<string> headers;
    string head = rio_RecvLineb();
    while(strcmp(buf, "\r\n"))
    {
        headers.push_back(head);
        //std::cout << head;
        head = rio_RecvLineb();
    };
    return headers;
}

string RIO::read_requestCnts(size_t length)
{
    auto n = rio_recvNb(buf, length, 0);
    buf[length] = '\0';
    if( n < 0 )
    {
        fprintf(stderr, "recvNb error:%s\n", gai_strerror(n));
        exit(1);
    }
    return string(buf);
}

ssize_t RIO::rio_recvLineb(void *usrbuf, size_t maxlen, int flags)
{
    int n, rc;
    char c, *bufp = (char *)usrbuf;

    for(n=1; n < maxlen; ++n)
    {
        if( (rc = rio_recv(&c, 1, flags)) == 1)
        {
            *bufp++ = c;
            if( c == '\n' )
            {
                ++n;
                break;
            }
        }
        else if( rc == 0 ) break;
        else return -1;
    }
    *bufp = 0;
    return n - 1;
}

string RIO::rio_RecvLineb(int flags)
{
    auto res = rio_recvLineb(buf, MAX_LINE, flags);
    if( res == -1)
    {
        fprintf(stderr, "recv error:%s\n", gai_strerror(res));
        pthread_exit(nullptr);
    }
    else if( res == 0 )
    {
        printf("shutting down connection...\n");
        pthread_exit(nullptr);
    }
    return string(buf);
}
ssize_t RIO::rio_recvNb(void *usrbuf, size_t n, int flags)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char *)usrbuf;

    while( nleft > 0 )
    {
        if( (nread = rio_recv(bufp, nleft, flags)) < 0 ) return -1;
        else if( nread == 0 ) break;
        nleft -= nread;
        bufp += nread;
    }
    return (n-nleft);
}
ssize_t RIO::rio_send(const void *buf, size_t n, int flags)
{
    auto left = n;
    ssize_t nwritten;
    char *bufptr = (char *)buf;

    while(left > 0)
    {
        if( (nwritten = send(rio_fd, bufptr, left, flags)) <=0 )
        {
            if( errno == EINTR ) nwritten = 0;
            else return -1;
        }
        left -= nwritten;
        bufptr += nwritten;

    }
    return n;
}
ssize_t RIO::rio_recv(void *usrbuf, size_t n, int flags)
{
    while(rio_cnt <= 0)
    {
        rio_cnt = recv(rio_fd, rio_buf, sizeof(rio_buf), flags);
        if( rio_cnt < 0)
        {
            if( errno != EINTR ) return -1;
        }
        else if(rio_cnt == 0) return 0;
        else rio_bufptr = rio_buf;
    }
    int cnt = n;
    if(rio_cnt < n) cnt = rio_cnt;
    memcpy(usrbuf, rio_bufptr, cnt);
    rio_bufptr += cnt;
    rio_cnt -= cnt;

    return cnt;
}