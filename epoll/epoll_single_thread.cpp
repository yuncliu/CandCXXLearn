
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <set>
using namespace std;

#define MAXLINE		10
#define OPEN_MAX	100
#define LISTENQ		20
#define SERV_PORT	5555
#define INFTIM		1000

static int epfd = 0;
static int serv_fd = 0;
static int client_fd = 0;
static int client_connect_flag = 0;
static set<int> clients;
static queue<string> q;

int main_loop();
int create_client();
int create_server();
int accept_client();
int handle_in(int fd);
int handle_out(int fd);

inline int set_sent(int fd, bool b) {
    struct epoll_event ev;
    ev.data.fd = fd;
    if (b) {
        ev.events = EPOLLIN | EPOLLOUT;
    } else {
        ev.events = EPOLLIN;
    }
    return epoll_ctl(epfd, EPOLL_CTL_MOD, fd , &ev);
}

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);

    if(opts < 0) {
        perror("fcntl(sock, GETFL)");
        exit(1);
    }

    opts = opts | O_NONBLOCK;

    if(fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(sock, SETFL, opts)");
        exit(1);
    }
}


int main(int argc, char *argv[])
{
    epfd = epoll_create(256);
    create_client();
    create_server();
    main_loop();
    return 0;
}

int create_client() {
    struct sockaddr_in serveraddr;
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct epoll_event ev;
    ev.data.fd = client_fd;
    ev.events = EPOLLIN | EPOLLOUT; //EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

    bzero(&serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(1060);
    socklen_t sock_len =  sizeof(serveraddr);

    setnonblocking(client_fd);
    connect(client_fd, (struct sockaddr*)&serveraddr, sock_len);
    return 0;
}

int create_server() {
    struct sockaddr_in addr;
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9999);
    socklen_t sock_len = sizeof(addr);
    bind(serv_fd, (struct sockaddr*)&addr, sock_len);
    listen(serv_fd, 10);
    setnonblocking(serv_fd);

    struct epoll_event ev;
    ev.data.fd = serv_fd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_fd, &ev);
}

int accept_client() {
    struct sockaddr_in addr;
    socklen_t sock_len = sizeof(addr);
    int client_fd = accept(serv_fd, (struct sockaddr*)&addr, &sock_len);

    struct epoll_event ev;
    ev.data.fd = client_fd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);
    printf("get a client\n");
    clients.insert(client_fd);
    return 0;
}

int handle_in(int fd) {
    char buf[1024] = {0};
    if (fd == serv_fd) {
        accept_client();
        return 0;
    }
    else if (fd == client_fd) {
        recv(fd, buf, 1024, 0);
    }
    else if (clients.find(fd) != clients.end())
    {
        recv(fd, buf, 6, 0);
        string a(buf);
        q.push(a);
        set_sent(client_fd, true);
    }
    return 0;
}

int handle_out(int fd) {
    if (fd == client_fd && 0 == client_connect_flag) {
        printf("connect complished\n");
        client_connect_flag = 1;
        return set_sent(fd, false);
    }

    if (q.empty()) {
        set_sent(client_fd, false);
        return 0;
    }
    string buf = q.front();
    q.pop();

    send(client_fd, buf.c_str(), buf.size(), 0);

    if (q.empty()) {
        set_sent(client_fd, false);
    }
    return 0;
}


int main_loop() {
    struct epoll_event evs[20];
    char buf[1024] = {0};
    int nfds = 0;
    int len = 0;
    int c = 0;
    while(1) {
        c++;
        if (c > 10000) {
            printf("queue size = %ld\n", q.size());
            c = 0;
        }
        nfds = 0;
        len = 0;
        memset(buf, 0x00, 1024);
        nfds = epoll_wait(epfd, evs, 20, 3000);
        int i = 0;
        for(i = 0; i < nfds; ++i) {
            if (evs[i].events & EPOLLOUT)
            {
                handle_out(evs[i].data.fd);
            }
            else if(evs[i].events & EPOLLIN) {
                handle_in(evs[i].data.fd);
            }
        }
   }
}
