#include "hv/TcpClient.h"
#include "hv/htime.h"
#include <iostream>

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
void setUnpackSettings(TcpClient& client) {
    unpack_setting_t unpack_setting;
    memset(&unpack_setting, 0, sizeof(unpack_setting_t));
    unpack_setting.package_max_length = DEFAULT_PACKAGE_MAX_LENGTH;
    unpack_setting.mode = UNPACK_BY_LENGTH_FIELD;
    unpack_setting.body_offset = sizeof(FilePacketHeader); 
    unpack_setting.length_field_offset = offsetof(FilePacketHeader, data_length);
    unpack_setting.length_field_bytes = sizeof(unsigned int);
    unpack_setting.length_field_coding = ENCODE_BY_BIG_ENDIAN;
    std::cout << "length_field_offset:" << offsetof(FilePacketHeader, data_length) << std::endl;
    std::cout << "length_field_bytes:" << sizeof(unsigned int) << std::endl;
    client.setUnpack(&unpack_setting);
}

int main(int argc, char* argv[]) {
    int port = 4485;

    TcpClient cli;
    int connfd = cli.createsocket(port);
    if (connfd < 0) {
        return -20;
    }
    
    printf("client connect to port %d, connfd=%d ...\n", port, connfd);


    // Set the unpacking rules before starting the server
    setUnpackSettings(cli);

    cli.onConnection = [&](const SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("connected %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
            
            setInterval(3000, [channel](TimerID timerID){
                if (channel->isConnected()) {
                    FilePacketHeader header;
                    memset(&header, 0, sizeof(header));
                    header.magic_number = 0x12345678;
                    header.packet_type = 1; 
                    header.file_size = 0X778866; 
                    header.packet_id = 0; 
                    header.checkHash = 0; 
                    header.checkHash = 0xEEFF;
                    header.file_name[0] = 0xAA;
                    header.file_name[1] = 0xBB;
                    header.file_name[2] = 0xCC;


                    char str[] = {'1','1','1'}; 
                    // datetime_t dt = datetime_now();
                    // datetime_fmt(&dt, str); 

                    int len = 3;
                    header.data_length = 3; 

                    std::cout << "data_length: " << len;
                    std::vector<char> buffer(sizeof(FilePacketHeader) + len);
                    memcpy(buffer.data(), &header, sizeof(header)); 
                    memcpy(buffer.data() + sizeof(FilePacketHeader), str, len);
                    assert(buffer.size() == sizeof(FilePacketHeader) + len);
                    
                    std::cout << "Sending: ";
                    for(size_t i = 0; i < buffer.size(); ++i) {
                        printf("%02X ", (unsigned char)buffer[i]);
                    }
                    std::cout << std::endl;

                    channel->write(buffer.data(), buffer.size());
                    killTimer(timerID);
                } else {
                    killTimer(timerID);
                }
            });
        } else {
            printf("disconnected %s! connfd=%d\n", peeraddr.c_str(), channel->fd());
        }
    };

    cli.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
    };
    cli.onWriteComplete = [](const SocketChannelPtr& channel, Buffer* buf) {
        // printf("> %.*s\n", (int)buf->size(), (char*)buf->data());
        printf("onWriteComplete.size = %d",(int)buf->size());
    };

    // reconnect: 1,2,4,8,10,10,10...
    reconn_setting_t reconn;
    reconn_setting_init(&reconn);
    reconn.min_delay = 1000;
    reconn.max_delay = 10000;
    reconn.delay_policy = 2;
    cli.setReconnect(&reconn);
    cli.start();

    // press Enter to stop
    while (getchar() != '\n');
    return 0;
}
