#include "hv/TcpServer.h"
#include <iostream>
#include <cstring>

using namespace hv;

struct FilePacketHeader {
    unsigned int magic_number;      // Magic number for packet validation, e.g., 0x12345678
    unsigned int packet_type;       // Packet type, 0 for file data, 1 for control commands
    unsigned long long file_size;   // Total size of the file, for progress calculation and verification
    unsigned long long packet_id;   // Packet sequence number, to ensure the correct order of packets
    unsigned int data_length;       // Length of the valid data in this packet
    unsigned int checkHash;         // Hash for data integrity and correctness verification
    char file_name[256];            // File name
};

// Assuming TcpServer has a method to set unpack settings, which might be a custom implementation or extension
void setUnpackSettings(TcpServer& server) {
    unpack_setting_t unpack_setting;
    memset(&unpack_setting, 0, sizeof(unpack_setting_t));
    unpack_setting.package_max_length = DEFAULT_PACKAGE_MAX_LENGTH;
    unpack_setting.mode = UNPACK_BY_LENGTH_FIELD;
    unpack_setting.body_offset = sizeof(FilePacketHeader); // Adjust based on the actual header size without file_name
    unpack_setting.length_field_offset = offsetof(FilePacketHeader, data_length);
    unpack_setting.length_field_bytes = sizeof(unsigned int);
    unpack_setting.length_field_coding = ENCODE_BY_BIG_ENDIAN;
    server.setUnpack(&unpack_setting);
}

int main() {
    int port = 4485;
    TcpServer srv;

    int listenfd = srv.createsocket(port);
    if (listenfd < 0) {
        std::cerr << "Failed to create server socket." << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << port << ", listenfd=" << listenfd << " ..." << std::endl;

    // Set the unpacking rules before starting the server
    setUnpackSettings(srv);

    // Set connection callback
    srv.onConnection = [](const SocketChannelPtr& channel) {
        if (channel->isConnected()) {
            std::cout << channel->peeraddr() << " connected! connfd=" << channel->fd() << std::endl;
        } else {
            std::cout << channel->peeraddr() << " disconnected! connfd=" << channel->fd() << std::endl;
        }
    };

    // Set message callback
    srv.onMessage = [&](const SocketChannelPtr& channel, Buffer* buf) {
        // Parse packet header and handle messages accordingly
        // printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
        std::cout << (int)buf->size() << std::endl;;
    };

    srv.setThreadNum(4);
    srv.start();

    // Run until Enter is pressed
    std::cin.get();
    return 0;
}
