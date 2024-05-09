//
// Created by xhome on 2020/9/7.
// 一个简单的有限状态机测试
//

#include "tinyfsm.hpp"
#include <iostream>

//1. 事件的声明,可以携带参数
struct Toggle : tinyfsm::Event{};

//2.定义状态机基类, 投食型状态机
struct Switch : tinyfsm::MealyMachine<Switch>
{
    //纯虚反射，需要在每个状态机中实现.
    virtual void react(Toggle const &) = 0;

    //转换操作,开路
    static void OpenCircuit(){
       std::cout << "开路(light goes OFF)" << std::endl;
    }

    static  void CloseCircuit(){
        std::cout << "关路(light goes ON)" << std::endl;
    }

};

//3.状态机申明
struct Off;

struct On : Switch{
    void react(Toggle const &) override {
        transit<Off>(OpenCircuit); //有限状态机状态切换到off
    }
};

struct Off :Switch{
    void react(Toggle const &) override {
        transit<On>(CloseCircuit);  //状态切换到on
    }
};

//初始化状态机的状态.
FSM_INITIAL_STATE(Switch, Off)

int main()
{
    //开启状态机
    Switch::start();

    std::cout << "> You are facing a light switch..." << std::endl;

    while(1)
    {
        char c;
        std::cout << std::endl << "t=Toggle, q=Quit ? ";
        std::cin >> c;
        switch(c) {
            case 't':
                std::cout << "> Toggling switch..." << std::endl;
                Switch::dispatch(Toggle());
                break;
            case 'q':
                return 0;
            default:
                std::cout << "> Invalid input" << std::endl;
        };
    }

    return 0;
}

