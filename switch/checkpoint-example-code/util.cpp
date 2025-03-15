#include "util.hpp"

void servicesExit(void)
{
    Logger::getInstance().flush();

    if (g_ftpAvailable)
        ftp_exit();
    if (g_notificationLedAvailable)
        hidsysExit();
    pdmqryExit();
    socketExit();
    Account::exit();
    freeIcons();
    SDLH_Exit();
    nsExit();
    plExit();
    romfsExit();
}

Result servicesInit(void)
{
    io::createDirectory("sdmc:/switch");
    io::createDirectory("sdmc:/switch/Checkpoint");
    io::createDirectory("sdmc:/switch/Checkpoint/saves");

    Logger::getInstance().log(Logger::INFO, "Starting Checkpoint loading...");

    if (appletGetAppletType() != AppletType_Application) {
        Logger::getInstance().log(Logger::WARN, "Please do not run Checkpoint in applet mode.");
    }

    Result socinit = 0;
    if ((socinit = socketInitializeDefault()) == 0) {
        // nxlinkStdio();
    }
    else {
        Logger::getInstance().log(Logger::INFO, "Unable to initialize socket. Result code 0x%08lX.", socinit);
    }

    g_shouldExitNetworkLoop = R_FAILED(socinit);

    Result res = 0;

    romfsInit();

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    hidInitializeTouchScreen();

    if (R_FAILED(res = plInitialize(PlServiceType_User))) {
        Logger::getInstance().log(Logger::ERROR, "plInitialize failed. Result code 0x%08lX.", res);
        return res;
    }

    if (R_FAILED(res = Account::init())) {
        Logger::getInstance().log(Logger::ERROR, "Account::init failed. Result code 0x%08lX.", res);
        return res;
    }

    if (R_FAILED(res = nsInitialize())) {
        Logger::getInstance().log(Logger::ERROR, "nsInitialize failed. Result code 0x%08lX.", res);
        return res;
    }

    if (!SDLH_Init()) {
        Logger::getInstance().log(Logger::ERROR, "SDLH_Init failed. Result code 0x%08lX.", res);
        return -1;
    }

    if (R_SUCCEEDED(res = hidsysInitialize())) {
        g_notificationLedAvailable = true;
    }
    else {
        Logger::getInstance().log(Logger::INFO, "Notification led not available. Result code 0x%08lX.", res);
    }

    Configuration::getInstance();

    if (R_SUCCEEDED(socinit)) {
        if (R_SUCCEEDED(res = ftp_init())) {
            g_ftpAvailable = true;
            Logger::getInstance().log(Logger::INFO, "FTP Server successfully loaded.");
        }
        else {
            Logger::getInstance().log(Logger::INFO, "FTP Server failed to load. Result code 0x%08lX.", res);
        }
    }

    if (R_SUCCEEDED(res = pdmqryInitialize())) {}
    else {
        Logger::getInstance().log(Logger::WARN, "pdmqryInitialize failed with result 0x%08lX.", res);
    }

    Logger::getInstance().log(Logger::INFO, "Checkpoint loading completed!");

    return 0;
}

std::u16string StringUtils::UTF8toUTF16(const char* src)
{
    char16_t tmp[256] = {0};
    utf8_to_utf16((uint16_t*)tmp, (uint8_t*)src, 256);
    return std::u16string(tmp);
}

// https://stackoverflow.com/questions/14094621/change-all-accented-letters-to-normal-letters-in-c
std::string StringUtils::removeAccents(std::string str)
{
    std::u16string src = UTF8toUTF16(str.c_str());
    const std::u16string illegal = UTF8toUTF16("ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüūýþÿ");
    const std::u16string fixed = UTF8toUTF16("AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuuypy");

    for (size_t i = 0, sz = src.length(); i < sz; i++) {
        size_t index = illegal.find(src[i]);
        if (index != std::string::npos) {
            src[i] = fixed[index];
        }
    }

    return UTF16toUTF8(src);
}

std::string StringUtils::removeNotAscii(std::string str)
{
    for (size_t i = 0, sz = str.length(); i < sz; i++) {
        if (!isascii(str[i])) {
            str[i] = ' ';
        }
    }
    return str;
}

HidsysNotificationLedPattern blinkLedPattern(u8 times)
{
    HidsysNotificationLedPattern pattern;
    memset(&pattern, 0, sizeof(pattern));

    pattern.baseMiniCycleDuration = 0x1;   // 12.5ms.
    pattern.totalMiniCycles       = 0x2;   // 2 mini cycles.
    pattern.totalFullCycles       = times; // Repeat n times.
    pattern.startIntensity        = 0x0;   // 0%.

    pattern.miniCycles[0].ledIntensity      = 0xF; // 100%.
    pattern.miniCycles[0].transitionSteps   = 0xF; // 15 steps. Total 187.5ms.
    pattern.miniCycles[0].finalStepDuration = 0x0; // Forced 12.5ms.
    pattern.miniCycles[1].ledIntensity      = 0x0; // 0%.
    pattern.miniCycles[1].transitionSteps   = 0xF; // 15 steps. Total 187.5ms.
    pattern.miniCycles[1].finalStepDuration = 0x0; // Forced 12.5ms.

    return pattern;
}

void blinkLed(u8 times)
{
    if (g_notificationLedAvailable) {
        PadState pad;
        padInitializeDefault(&pad);
        s32 n;
        HidsysUniquePadId uniquePadIds[2]    = {0};
        HidsysNotificationLedPattern pattern = blinkLedPattern(times);
        memset(uniquePadIds, 0, sizeof(uniquePadIds));
        Result res = hidsysGetUniquePadsFromNpad(padIsHandheld(&pad) ? HidNpadIdType_Handheld : HidNpadIdType_No1, uniquePadIds, 2, &n);
        if (R_SUCCEEDED(res)) {
            for (s32 i = 0; i < n; i++) {
                hidsysSetNotificationLedPattern(&pattern, uniquePadIds[i]);
            }
        }
    }
}
