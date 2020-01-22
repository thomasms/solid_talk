#ifndef MY_PLASMA_OBJECT_H
#define MY_PLASMA_OBJECT_H

#include <algorithm>
#include <iomanip>
#include <iostream>

struct MyPlasmaObject{
    explicit MyPlasmaObject(double alpha, double beta, double gamma) : 
    _alpha(alpha),
    _beta(beta),
    _gamma(gamma),
    _theta(0.0)
    {}

    void init() {
        _theta = 0.0;
        // other initialisation
    }

    double compute(double parameter){
        // dummy example
        _theta = _beta * _gamma*2.0 - _alpha;
        return std::max(parameter, _theta*_theta);
    }

    inline double getTheta() const {
        return _theta;
    }

  private:
    // some arbritary parameters for plasma
    double _alpha;
    double _beta;
    double _gamma;

    double _theta;
};


// serializer
std::ostream& operator<<(std::ostream& os, const MyPlasmaObject& plasma){
    os << "Theta=";
    os << std::setprecision(5) << plasma.getTheta();
    return os;
}

#endif