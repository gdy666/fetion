/*
 * sock.h
 *
 *  Created on: 2013年8月6日
 *
 *  From: https://github.com/lattera/libsock
 */

#ifndef SOCK_H_
#define SOCK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <errno.h>

#define EXPORTED_SYM __attribute__((visibility("default")))

#define SOCK_FLAG_NONE          0x00000000
#define SOCK_FLAG_ACTIVE        0x00000001
#define SOCK_FLAG_KEVENT_ACTIVE 0x00000002

#define SOCK_TCP    SOCK_STREAM
#define SOCK_UDP    SOCK_DGRAM

typedef struct _sock {
    int sockfd;
    unsigned int flags;
    int protocol;

    /* For UDP */
    struct addrinfo *servinfo;
    struct addrinfo *addrinfo;
    char *ipaddr;
} SOCK;

SOCK *SocketClient(const char *hostname, const char *port, int protocol);
ssize_t SocketWrite(SOCK *sock, const void *data, size_t sz);
ssize_t SocketRead(SOCK *sock, void *data, size_t sz);
void SocketClose(SOCK *sock);
ssize_t udp_read(SOCK *sock, void *data, size_t sz);

SOCK *get_sock_client(const char *host, const char *port, int protocol);
char *get_udp_packet_ip(SOCK *s, void *addrinfo);

#endif /* SOCK_H_ */
