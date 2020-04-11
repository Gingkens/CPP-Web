#include "Net.h"
#include "session.h"
#include "HttpCookie.h"
#include "Response.h"

#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

typedef struct sockaddr  SA;

void *doit(void *vargp);
void process(RIO &rp, Request &request);
void serve_static(RIO &rp, const char *filename, int filesize);
void serve_dynamic(int fd, const char *filename, const char *cgiargs, const char * cookie);

int main(int argc, char *argv[])
{
    struct sockaddr_storage clientaddr;
    socklen_t clientlen;
    char hostname[MAX_SIZE], port[MAX_SIZE];
    pthread_t tid;

    int listenfd = open_listentfd(argv[1]), *clientfd, connfd;

    if(listenfd < 0) return -1;
 
    chdir("/home/gingken/Workspace/Web_D");

    printf("Starting the server...\n\n");

    while(1)
    {
        clientfd = (int *)malloc(sizeof(int));
        *clientfd = accept(listenfd, (SA*)&clientaddr, &clientlen);

        getnameinfo((SA *)&clientaddr, clientlen, hostname, MAX_SIZE, port, MAX_SIZE, 0);
        printf("accpet connection from (%s, %s) , fd : %d\n", hostname, port, *clientfd);
        pthread_create(&tid, nullptr, doit, clientfd);
    }
    return 0;
}

void *doit(void *vargp)
{
    int connfd = *((int *)vargp);
    pthread_detach(pthread_self());
    free(vargp);

    RIO crio(connfd);

    while(1)
    {
        cout << "waitting for " << connfd << '\n';
        Request request = crio.read_request();                        //读取HTTP消息头第一行

        process(crio, request);
    }
    close(connfd);
}

void process(RIO &rp, Request &request)
{
    struct stat sbuf;
    Response response;
//    Session sess(request["Cookie"]);
    
//    cout << "request file:" << request.getFilename() << '\n';

    if( stat(request.getFilename().c_str(), &sbuf) < 0 )         //404
    {
        stat("404.html", &sbuf);
        response.init(404, "Not Found", "404.html", "text/html", sbuf.st_size);
    }  
    else if(!strcmp(request.getMethod().c_str(), "GET"))
        response.init(200, "OK", request.getFilename() ,request.getFiletype(), sbuf.st_size);
    else if(!strcmp(request.getMethod().c_str(), "POST"))            //重定向
    {       
        response.Redirect("/");
        HttpCookie cookie("session", request.getData("name"));
        // cookie.setExpires();
        response.addCookie(cookie);
        rp.rio_send(response.getResponse().c_str(), response.size(), 0);
        return;

    }
    else                                        //未实现 501
    {
        // strcpy(filename, "501.html");
        // strcpy(filetype, "text/html");
        // stat(filename, &sbuf);
        // response.init(501, "Not Implemented", filetype, sbuf.st_size);

        // head = header(501, "Not Implemented", sbuf.st_size, filetype, infos);
    }

    if( request.Is_static())
    {
        rp.rio_send(response.getResponse().c_str(), response.size(), 0);
        serve_static(rp, response.getResponseFile().c_str(), sbuf.st_size);
    }
    else
    {
        serve_dynamic(rp.get_fd(), request.getFilename().c_str(), \
            request.getArgs().c_str(), request["Cookie"].c_str());
    }
    
}

void serve_dynamic(int fd, const char *filename, const char *cgiargs, const char * cookie)
{
    const char  *emptylist[] = {filename, cgiargs, cookie, NULL};  

    if(fork() == 0)
    {
        dup2(fd, STDOUT_FILENO);
        execve(filename, (char * const*)emptylist, environ);
    }

}

void serve_static(RIO &rp, const char *filename, int filesize)
{
    
    int reqfd = open(filename, O_RDONLY, 0);
    char *srcp = (char *)mmap(0, filesize, PROT_READ, MAP_PRIVATE, reqfd, 0);
    close(reqfd);
    rp.rio_send(srcp, strlen(srcp), 0);
    munmap(srcp, filesize);

    //cout << "send content:" << strlen(srcp) << '\n';
}