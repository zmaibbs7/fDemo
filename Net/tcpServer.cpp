#include <iostream>
#include "hv/TcpServer.h"
using namespace hv;

int main() {
    int port = 4485;
    TcpServer srv;
    int listenfd = srv.createsocket(port);
    if (listenfd < 0) {
        return -1;
    }

    unpack_setting_t s_unpack_setting;
    memset(&s_unpack_setting, 0, sizeof(unpack_setting_t));
    s_unpack_setting.package_max_length = DEFAULT_PACKAGE_MAX_LENGTH;
    s_unpack_setting.mode = UNPACK_BY_DELIMITER;
    s_unpack_setting.delimiter[0] = '1';
    s_unpack_setting.delimiter[1] = '2';
    s_unpack_setting.delimiter_bytes = 2;
    srv.setUnpack(&s_unpack_setting);

    printf("server listen on port %d, listenfd=%d ...\n", port, listenfd);
    srv.onConnection = [](const SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("%s connected! connfd=%d\n", peeraddr.c_str(), channel->fd());
        } else {
            printf("%s disconnected! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
    };
    srv.onMessage = [&](const SocketChannelPtr& channel, Buffer* buf) {
        // echo
        channel->write(buf);
        std::cout << "---receive----"  << (int)buf->size() << std::endl;
        std::cout << std::string((char*)buf->data() , (int)buf->size()) << std::endl;


    };
    srv.setThreadNum(4);
    srv.start();

    // press Enter to stop
    while (getchar() != '\n');
    return 0;
}
