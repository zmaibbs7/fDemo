#include "hv/UdpClient.h"
#include "hv/htime.h"

using namespace hv;

struct FilePacketHeader {
    uint32_t magic_number;          // Magic number for packet validation, e.g., 0x12345678
    uint8_t  packet_type;           // Packet type, 0 for file data, 1 for control commands
    uint32_t packet_detail_length;  // Packet type detail information length
    uint64_t packet_id;             // Packet sequence number, to ensure the correct order of packets
    uint64_t packet_sum;            // Packet sequence number, to ensure the correct order of packets
    uint32_t data_length;           // Length of the valid data in this packet
};

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
