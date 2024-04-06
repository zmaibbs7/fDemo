#include "hv/UdpClient.h"
#include "hv/htime.h"

using namespace hv;

int main(int argc, char* argv[]) {

    int port = 8831;

    UdpClient cli;
    int sockfd = cli.createsocket(port);
    if (sockfd < 0) {
        return -20;
    }
    printf("client sendto port %d, sockfd=%d ...\n", port, sockfd);
    cli.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
    };
    cli.onWriteComplete = [](const SocketChannelPtr& channel, Buffer* buf) {
        printf("> %.*s\n", (int)buf->size(), (char*)buf->data());
    };
    cli.start();

    // sendto(time) every 3s
    cli.loop()->setInterval(3000, [&cli](TimerID timerID) {
        char str[DATETIME_FMT_BUFLEN] = {0};
        datetime_t dt = datetime_now();
        datetime_fmt(&dt, str);
        cli.sendto(str);
    });

    // press Enter to stop
    while (getchar() != '\n');
    return 0;
}
