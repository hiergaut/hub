#include "SizeOf.hpp"

#include <cmath>

namespace hub {

//// source : https://www.mbeckler.org/blog/?p=114
//// Prints to the provided buffer a nice number of bytes (KB, MB, GB, etc)
std::string pretty_bytes( hub::Size_t bytes ) {

    constexpr auto buffSize = 32;
    char buff[buffSize] { 0 };
    static CONSTEXPR17 std::string_view suffixes[] { "Bytes", "Ko", "Mo", "Go", "To", "Po", "Eo" };
    uint64_t s          = 0; // which suffix to use
    double count        = bytes;
    constexpr auto kilo = 1'000;

    while ( count >= kilo && s < 7 ) {
        s++;
        count /= kilo;
    }

    if ( count - floor( count ) == 0.0 ) {
#ifdef WIN32
        snprintf( buff, buffSize, "%d %s", (int)count, suffixes[s].data() );
#else
#    ifdef OS_MACOS
        snprintf( buff, buffSize, "%d %s", (int)count, suffixes[s].data() );
#    else
        sprintf( buff, "%d %s", (int)count, suffixes[s].data() );
#    endif
#endif
    }
    else {
#ifdef WIN32
        snprintf( buff, buffSize, "%.1f %s", count, suffixes[s].data() );
#else
#    ifdef OS_MACOS
        snprintf( buff, buffSize, "%.1f %s", count, suffixes[s].data() );
#    else
        sprintf( buff, "%.1f %s", count, suffixes[s].data() );
#    endif
#endif
    }

    return buff;
}

} // namespace hub
