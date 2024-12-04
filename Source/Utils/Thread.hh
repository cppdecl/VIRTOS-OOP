#ifndef UTILS_THREAD_HH
#define UTILS_THREAD_HH

#include <memory>
#include <mutex>
#include <thread>

template <typename T>
class MakeThreadSafe 
{
    private:
    T m_Data;
    mutable std::mutex m_Mutex;

    public:
    MakeThreadSafe() = default;
    explicit MakeThreadSafe(const T& initialValue) : m_Data(initialValue) {}

    T Get() const 
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_Data;
    }

    void Set(const T& value) 
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Data = value;
    }

    template <typename Func>
    void Modify(Func func) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        func(m_Data);
    }
};

namespace Thread 
{
    inline void Sleep(int ms) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }


}

#endif // UTILS_THREAD_HH