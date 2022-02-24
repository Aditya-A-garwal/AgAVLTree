#include <iostream>
#include <chrono>

struct ScopedTimer {

    private:
    std::chrono::high_resolution_clock::time_point m_startPoint;
    std::chrono::high_resolution_clock::time_point m_endPoint;

    const char *                                   msg;

    void
    stop ()
    {

        m_endPoint = std::chrono::high_resolution_clock::now ();
        auto diff  = std::chrono::duration_cast<std::chrono::milliseconds> (m_endPoint - m_startPoint).count ();
        std::cout << "Time elapsed for (" << msg << "): " << format_integer (diff) << " ms" << std::endl;
    }

    public:
    ScopedTimer (const char * pMsg) : msg {pMsg}
    {

        std::cout << "Starting new scoped timer for (" << msg << ")" << std::endl;
        m_startPoint = std::chrono::high_resolution_clock::now ();
    }

    ~ScopedTimer ()
    {
        stop ();
    }
};