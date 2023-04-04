#ifndef PHASE_ONE_EXCEPTION_H
#define PHASE_ONE_EXCEPTION_H
#include <exception>

class PhaseOneException : public std::exception {
  private: 
  const char* _message;

  public:
  PhaseOneException(): _message(nullptr){};
  PhaseOneException(const char* message): _message(message){};

  const char* what() const throw() {
    return _message;
  }
};

#endif