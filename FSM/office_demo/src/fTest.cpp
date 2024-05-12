#ifndef ELEVATORNAVBOTFSM_H
#define ELEVATORNAVBOTFSM_H

#include <iostream>
#include <tinyfsm.hpp>


//定义事件
struct TaskCompleted : tinyfsm::Event {}; // 任务完成
struct ElevatorDoorOpened : tinyfsm::Event {}; //电梯门已经开了 事件
struct ElevatorExited : tinyfsm::Event {}; // 出梯动作完成 事件

//定义状态机
class ElevatorNavBotFSM : public tinyfsm::Fsm<ElevatorNavBotFSM>
{
public:
    // 默认的事件处理
    virtual void react(ElevatorDoorOpened const &) { std::cout << "Ignored ElevatorArrived" << std::endl; }

    virtual void entry(void) { }; 
    virtual void exit(void)  { }; 
};

//定义状态
/**
 * @brief 进电梯-呼梯点 状态
 */
class InElevatorCallPoint_Status : public ElevatorNavBotFSM {

    virtual void entry(void){ std::cout << "XX Ignored ElevatorArrived" << floor <<  std::endl; }
    private:
        int floor = 99;

};

/**
 * @brief 开始任务状态
 */
class TaskStart_Status : public ElevatorNavBotFSM {
    virtual void entry(void)
    {
        std::cout << "==entry==" << std::endl;
        transit<InElevatorCallPoint_Status>();
    }
};

/**
 * @brief 申请电梯状态
 */
class RequestElevator_Status : public ElevatorNavBotFSM {

};

/**
 * @brief 进梯出梯状态
 */
class Elevatoring__Status : public ElevatorNavBotFSM {

};

/**
 * @brief 出电梯-呼梯点 状态
 */
class OutElevatorCallPoint_Status : public ElevatorNavBotFSM {

};

/**
 * @brief 执行任务 状态
 */
class PerformAction_Status : public ElevatorNavBotFSM {

};

FSM_INITIAL_STATE(ElevatorNavBotFSM, TaskStart_Status)

int main()
{
    //开启状态机
    ElevatorNavBotFSM::start();



    return 0;
}
#endif //ELEVATORNAVBOTFSM_H