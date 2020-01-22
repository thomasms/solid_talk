#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>


// super class to do everything
struct MyBadClass{
    explicit MyBadClass(double theta) : _theta(theta){
        std::cout << "MyBadClass::Initial value of theta is: " << std::setprecision(5) << _theta << "\n";
    }

    // this is a bad function
    void calculateTheta(){
        std::cout << "MyBadClass::Calculating theta...\n";
        _theta *= 2.0;
    }   

    inline double getTheta() const{
        return _theta;
    }

    void toFile(std::string filename) const{
        std::cout << "MyBadClass::Writing to imaginary file: " << filename << " ...\n";
        std::ofstream myfile(filename);
        myfile << "Theta=";
        myfile << std::setprecision(5) << _theta;
        myfile.close();
    }

    void plot() const{
        std::cout << "MyBadClass::Pretending to plot...\n";
        std::cout << std::string(ceil(_theta), '*') << "\n";
    }
   
  private:
    double _theta;  
};

int main(int argv, char** argc){

    MyBadClass c1(2.3);
    c1.calculateTheta();

    std::cout << "Theta is: " << std::setprecision(5) << c1.getTheta() << "\n";

    c1.toFile("bad.txt");

    c1.plot();

    return 0;
}
