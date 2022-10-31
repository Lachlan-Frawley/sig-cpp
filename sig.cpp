#include "sig.h"

#include <mutex>
#include <atomic>
#include <unordered_map>

static std::recursive_mutex _mutex;
#define LOCK() std::scoped_lock<decltype(_mutex)> f_lock(_mutex)

static std::unordered_map<sig::signals, sighandler_t> _default_handlers;
static std::unordered_map<sig::signals, std::vector<sig::signal_handler_type>> _signal_handlers;
static std::vector<sig::exit_handler_type> _exit_handlers;
static std::vector<sig::exit_handler_type> _quick_exit_handlers;

static bool sig_IS_SIGNAL_RERAISE(int signal)
{
    // TODO
    return true;
}

static void sig_SIGNAL_HANDLER(int signal)
{
    LOCK();

    auto f_signal = static_cast<sig::signals>(signal);

    auto found_handlers = _signal_handlers.find(f_signal);
    if(found_handlers != _signal_handlers.end())
    {
        auto& handlers = found_handlers->second;
        auto size = handlers.size();
        for(auto i = 0; i < size; ++i)
        {
            auto func = handlers[i];
            if(func)
            {
                func(signal);
            }
        }
    }

    if(sig_IS_SIGNAL_RERAISE(signal))
    {
        auto base_handler = _default_handlers.find(f_signal);
        if(base_handler != _default_handlers.end())
        {
            auto handler = base_handler->second;
            if(handler)
            {
                // Re-insert the original handler and raise the signal
                std::signal(signal, handler);
                std::raise(signal);
            }
            else
            {
                // TODO - Error
            }
        }
        else
        {
            // TODO - Error
        }
    }
}

static void sig_EXIT_HANDLER()
{
    LOCK();

    auto size = _exit_handlers.size();
    for(auto i = 0; i < size; ++i)
    {
        auto& func = _exit_handlers[i];
        if(func)
        {
            func();
        }
    }

    // TODO - Anything more?
}

static void sig_QUICK_EXIT_HANDLER()
{
    LOCK();

    auto size = _quick_exit_handlers.size();
    for(auto i = 0; i < size; ++i)
    {
        auto& func = _quick_exit_handlers[i];
        if(func)
        {
            func();
        }
    }

    // TODO - Anything more?
}

void sig::initialize(sig::Settings settings) noexcept
{
    static std::atomic_bool isInitialized(false);

    if(!isInitialized)
    {
        isInitialized = true;

        // TODO - Error
        auto atexit_result = std::atexit(sig_EXIT_HANDLER);
        auto atquickexit_result = std::at_quick_exit(sig_QUICK_EXIT_HANDLER);

        for(sig::signals sg : ALL_SIGNALS)
        {
            auto default_handler = std::signal(static_cast<int>(sg), sig_SIGNAL_HANDLER);

            // TODO - Error
            auto result = _default_handlers.emplace(sg, default_handler);
        }
    }
}

void sig::push_signal_handler(sig::signal_handler_type func, sig::signals signal)
{
    LOCK();

    auto& handlers = _signal_handlers[signal];
    handlers.emplace_back(func);
}

void sig::push_signal_handler(sig::signal_handler_type func, std::set<sig::signals> signals)
{
    LOCK();

    for(sig::signals sg : signals)
    {
        auto& handlers = _signal_handlers[sg];
        handlers.emplace_back(func);
    }
}

void sig::push_exit_handler(sig::exit_handler_type func)
{
    LOCK();

    _exit_handlers.emplace_back(std::move(func));
}

void sig::push_quick_exit_handler(sig::exit_handler_type func)
{
    LOCK();

    _quick_exit_handlers.emplace_back(std::move(func));
}
