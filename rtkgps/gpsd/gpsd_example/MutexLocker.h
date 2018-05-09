/********************************************
 *  Gpsd client example
 *  created by s.farmbauer@red-beard-code.de
 ********************************************/

#ifndef GPSD_EXAMPLE_MUTEXLOCKER_H
#define GPSD_EXAMPLE_MUTEXLOCKER_H
class MutexLocker
{
public:
    MutexLocker(std::mutex& lock)
        :m_lock(lock)
    {
        m_lock.lock();
    }

    ~MutexLocker()
    {
        m_lock.unlock();
    }

private:
    std::mutex& m_lock;
 };

#endif //GPSD_EXAMPLE_MUTEXLOCKER_H
