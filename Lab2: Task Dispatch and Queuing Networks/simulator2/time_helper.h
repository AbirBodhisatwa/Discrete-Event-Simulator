#ifndef TIME_HELPER_H
#define TIME_HELPER_H

#include <cstdint>

#define NS_PER_SEC 1000000000ull

// Converts seconds to nanoseconds
inline uint64_t convertSecondsToTime(double sec)
{
    return static_cast<uint64_t>(sec * static_cast<double>(NS_PER_SEC));
}

// Convert nanoseconds to seconds
inline double convertTimeToSeconds(uint64_t t)
{
    return static_cast<double>(t) / static_cast<double>(NS_PER_SEC);
}

#endif /* TIME_HELPER_H */
