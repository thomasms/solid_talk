#include <iostream>
#include <string>
#include <vector>

// my simple logger - VerySimpleLogger
#include "loggerlib1.hpp"

// our class depends on the implementation of VerySimpleLogger
struct MyCoolClass{

    void method1(){
        _logger.addMessage("Called method1!");
        //other stuff
    }

    void method2(){
        _logger.addMessage("Called method2!");
        //other stuff
        _logger.addMessage("error");
    }

    // we implement this because our logger doesn't do this
    // LoggerFromAnotherLib does but note the comment on bool() operator.
    bool hasError() const{
        for(int i=0;i<_logger.length();++i){
            if (_logger.getMessage(i).find("error") != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    // complex stuff
    //...

  private:
    // to change this logger to another i.e. LoggerFromAnotherLib
    // from loggerlib2.hpp
    // would cause us to change the implementation in this class
    VerySimpleLogger _logger;
};

// main program
int main(int argv, char** argc){

    MyCoolClass cc;

    cc.method1();
    if(cc.hasError())
        std::cout << "1: An error occurred - oh dear!\n";
    cc.method2();
    if(cc.hasError())
        std::cout << "2: An error occurred - oh dear!\n";

    return 0;
}
