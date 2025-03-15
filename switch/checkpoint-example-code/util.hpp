 #ifndef UTIL_HPP
#define UTIL_HPP

#include "account.hpp"
#include "common.hpp"
#include "io.hpp"
#include <switch.h>
#include <sys/stat.h>
extern "C" {
#include "ftp.h"
}

// debug
#include <arpa/inet.h>
#include <sys/errno.h>
#include <sys/socket.h>

void servicesExit(void);
Result servicesInit(void);
HidsysNotificationLedPattern blinkLedPattern(u8 times);
void blinkLed(u8 times);

namespace StringUtils {
    std::string removeAccents(std::string str);
    std::string removeNotAscii(std::string str);
    std::u16string UTF8toUTF16(const char* src);
}

#endif
