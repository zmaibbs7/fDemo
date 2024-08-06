#include <zmq.hpp>  
#include <string>  
#include <iostream>  
  
int main() {  
    // 创建一个 ZeroMQ 上下文  
    zmq::context_t context(1);  
  
    // 创建一个 ZeroMQ 套接字并绑定到端口  
    zmq::socket_t socket(context, ZMQ_REP);  
    socket.bind("tcp://*:8888");  
  
    while (true) {  
        // 接收消息  
        zmq::message_t message(10); // 10 bytes buffer  
        socket.recv(message);  
        std::cout << "Received Client: " << std::string(static_cast<char*>(message.data()), message.size()) << std::endl;  
  
        // 发送回应消息  
        std::string reply = "Hello from C++";  
        socket.send(reply.c_str(), reply.size());  
    }  
  
    return 0;  
}

