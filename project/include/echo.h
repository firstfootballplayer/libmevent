#ifndef libmevent_EXAMPLES_SIMPLE_ECHO_ECHO_H
#define libmevent_EXAMPLES_SIMPLE_ECHO_ECHO_H

// #include "libmevent/net/TcpServer.h"

// RFC 862
// class EchoServer
// {
// public:
//     EchoServer(libmevent::net::EventLoop* loop, const libmevent::net::InetAddress& listenAddr);

//     void start();  // calls server_.start();

// private:
//     void onConnection(const libmevent::net::TcpConnectionPtr& conn);

//     void onMessage(const libmevent::net::TcpConnectionPtr& conn, libmevent::net::Buffer* buf, libmevent::Timestamp time);

//     libmevent::net::TcpServer server_;
// };

void send2();
#endif  // libmevent_EXAMPLES_SIMPLE_ECHO_ECHO_H
