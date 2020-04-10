//
// Created by alex on 17/12/19.
//

#ifndef VIRGILIUMCLIENT_EXCEPTIONS_H
#define VIRGILIUMCLIENT_EXCEPTIONS_H

#include <exception>
#include <string>

class wrongpositionException : public std::exception{
public:
    std::string what ();
};


#endif //VIRGILIUMCLIENT_EXCEPTIONS_H
