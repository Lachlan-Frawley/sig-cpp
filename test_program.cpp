#include <iostream>

#include "sig.h"

#include <CLI/CLI.hpp>


void my_cleanup_signal_handler(int signal)
{
    std::cerr << "Cleaning up! (I swear!)" << std::endl;
}

int main(int argc, char** argv)
{
    std::cout << "Hello World!" << std::endl;

    sig::initialize();

    sig::push_signal_handler(my_cleanup_signal_handler, { sig::ALL_SIGNALS.begin(), sig::ALL_SIGNALS.end() });

    //std::raise(SIGPIPE);

    int* var = nullptr;
    (*var) = 1;

    std::cout << "Goodbye World!" << std::endl;

    return 0;
}
