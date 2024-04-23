#include <iostream>
#include "hv/requests.h"
#include "nlohmann/json.hpp"

int main() {
    using json = nlohmann::json;

    // 构造 JSON header
    json body = {
        {"menu_id", 5},
        {"callee_num", "201"},
        {"callee_type", "ext"}
    };

    // 将 JSON 对象转换为字符串，以便作为请求的一部分发送
    std::string body_str = body.dump();
    http_headers headers;
    headers["Content-Type"] = "application/json";

    // 发送 POST 请求
    auto resp = requests::post("http://47.100.126.90:443/test/test/connect", body_str, headers);
    if (resp == nullptr) {
        std::cout << "Request failed!\n";
    } else {
        std::cout << resp->status_code << " " << resp->status_message() << "\r\n";
        std::cout << resp->body << "\n";
    }

    return 0;
}
