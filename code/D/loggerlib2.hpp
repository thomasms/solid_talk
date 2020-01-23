#include <string>
#include <vector>

#ifndef LOGGER_TWO_HPP
#define LOGGER_TWO_HPP

// different methods and underlying datastructure
// represents a logger from an external library 
// luckily for this example it does not depend on anything
struct LoggerFromAnotherLib{
    // add message
    inline void log(std::string message){
        _messages.push_back(message);
    }

    // operator overload for +=
    inline void operator+=(std::string message){
        log(message);
    }

    // get the number of messages
    inline size_t nrofmessages() const{
        return _messages.size();
    }

    // get message - this is the same
    // for some weird reason it expects
    // 1 based indices, not normal 0 based indices
    std::string operator[](int index) const{
        return _messages[index-1];
    }

    // checks for ERROR in string to check for error
    // only by reading the documentation of this library
    // would you ever know this! 
    inline operator bool() const{
        for(int i=0;i<static_cast<int>(_messages.size());++i){
            if (_messages[i].find("ERROR") != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    // other methods and more complex stuff
    // iterators, serialization, etc

  private:
    std::vector<std::string> _messages;
};

#endif
