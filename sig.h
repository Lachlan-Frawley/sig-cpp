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

    constexpr std::array FATAL_SIGNALS
    {
        signals::INTERRUPT,
        signals::TERMINATE,
        signals::ABORT,
        signals::QUIT,
        signals::SEGFAULT
    };

    struct Settings
    {
        std::set<signals> fatal_signals { FATAL_SIGNALS.begin(), FATAL_SIGNALS.end() };
    };

    void initialize(Settings settings = {}) noexcept;

    void push_signal_handler(const signal_handler_type& func, signals signal);
    void push_signal_handler(const signal_handler_type& func, const std::set<signals>& signals);

    void push_temporary_handler(const signal_handler_type& func, signals signal);
    signal_handler_type pop_temporary_handler(signals signal);
    bool is_temporary_handler_populated(signals signal);
}
