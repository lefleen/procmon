#include "UserInterface.h"

void UserInterface::Errors::show(const str_t& what, const Result)
{
    std::cerr << "[procmon] error: " << what << std::endl;
}
