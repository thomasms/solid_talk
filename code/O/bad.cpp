#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "plasma.hpp"

enum PlasmaProcess {
    DO_PLASMA_INIT,
    DO_PLASMA_COMPUTE,
    DO_PLASMA_WRITE_TO_FILE,
    DO_PLASMA_GET
};

void toFile(const MyPlasmaObject& plasma, std::string filename){
    std::ofstream myfile(filename);
    myfile << plasma;
}

void process(const std::vector<PlasmaProcess>& options, 
             MyPlasmaObject& obj){
    for(auto option: options){
        switch(option){
            case DO_PLASMA_INIT:
                std::cout << "Initialising...\n";
                obj.init();
                break;
                // ...
            case DO_PLASMA_COMPUTE:
                std::cout << "Computing...\n";
                obj.compute(3.4);
                break;
                // ...
            case DO_PLASMA_WRITE_TO_FILE:
                std::cout << "Writing to file...\n";
                toFile(obj, "bad.txt");
                break;
                //.. 
            case DO_PLASMA_GET:
                std::cout << "Theta is: " << std::setprecision(5) << obj.getTheta() << "\n";
                break;
                // ..
            default:
                std::cout << "Invalid option!\n";
        }
    }
}

int main(int argv, char** argc){

    MyPlasmaObject c1(2.0, 3.0, 4.0);
    const std::vector<PlasmaProcess> options = {
        DO_PLASMA_INIT,
        DO_PLASMA_GET,
        DO_PLASMA_INIT,
        DO_PLASMA_COMPUTE,
        DO_PLASMA_GET,
        DO_PLASMA_WRITE_TO_FILE
    };
    
    process(options, c1);

    return 0;
}


