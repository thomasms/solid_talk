
#include <atomic>
#include <iostream>
#include <functional>
#include <memory>
#include <random>
#include <vector>

// maybe a bit overkill but proves a point
struct IConstValidatible{
    virtual void validate() const = 0;
};

struct IResetible{
    virtual void reset() = 0;
};

struct ISimpleChecker{
    virtual operator bool() const = 0;
};

template<typename ValueType>
struct ISetAccess{
    virtual void operator()(const ValueType& value) = 0;
};

// only use multiple inheritance for interfaces!
template<typename ValueType>
struct CheatGuesser : public IConstValidatible,
                      public IResetible,
                      public ISimpleChecker,
                      public ISetAccess<ValueType>{

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

// no cheating - no reset
template<typename ValueType>
struct NoCheatingGuesser : public IConstValidatible,
                           public ISimpleChecker,
                           public ISetAccess<ValueType>{

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

// no cheating - no reset
template<typename ValueType>
struct LuckyGuesser : public IConstValidatible,
                      public ISimpleChecker{

    LuckyGuesser(const ValueType& secretValue) : 
    _secretValue(secretValue), _value(ValueType{0})
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
    }

    // check if guesses are less than 4 - some arb limit
    // stop when we get it correct or reach our limit
    virtual operator bool() const{
        return _value == _secretValue;
    }

  private:
    const ValueType _secretValue;
    ValueType _value;
};

// main program
int main(int argv, char** argc){

    using CheatGuesser = CheatGuesser<int>;
    using NoCheatingGuesser = NoCheatingGuesser<int>;
    using LuckyGuesser = LuckyGuesser<int>;

    // set secrets! for this example between 1 and 10
    CheatGuesser cheatGuesser(4); // can reset and guess
    NoCheatingGuesser nocheatGuesser(3); // can guess 4 times, no reset
    LuckyGuesser luckyGuesser(5); // one guess only!

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 10);

    // always win
    while (!cheatGuesser){
        cheatGuesser.reset();
        cheatGuesser(dis(gen));
        cheatGuesser.validate();
    }

    // sometimes win
    while (!nocheatGuesser){
        nocheatGuesser(dis(gen));
        nocheatGuesser.validate();
    }

    // rarely win
    luckyGuesser.validate();

    return 0;
}
