#include <iostream>
#include <fstream>
#include "header.pb.h"

int main() {
    // 初始化Protobuf库
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // 创建并填充Person消息
    packet::MessagePacketHeader msgHeader;
    msgHeader.set_magic_number(887654321);
    msgHeader.set_packet_type(packet::MessageType::CONTROL);
    msgHeader.set_data_length(100);
    msgHeader.set_packet_id(0);
    msgHeader.set_packet_checksum(4);

    // 序列化消息到文件
    std::ofstream output("test.bin", std::ios::binary);
    msgHeader.SerializeToOstream(&output);
    output.close();

    // 从文件反序列化消息
    packet::MessagePacketHeader msgHeader_read;
    std::ifstream input("test.bin", std::ios::binary);
    msgHeader_read.ParseFromIstream(&input);
    input.close();

    // 打印消息
    std::cout << "magic_number: " << msgHeader_read.magic_number() << std::endl;
    std::cout << "packet_type: " << msgHeader_read.packet_type() << std::endl;
    std::cout << "data_length: " << msgHeader_read.data_length() << std::endl;
    std::cout << "packet_id: " << msgHeader_read.packet_id() << std::endl;
    std::cout << "packet_checksum: " << msgHeader_read.packet_checksum() << std::endl;

    // 清理Protobuf库
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
