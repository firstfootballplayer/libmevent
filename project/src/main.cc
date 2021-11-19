// /*=============================================================================
// #     FileName: tcpservselect.c
// #         Desc: receive client data and then send they back.
// #       Author: Licaibiao
// #   LastChange: 2017-02-12
// =============================================================================*/
// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <errno.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <signal.h>
// #define MAXLINE 1024
// #define LISTENLEN 10
// #define SERV_PORT 8000
// int main(int argc, char** argv)
// {
//     int i, maxi, maxfd, listenfd, connfd, sockfd;
//     int nready, client[FD_SETSIZE];
//     ssize_t n;
//     fd_set rset, allset, wset;
//     char buf[MAXLINE];
//     socklen_t clilen;
//     struct sockaddr_in cliaddr, servaddr;

//     listenfd = socket(AF_INET, SOCK_STREAM, 0);

//     bzero(&servaddr, sizeof(servaddr));
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//     servaddr.sin_port = htons(SERV_PORT);
//     int optval = 1;
//     setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
//     bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

//     listen(listenfd, LISTENLEN);

//     maxfd = listenfd; /* initialize */
//     maxi = -1;        /* index into client[] array */
//     for(i = 0; i < FD_SETSIZE; i++)
//         client[i] = -1; /* -1 indicates available entry */
//     for(i = 0; i < FD_SETSIZE; i++)
//         if(client[i] < 0) {
//             client[i] = listenfd; /* save descriptor */
//             break;
//         }
//     FD_ZERO(&allset);
//     FD_SET(listenfd, &allset);
//     FD_ZERO(&wset);
//     FD_SET(listenfd, &wset);

//     for(;;) {
//         rset = allset; /* structure assignment */
//         nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
//         // printf("nready%d\n", nready);
//         if(FD_ISSET(listenfd, &rset)) /* new client connection */
//         {
//             clilen = sizeof(cliaddr);
//             connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
//             char buf[1024];
//             // printf("new client: %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buf, 1024), ntohs(cliaddr.sin_port));

//             for(i = 0; i < FD_SETSIZE; i++)
//                 if(client[i] < 0) {
//                     client[i] = connfd; /* save descriptor */
//                     break;
//                 }
//             if(i == FD_SETSIZE) {
//                 printf("too many clients\n");
//                 exit(0);
//             }

//             FD_SET(connfd, &allset); /* add new descriptor to set */
//             // FD_SET(connfd, &wset);
//             if(connfd > maxfd)
//                 maxfd = connfd; /* for select */
//             if(i > maxi)
//                 maxi = i; /* max index in client[] array */

//             if(--nready <= 0)
//                 continue; /* no more readable descriptors */
//         }

//         for(i = 0; i <= maxi; i++) /* check all clients for data */
//         {
//             // printf("maxi %d\n", maxi);
//             if((sockfd = client[i]) < 0)
//                 continue;
//             if(FD_ISSET(sockfd, &rset) && sockfd != listenfd) {
//                 // printf("rset %d\n", sockfd);
//                 if((n = read(sockfd, buf, MAXLINE)) == 0) /* connection closed by client */
//                 {
//                     close(sockfd);
//                     FD_CLR(sockfd, &allset);
//                     client[i] = -1;
//                 } else {
//                     write(sockfd, buf, n);
//                     close(sockfd);
//                     FD_CLR(sockfd, &allset);
//                     client[i] = -1;
//                 }
//                 if(--nready <= 0)
//                     break; /* no more readable descriptors */
//             }
//             // if(FD_ISSET(sockfd, &wset)) {
//             //     printf("wset %d\n", sockfd);
//             //     FD_CLR(sockfd, &wset);
//             // }
//         }
//     }
// }
