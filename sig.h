#include <set>
#include <array>
#include <csignal>
#include <functional>

namespace sig
{
    typedef std::function<void(int)> signal_handler_type;

    enum class signals : int
    {
        USER1 = SIGUSR1,
        USER2 = SIGUSR2,
        INTERRUPT = SIGINT,
        TERMINATE = SIGTERM,
        ABORT = SIGABRT,
        QUIT = SIGQUIT,
        SEGFAULT = SIGSEGV,
        PIPE = SIGPIPE,
    };

    constexpr std::array ALL_SIGNALS
    {
        signals::USER1,
        signals::USER2,
        signals::INTERRUPT,
        signals::TERMINATE,
        signals::ABORT,
        signals::QUIT,
        signals::SEGFAULT,
        signals::PIPE
    };

    struct Settings
    {
        // TODO - surely will have stuff in here
    };

    void initialize(Settings settings = {}) noexcept;

    void push_signal_handler(const signal_handler_type& func, signals signal);
    void push_signal_handler(const signal_handler_type& func, const std::set<signals>& signals);
}
