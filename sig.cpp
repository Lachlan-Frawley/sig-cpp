#include "sig.h"

#include <mutex>
#include <atomic>
#include <unordered_map>

static std::recursive_mutex _mutex;
#define LOCK() std::scoped_lock<decltype(_mutex)> f_lock(_mutex)

static std::unordered_map<sig::signals, sighandler_t> default_handlers;
static std::unordered_map<sig::signals, std::vector<sig::signal_handler_type>> signal_handlers;
static std::unordered_map<sig::signals, sig::signal_handler_type> temporary_handlers;

static sig::Settings sig_settings;

static bool sig_IS_SIGNAL_FATAL(int signal)
{
    auto sg = static_cast<sig::signals>(signal);

    // If we can find the signal in the 'fatal_signals' set, then it's fatal!
    return sig_settings.fatal_signals.find(sg) != sig_settings.fatal_signals.end();
}

static void sig_SIGNAL_HANDLER(int signal)
{
    LOCK();

    auto f_signal = static_cast<sig::signals>(signal);

    auto found_tmp = temporary_handlers.find(f_signal);
    if(found_tmp != temporary_handlers.end())
    {
        auto func = found_tmp->second;
        if(func)
        {
            func(signal);
        }

        return;
    }

    auto found_handlers = signal_handlers.find(f_signal);
    if(found_handlers != signal_handlers.end())
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

    if(sig_IS_SIGNAL_FATAL(signal))
    {
        auto base_handler = default_handlers.find(f_signal);
        if(base_handler != default_handlers.end())
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
                // TODO - Log Error?
            }
        }
        else
        {
            // TODO - Log Error?
        }

        // TODO - Better handling?
        exit(128 + signal);
    }
}

void sig::initialize(sig::Settings settings) noexcept
{
    static std::atomic_bool isInitialized(false);

    if(!isInitialized)
    {
        isInitialized = true;
        sig_settings = std::move(settings);

        for(sig::signals sg : ALL_SIGNALS)
        {
            auto default_handler = std::signal(static_cast<int>(sg), sig_SIGNAL_HANDLER);
            default_handlers.emplace(sg, default_handler);
        }
    }
}

void sig::push_signal_handler(const sig::signal_handler_type& func, sig::signals signal)
{
    LOCK();

    auto& handlers = signal_handlers[signal];
    handlers.emplace_back(func);
}

void sig::push_signal_handler(const sig::signal_handler_type& func, const std::set<sig::signals>& signals)
{
    LOCK();

    for(sig::signals sg : signals)
    {
        auto& handlers = signal_handlers[sg];
        handlers.emplace_back(func);
    }
}

void sig::push_temporary_handler(const sig::signal_handler_type &func, sig::signals signal)
{
    if(temporary_handlers.find(signal) == temporary_handlers.end())
    {
        temporary_handlers[signal] = func;
    }
}

sig::signal_handler_type sig::pop_temporary_handler(sig::signals signal)
{
    if(temporary_handlers.find(signal) == temporary_handlers.end())
    {
        return nullptr;
    }
    else
    {
        auto tmp = temporary_handlers[signal];
        temporary_handlers.erase(signal);
        return tmp;
    }
}

bool sig::is_temporary_handler_populated(sig::signals signal)
{
    return temporary_handlers.find(signal) != temporary_handlers.end();
}
