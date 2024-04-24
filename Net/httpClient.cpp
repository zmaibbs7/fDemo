#include <iostream>
#include "hv/requests.h"
#include "nlohmann/json.hpp"

//打电话
void demo1()
{
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
}


//获取token
std::string get_cloud_token()
{
    using json = nlohmann::json;

    // 构造 JSON header
    json body = {
        {"secretKey", "893d1856723bfb71"},
        {"sn", "G2403202404230001"}
    };
    std::string body_str = body.dump();

    http_headers headers;
    headers["Content-Type"] = "application/json";
    auto resp = requests::post("http://47.100.126.90:443/delibot/api/auth/token", body_str, headers);
    if (resp == nullptr) {
        std::cout << "Request failed!\n";
    } else 
    {
        std::cout << resp->status_code << " " << resp->status_message() << "\r\n";
        std::cout << resp->body << "\n";
    }

    json response = json::parse(resp->body.c_str());
    std::string token = response["data"]["accessToken"];
    return token;
}

//获取最新时间
void get_cloud_time(const std::string& toeken)
{
    using json = nlohmann::json;


    http_headers headers;
    headers["Content-Type"] = "application/x-www-form-urlencoded";
    headers["accessToken"] = toeken;

    // 发送 POST 请求
    auto resp = requests::get("http://47.100.126.90:443/delibot/api/device/info", headers);
    if (resp == nullptr) {
        std::cout << "Request failed!\n";
    } else 
    {
        std::cout << resp->status_code << " " << resp->status_message() << "\r\n";
        std::cout << resp->body << "\n";
    }
}

int main() {


    std::string token = get_cloud_token();
    std::cout << "token = " << token << std::endl;
    get_cloud_time(token);
    return 0;
}
