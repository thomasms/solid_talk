#include <string>
#include <vector>

#ifndef LOGGER_ONE_HPP
#define LOGGER_ONE_HPP

// very simple, naive logger
struct VerySimpleLogger{
    // add message
    inline void addMessage(std::string message){
        _messages.push_back(message);
    }

    // get the number of messages
    inline int length() const{
        return static_cast<int>(_messages.size());
    }

    // get message
    std::string getMessage(int index) const{
        return _messages[index];
    }

  private:
    std::vector<std::string> _messages;
};

#endif
