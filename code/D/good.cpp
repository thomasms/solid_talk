#include <iostream>
#include <fstream>
#include <memory>
#include <string>

// my simple logger - VerySimpleLogger
#include "loggerlib1.hpp"

// my other logger - LoggerFromAnotherLib
#include "loggerlib2.hpp"

// remember the I - only provide an interface for what you need
// we dictate the interface we need
// we invert the dependency
// by depending on the interface
// not the specific implementation of a logger
struct ISimpleLogger{
    virtual void append(std::string message) = 0;
    virtual size_t size() const = 0;
    virtual std::string get(int index) const = 0;
    virtual operator bool() const = 0;
};

// Wraps the simple logger
struct SimpleLoggerAdapter : public ISimpleLogger{
    virtual void append(std::string message){
        _logger.addMessage(message);
    }

    virtual size_t size() const{
        return _logger.length();
    }

    virtual std::string get(int index) const{
        return _logger.getMessage(index);
    }

    virtual operator bool() const{
        for(int i=0;i<_logger.length();++i){
            if (_logger.getMessage(i).find("error") != std::string::npos) {
                return true;
            }
        }
        return false;
    }

  private:
    // internal logger
    VerySimpleLogger _logger;
};

// Wraps the other external logger
struct AnotherLoggerAdapter : public ISimpleLogger{
    virtual void append(std::string message){
        _logger += message;
    }

    virtual size_t size() const{
        return _logger.nrofmessages();
    }

    // remember that it is 1 based index in the library
    virtual std::string get(int index) const{
        return _logger[index+1];
    }

    // already implemented in _logger
    virtual operator bool() const{
        return _logger;
    }

  private:
    // internal logger
    LoggerFromAnotherLib _logger;
};

// test logger
struct MockNoErrorLogger : public ISimpleLogger{
    virtual void append(std::string message){
        std::cout << "You tried to append a message to a mock\n";
    }

    virtual size_t size() const{
        return 10;
    }

    virtual std::string get(int index) const{
        return "You got message: " + std::to_string(index);
    }

    virtual operator bool() const{
        return false;
    }
};

// our class depends on the implementation of VerySimpleLogger
struct MyCoolClass{

    // create the class using construction injection
    MyCoolClass(ISimpleLogger& logger) :
    _logger(logger)
    {
    }

    void method1(){
        _logger.append("Called method1!");
        //other stuff
    }

    void method2(){
        _logger.append("Called method2!");
        //other stuff
        _logger.append("error");
    }

    // complex stuff
    //...

  private:
    // this is now an interface
    // set by our code not the external library
    ISimpleLogger& _logger;
};

// serializer
std::ostream& operator<<(std::ostream& os, const ISimpleLogger& logger){
    os << " ** Messages ** \n";
    for(int i=0;i<static_cast<int>(logger.size());++i)
        os << "[" << i << "]: " << logger.get(i) << "\n";
    
    return os;
}

// simple test function for running the class methods
// and checking (and writing) the log
void run(ISimpleLogger& logger, std::string filename){
    // MyCoolClass now has a handle to the logger
    // passed in at construction
    MyCoolClass cc(logger);

    cc.method1();
    cc.method1();
    if(logger)
        std::cout << "1: An error occurred - oh dear!\n";
    cc.method2();
    if(logger)
        std::cout << "2: An error occurred - oh dear!\n";
        
    // write messages to file
    std::ofstream myfile(filename);
    myfile << logger;
}

// main program
int main(int argv, char** argc){

    // both have the same interface 
    // that is owned by MyCoolClass
    SimpleLoggerAdapter logger1;
    AnotherLoggerAdapter logger2;
    MockNoErrorLogger mocklogger;

    run(logger1, "logger1.txt");
    run(logger2, "logger2.txt");
    run(mocklogger, "mocklogger.txt");

    return 0;
}
