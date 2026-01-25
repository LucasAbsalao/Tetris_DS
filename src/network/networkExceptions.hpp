#include <exception>
#include <string>

class EnetException : public std::exception {
private:
    std::string message;
public:
    EnetException(std::string msg) : message(msg) {}

    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class ServerConnectionException : public EnetException { // Exception to connect to the server
public:
    ServerConnectionException(const std::string& msg = "Failed to connect to server") 
        : EnetException(msg) {}
};