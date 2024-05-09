//
// Created by xhome on 2020/9/7.
// 本例展示故障开关,多重次
//

// DumpState  转储状态.

#include "tinyfsm.hpp"
#include <iostream>
#include <stdlib.h> /* rand */

//模板类申明.
template <int inum>
class Off;

//状态转储
static void DumpState(int inum, const char * state, int on_counter, int defect_level){
    std::cout << "* Switch[" << inum << "] is " << state << " (on_counter=" << on_counter << ", defect_level=" << defect_level << ")" << std::endl;
}

//1.事件定义
struct Toggle:tinyfsm::Event{};

//2.状态机基类定义
template <int inum>
class DefectiveSwitch : public tinyfsm::Fsm<DefectiveSwitch<inum>>
{
public:
    static constexpr unsigned int defect_level = (inum*2);

    static void reset(void ){
        on_counter = 0;
    }

    void react(tinyfsm::Event const &){};

    virtual void react(Toggle const &){};
    virtual void entry(void){};
    void exit(){};

protected:
    static unsigned int on_counter;
};

template <int inum>
unsigned int DefectiveSwitch<inum>::on_counter{0};

//状态变量的定义
template <int inum>
class On : public DefectiveSwitch<inum>
{
    using base = DefectiveSwitch<inum>;

    void entry()override {
        base::on_counter++;
        DumpState(inum, "ON ", base::on_counter, base::defect_level);
    }

    void react(Toggle const &) override {
        base::template transit<Off<inum>>();
    }
};

template <int inum>
class  Off : public DefectiveSwitch<inum>
{
    using base = DefectiveSwitch<inum>;
    void entry() override {
        DumpState(inum, "OFF", base::on_counter , base::defect_level);
    }

    void react(Toggle const &) override {
        if((rand()%(base::defect_level +1)) == 0){
            base::template transit<On<inum>>();
        }else{
            std::cout << "* Kzzz kzzzzzz" << std::endl;
            base::template transit< Off<inum> >();
        }
    }

};

FSM_INITIAL_STATE(DefectiveSwitch<0>, Off<0>)
FSM_INITIAL_STATE(DefectiveSwitch<1>, Off<1>)
FSM_INITIAL_STATE(DefectiveSwitch<2>, Off<2>)

//4.状态机列表申明
using fsm_handle = tinyfsm::FsmList<DefectiveSwitch<0>, DefectiveSwitch<1>, DefectiveSwitch<2>>;

template <int inum>
void ToggleSingle(){
    std::cout << "> Toggling switch " << inum << "..." << std::endl;
    DefectiveSwitch<inum>::dispatch(Toggle());
}

int main()
{
    fsm_handle::start();

    while(1)
    {

        char c;
        std::cout << std::endl << "0,1,2=Toggle single, a=Toggle all, r=Restart, q=Quit ? ";
        std::cin >> c;

        switch(c) {
            case '0': ToggleSingle<0>(); break;
            case '1': ToggleSingle<1>(); break;
            case '2': ToggleSingle<2>(); break;
            case 'a':
                std::cout << "> Toggling all switches..." << std::endl;
                fsm_handle::dispatch(Toggle());
                break;
            case 'r':
                fsm_handle::reset();
                fsm_handle::start();
                break;
            case 'q':
                return 0;
            default:
                std::cout << "> Invalid input" << std::endl;
        };
    }

    return 0;
}

