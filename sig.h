#include <set>
#include <array>
#include <csignal>
#include <functional>

namespace sig
{
    typedef std::function<void(int)> signal_handler_type;
    typedef std::function<void(void)> exit_handler_type;

    enum class signals : int
    {
        //USER1 = SIGUSR1,
        //USER2 = SIGUSR2,
        INTERRUPT = SIGINT,
        TERMINATE = SIGTERM,
        //ABORT = SIGABRT,
        //QUIT = SIGQUIT,
        //STOP = SIGSTOP,
        //SEGFAULT = SIGSEGV,
        //KILL = SIGKILL,
        //PIPE = SIGPIPE,
    };

    constexpr std::array ALL_SIGNALS
    {
        signals::INTERRUPT,
        signals::TERMINATE
    };

    struct Settings
    {
        // TODO - surely will have stuff in here
    };

    void initialize(Settings settings = {}) noexcept;

    void push_signal_handler(signal_handler_type func, signals signal);
    void push_signal_handler(signal_handler_type func, std::set<signals> signals);

    void push_exit_handler(exit_handler_type func);
    void push_quick_exit_handler(exit_handler_type func);
}
