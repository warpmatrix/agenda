#include "Exception.hpp"

AgendaException::AgendaException(std::string msg)
    :msg("Error: " + msg) {}
const char *AgendaException::what() const throw () {
    return msg.c_str();
}