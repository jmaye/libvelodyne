#ifndef THREADEXCEPTION_H
#define THREADEXCEPTION_H

#include <stdexcept>
#include <string>

class ThreadException : public std::runtime_error {
  ThreadException& operator = (const ThreadException &other);

public:
  ThreadException(const std::string &msg = "");
  ThreadException(const ThreadException &other);

protected:

};

#endif // THREADEXCEPTION_H
