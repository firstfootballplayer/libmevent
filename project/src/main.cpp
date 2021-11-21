#include "libmevent/net/TcpServer.h"

#include "libmevent/base/LogFile.h"
#include "libmevent/base/Logging.h"
#include "libmevent/base/Thread.h"
#include "libmevent/net/EventLoop.h"
#include "libmevent/net/InetAddress.h"

#include <utility>

#include <stdio.h>
#include <unistd.h>

using namespace libmevent;
using namespace libmevent::net;

int numThreads = 0;
std::unique_ptr<libmevent::LogFile> g_logFile;
void outputFunc(const char* msg, int len)
{
    g_logFile->append(msg, len);
}

void flushFunc()
{
    g_logFile->flush();
}

class EchoServer
{
public:
    EchoServer(EventLoop* loop, const InetAddress& listenAddr) : loop_(loop), server_(loop, listenAddr, "EchoServer")
    {
        server_.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));
        server_.setMessageCallback(std::bind(&EchoServer::onMessage, this, _1, _2, _3));
        server_.setThreadNum(numThreads);
    }

    void start()
    {
        server_.start();
    }
    // void stop();

private:
    void onConnection(const TcpConnectionPtr& conn)
    {
        // LOG_TRACE << conn->peerAddress().toIpPort() << " -> " <<
        // conn->localAddress().toIpPort() << " is " << (conn->connected() ? "UP"

        // "DOWN");
        // LOG_INFO << conn->getTcpInfoString();

        // conn->send("hello\n");
    }

    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
    {
        static int c = 0;
        //
        // printf("jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj\n");
        string msg(buf->retrieveAllAsString());
        // LOG_INFO << " recv fd:  " << conn->fd();
        // LOG_INFO << c++;
        if(msg == "exit\n") {
            conn->send("bye\n");
            conn->shutdown();
        }
        if(msg == "quit\n") {
            loop_->quit();
        }
        conn->send(msg);
        conn->forceClose();
    }

    EventLoop* loop_;
    TcpServer server_;
};

#include <signal.h>

void callback()
{
    printf("callback(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
}

void my_handler(int s)
{
    printf("Caught signal %d\n", s);
    alarm(2);
}

int main(int argc, char* argv[])
{

    // LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    // LOG_INFO << "sizeof TcpConnection = " << sizeof(TcpConnection);

    // g_logFile.reset(new ::LogFile(::basename("tt"), 200 * 100000));
    // ::Logger::setOutput(outputFunc);
    // ::Logger::setFlush(flushFunc);
    struct sigaction sa;

    //     /* setup alarm signal handler */
    sa.sa_handler = my_handler;
    sa.sa_flags = 0;
    if(sigaction(SIGALRM, &sa, NULL)) exit(3);
    alarm(2);
    if(argc > 1) {
        numThreads = atoi(argv[1]);
    }
    bool ipv6 = argc > 2;
    EventLoop loop;
    while(true) {
        ;
    }
    sleep(100);
    // loop.runEvery(1.0, callback);
    InetAddress listenAddr(8000, false, ipv6);
    EchoServer server(&loop, listenAddr);

    server.start();

    loop.loop();
}

// #include <stdio.h>
// #include <unistd.h>
// void a()

// {
//     for(int i = 0; i < 10000; i++)
//         for(int i = 0; i < 10000; i++)
//             ;
//     printf("\t\t+---call a() function\n");
//     sleep(1);
// }

// void c()

// {
//     for(int i = 0; i < 10000; i++)
//         for(int i = 0; i < 10000; i++)
//             ;
//     printf("\t\t+---call c() function\n");
//     a();
// }

// int b()
// {
//     for(int i = 0; i < 10000; i++)
//         for(int i = 0; i < 10000; i++)
//             ;
//     printf("\t+--- call b()     function\n");

//     a();

//     c();

//     return 0;
// }

// int main()

// {

//     printf(" main() function()\n");

//     b();

//     return 0;
// }