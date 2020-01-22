
#include <atomic>
#include <iostream>
#include <functional>
#include <memory>
#include <random>
#include <vector>


template<typename ValueType>
struct IGuesser{
    virtual void validate() const = 0;
    virtual void reset() = 0;

    virtual ValueType operator()() const = 0;
    virtual void operator()(const ValueType& value) = 0;
    virtual operator bool() const = 0;
};

template<typename ValueType>
struct CheatGuesser : public IGuesser<ValueType>{

    CheatGuesser(const ValueType& secretValue) : 
    _secretValue(secretValue), _value(ValueType{0}), _guesses(0)
    {}

    virtual void validate() const{
        if(_value< _secretValue){
            std::cout << " !! Fail !! Value is less than secret!\n";
        }
        else if(_value > _secretValue){
            std::cout << " !! Fail !! Value is greater than secret!\n";
        }
        else{
            std::cout << " ** Winner ** You got it!\n";
        }
        std::cout << _guesslimit - _guesses << " guesses remaining!\n";
    }

    // we can cheat by resetting the counter
    virtual void reset(){
        _guesses = 0;
    }

    // getter - we don't really need this method but we must implement it
    virtual ValueType operator()() const{
        return _value;
    }

    // setter
    virtual void operator()(const ValueType& value){
        _value = value;
        ++_guesses;
    }

    // check if guesses are less than 4 - some arb limit
    // stop when we get it correct or reach our limit
    virtual operator bool() const{
        return (_value == _secretValue) || (_guesses >= _guesslimit);
    }

  private:
    const int _guesslimit = 4;
    const ValueType _secretValue;
    ValueType _value;
    std::atomic_int _guesses;
};

// no cheating - reset does nothing!
template<typename ValueType>
struct NoCheatingGuesser : public IGuesser<ValueType>{

    NoCheatingGuesser(const ValueType& secretValue) : 
    _secretValue(secretValue), _value(ValueType{0}), _guesses(0)
    {}

    virtual void validate() const{
        if(_value < _secretValue){
            std::cout << " !! Fail !! Value is less than secret!\n";
        }
        else if(_value > _secretValue){
            std::cout << " !! Fail !! Value is greater than secret!\n";
        }
        else{
            std::cout << " ** Winner ** You got it!\n";
        }
        std::cout << _guesslimit - _guesses << " guesses remaining!\n";
    }

    // no cheating
    virtual void reset(){
        std::cout << " No cheating!\n";
    }

    // getter - we don't really need this method but we must implement it
    virtual ValueType operator()() const{
        return _value;
    }

    // setter
    virtual void operator()(const ValueType& value){
        _value = value;
        ++_guesses;
    }

    // check if guesses are less than 4 - some arb limit
    // stop when we get it correct or reach our limit
    virtual operator bool() const{
        return (_value == _secretValue) || (_guesses >= _guesslimit);
    }

  private:
    const int _guesslimit = 4;
    const ValueType _secretValue;
    ValueType _value;
    std::atomic_int _guesses;
};

// main program
int main(int argv, char** argc){

    using CheatGuesser = CheatGuesser<int>;
    using NoCheatingGuesser = NoCheatingGuesser<int>;

    // set secrets! for this example between 1 and 10
    CheatGuesser cheatGuesser(4);
    NoCheatingGuesser nocheatGuesser(3);

    // vector of references since c++11
    // instead of list of pointers this time.
    std::vector<std::reference_wrapper<IGuesser<int>>> guessers = {
        cheatGuesser,
        nocheatGuesser
    };

    // simple lambda for checking a value and attempting to reset
    auto guessAndCheck = [](IGuesser<int>& guesser, int value){
        // we can cheat by resetting the counter - or can we?
        guesser.reset();
        guesser(value);
        guesser.validate();
    };

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 10);

    // go until our guesses run out (remember it's 4!) 
    for(auto& guesser: guessers){
        while (!guesser.get()){
            guessAndCheck(guesser.get(), dis(gen));
        }
    }

    return 0;
}
