#include <fstream>
#include <iomanip>
#include <memory>
#include <vector>

#include "plasma.hpp"

// abstract base class
struct INonConstProcess{
    virtual void go() = 0;
};

class PlasmaInitProcess : public INonConstProcess{
    public:
        PlasmaInitProcess(MyPlasmaObject& obj) : _obj(obj) {}
        virtual void go() { 
            std::cout << "Initialising...\n";
            _obj.init();
        }

    private:
        MyPlasmaObject& _obj;
};

class PlasmaComputeProcess : public INonConstProcess{
    public:
        PlasmaComputeProcess(MyPlasmaObject& obj, double initialValue) : 
        _obj(obj),
        _initValue(initialValue)
        {}

        virtual void go() { 
            std::cout << "Computing...\n";
            _obj.compute(_initValue);
        }

    private:
        MyPlasmaObject& _obj;
        double _initValue;
};

// really this is a const process but want the same interface
// this suggests that our undryling data object MyPlasmaObject
// has a problem with its design - but not important for this example
class PlasmaWriteProcess : public INonConstProcess{
    public:
        PlasmaWriteProcess(const MyPlasmaObject& obj, std::string filename) : 
        _obj(obj),
        _filename(filename)
        {}

        virtual void go() { 
            std::cout << "Writing to file...\n";
            toFile(_filename);
        }

    private:

        void toFile(std::string filename){
            std::ofstream myfile(filename);
            myfile << _obj;
        }

        const MyPlasmaObject& _obj;
        std::string _filename;
};

// again this a const process but want the same interface
class PlasmaOutputProcess : public INonConstProcess{
    public:
        PlasmaOutputProcess(const MyPlasmaObject& obj) : 
        _obj(obj)
        {}

        virtual void go() { 
            std::cout << "Theta is: " << std::setprecision(5) << _obj.getTheta() << "\n";
        }

    private:
        const MyPlasmaObject& _obj;
};


// this doesn't need to be modified when adding a new process
struct ProcessManager{
    ProcessManager& add(const std::shared_ptr<INonConstProcess>& process){
        _processes.push_back(process);
        return *this;
    }

    void go() const{
        for(auto& process: _processes){
            process->go();
        }
    }

    void reset(){
        _processes.resize(0);
    }

  private:
    std::vector<std::shared_ptr<INonConstProcess>> _processes;
};

void process(const std::vector<std::shared_ptr<INonConstProcess>>& processes){
    for(auto& process: processes){
        process->go();
    }
}

// main program
int main(int argv, char** argc){

    MyPlasmaObject c1(2.0, 3.0, 4.0);

    ProcessManager manager;
    manager.add(std::make_shared<PlasmaInitProcess>(c1))
           .add(std::make_shared<PlasmaOutputProcess>(c1))
           .add(std::make_shared<PlasmaInitProcess>(c1))
           .add(std::make_shared<PlasmaComputeProcess>(c1, 3.4))
           .add(std::make_shared<PlasmaOutputProcess>(c1))
           .add(std::make_shared<PlasmaWriteProcess>(c1, "good.txt"))
           .go();

    return 0;
}


