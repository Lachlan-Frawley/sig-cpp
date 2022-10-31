#include <iostream>

#include "sig.h"

#include <CLI/CLI.hpp>

void my_exit_handler()
{
    std::cout << "atexit() handler" << std::endl;
}

void my_quick_exit_handler()
{
    std::cout << "at_quick_exit() handler" << std::endl;
}

void my_bound_exit_handler(bool is_quick)
{
    if(is_quick)
    {
        std::cout << "See you later! (Quick)" << std::endl;
    }
    else
    {
        std::cout << "See you later! (Normal)" << std::endl;
    }
}

/*void my_cleanup_signal_handler(int signal)
{
    std::cerr << "Cleaning up! (I swear!)" << std::endl;
}*/

int main(int argc, char** argv)
{
    std::cout << "Hello World!" << std::endl;

    sig::initialize();

    //sig::push_signal_handler(my_cleanup_signal_handler, { sig::signals::INTERRUPT, sig::signals::TERMINATE });

    sig::push_exit_handler(my_exit_handler);
    sig::push_quick_exit_handler(my_quick_exit_handler);

    sig::push_exit_handler(std::bind(my_bound_exit_handler, false));
    sig::push_quick_exit_handler(std::bind(my_bound_exit_handler, true));

    return 0;
}
