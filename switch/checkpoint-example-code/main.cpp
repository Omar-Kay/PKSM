#include "main.hpp"
#include "MainScreen.hpp"
extern "C" {
#include "ftp.h"
}

static void networkLoop(void)
{
    while (appletMainLoop() && !g_shouldExitNetworkLoop) {
        Configuration::getInstance().pollServer();
        if (g_ftpAvailable && Configuration::getInstance().isFTPEnabled()) {
            ftp_loop();
        }
    }
}

int main(void)
{
    Result res = servicesInit();
    if (R_FAILED(res)) {
        servicesExit();
        exit(res);
    }

    InputState input;
    g_input = &input;
    PadState pad;
    padInitializeDefault(&pad);

    g_screen = std::make_unique<MainScreen>(input);

    loadTitles();
    // get the user IDs
    std::vector<AccountUid> userIds = Account::ids();
    // set g_currentUId to a default user in case we loaded at least one user
    if (g_currentUId == 0)
        g_currentUId = userIds.at(0);

    Thread networkThread;
    threadCreate(&networkThread, (ThreadFunc)networkLoop, nullptr, nullptr, 16 * 1000, 0x2C, -2);
    threadStart(&networkThread);

    while (appletMainLoop()) {
        padUpdate(&pad);

        input.kDown = padGetButtonsDown(&pad);
        if (input.kDown & HidNpadButton_Plus)
            break;

        input.kHeld = padGetButtons(&pad);
        input.kUp   = padGetButtonsUp(&pad);
        hidGetTouchScreenStates(&input.touch, 1);

        g_screen->doDraw();
        g_screen->doUpdate(input);
        SDLH_Render();
    }

    g_shouldExitNetworkLoop = true;
    threadWaitForExit(&networkThread);
    threadClose(&networkThread);

    servicesExit();
    exit(0);
}
