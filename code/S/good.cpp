#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>


// logic
struct MyBetterClass{
    explicit MyBetterClass(double theta) : _theta(theta){
        std::cout << "MyBetterClass::Initial value of theta is: " << std::setprecision(5) << _theta << "\n";
    }

    // no longer mutates the state
    double calculateTheta() const{
        std::cout << "MyBetterClass::Calculating theta...\n";
        return _theta * 2.0;
    }    

  private:
    double _theta;  
};

// serializer
std::ostream& operator<<(std::ostream& os, const MyBetterClass& myclass){
    os << "Theta=";
    os << std::setprecision(5) << myclass.calculateTheta();
    return os;
}

// plotter
struct MyBetterClassPlotter {
    void operator()(const MyBetterClass& myclass) const{
        std::cout << "MyBetterClassPlotter::Pretending to plot...\n";
        std::cout << std::string(ceil(myclass.calculateTheta()), '*') << "\n";
    }
};

int main(int argv, char** argc){

    const MyBetterClass c1(2.3);
    const MyBetterClassPlotter plotter;

    std::cout << "Theta is: " << std::setprecision(5) << c1.calculateTheta() << "\n";

    std::ofstream myfile("good.txt");
    myfile << c1;

    plotter(c1);

    return 0;
}


