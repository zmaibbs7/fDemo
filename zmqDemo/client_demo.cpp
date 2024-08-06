#include <zmq.hpp>  
#include <string>  
#include <iostream>  
  
int main() {  
    // 创建一个 ZeroMQ 上下文  
    zmq::context_t context(1);  
  
    // 创建一个 ZeroMQ 套接字并连接到目标地址  
    zmq::socket_t socket(context, ZMQ_REQ);  
    socket.connect("tcp://localhost:8888");  
  
    // 发送消息  
    std::string message = "Hello, World! from client";  
    socket.send(message.c_str(), message.size());  
  
    //接收响应  
    zmq::message_t response;  
    socket.recv(&response, 0);   
    std::cout << "Received response: " << std::string(static_cast<char*>(response.data()), response.size()) << std::endl;  
  
    return 0;  
}

