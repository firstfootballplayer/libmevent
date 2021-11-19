// #include "libmevent/net/TcpServer.h"

// #include "libmevent/base/Logging.h"
// #include "libmevent/base/Thread.h"
// #include "libmevent/net/EventLoop.h"
// #include "libmevent/net/InetAddress.h"
// #include "libmevent/base/LogFile.h"

// #include <utility>

// #include <stdio.h>
// #include <unistd.h>

// using namespace libmevent;
// using namespace libmevent::net;

// int numThreads = 0;
// std::unique_ptr<libmevent::LogFile> g_logFile;
// void outputFunc(const char* msg, int len)
// {
//     g_logFile->append(msg, len);
// }

// void flushFunc()
// {
//     g_logFile->flush();
// }

// class EchoServer
// {
// public:
//     EchoServer(EventLoop* loop, const InetAddress& listenAddr) : loop_(loop), server_(loop, listenAddr, "EchoServer")
//     {
//         server_.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));
//         server_.setMessageCallback(std::bind(&EchoServer::onMessage, this, _1, _2, _3));
//         server_.setThreadNum(numThreads);
//     }

//     void start()
//     {
//         server_.start();
//     }
//     // void stop();

// private:
//     void onConnection(const TcpConnectionPtr& conn)
//     {
//         // LOG_TRACE << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " is " << (conn->connected() ? "UP" : "DOWN");
//         // LOG_INFO << conn->getTcpInfoString();

//         // conn->send("hello\n");
//     }

//     void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
//     {
//         static int c = 0;
//         // printf("jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj\n");
//         string msg(buf->retrieveAllAsString());
//         // LOG_INFO << " recv fd:  " << conn->fd();
//         LOG_INFO << c++;
//         if(msg == "exit\n") {
//             conn->send("bye\n");
//             conn->shutdown();
//         }
//         if(msg == "quit\n") {
//             loop_->quit();
//         }
//         conn->send(msg);
//         // conn->forceClose();
//     }

//     EventLoop* loop_;
//     TcpServer server_;
// };
// void callback()
// {
//     printf("callback(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
// }
// int main(int argc, char* argv[])
// {

//     // LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
//     // LOG_INFO << "sizeof TcpConnection = " << sizeof(TcpConnection);
//     // g_logFile.reset(new ::LogFile(::basename("tt"), 200 * 100000));
//     // ::Logger::setOutput(outputFunc);
//     // ::Logger::setFlush(flushFunc);

//     if(argc > 1) {
//         numThreads = atoi(argv[1]);
//     }
//     bool ipv6 = argc > 2;
//     EventLoop loop;

//     // loop.runEvery(1.0, callback);
//     InetAddress listenAddr(8000, false, ipv6);
//     EchoServer server(&loop, listenAddr);

//     server.start();

//     loop.loop();
// }
