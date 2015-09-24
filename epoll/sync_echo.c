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
#define SERV_PORT	9999

static int epfd = 0;
static int serv_fd = 0;

int main_loop();
int create_server();
int accept_client();
int handle_in(int fd);

void setnonblocking(int sock) {
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


int main(int argc, char *argv[]) {
    /* listen 256 fds maximum*/
    epfd = epoll_create(256);
    create_server();
    main_loop();
    return 0;
}

int create_server() {
    struct sockaddr_in addr;
    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(SERV_PORT);
    socklen_t sock_len = sizeof(addr);
    bind(serv_fd, (struct sockaddr*)&addr, sock_len);
    listen(serv_fd, 10);
    setnonblocking(serv_fd);
    struct epoll_event ev;
    ev.data.fd = serv_fd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_fd, &ev);
    return 0;
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
    return 0;
}

int handle_in(int fd) {
    char buf[1024] = {0};
    if (fd == serv_fd) {
        accept_client();
        return 0;
    }
    /* send immedately after recv, so I call it synchronized */
    int len = recv(fd, buf, 6, 0);
    send(fd, buf, len, 0);
    return 0;
}

int main_loop() {
    struct epoll_event evs[20];
    char buf[1024]  = {0};
    int nfds        = 0;
    int len         = 0;
    while (1) {
        nfds    = 0;
        len     = 0;
        memset(buf, 0x00, 1024);
        nfds = epoll_wait(epfd, evs, 20, 3000);
        int i = 0;
        for (i = 0; i < nfds; ++i) {
            if (evs[i].events & EPOLLIN) {
                handle_in(evs[i].data.fd);
            }
        }
    }
    return 0;
}
