
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>

class invalidInput : public std::exception{};
class playerAlreadyExist : public std::exception{};
class playerDoesntExist : public std::exception{};
class levelDoesntExist : public std::exception{};
class levelAlreadyExist : public std::exception{};

#endif /*EXCEPTIONS_H*/

