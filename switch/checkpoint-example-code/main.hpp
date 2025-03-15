#ifndef MAIN_HPP
#define MAIN_HPP

#include "InputState.hpp"
#include "Screen.hpp"
#include "account.hpp"
#include "title.hpp"
#include "util.hpp"
#include <memory>
#include <switch.h>

typedef enum { SORT_ALPHA, SORT_LAST_PLAYED, SORT_PLAY_TIME, SORT_MODES_COUNT } sort_t;

inline float g_currentTime = 0;
inline AccountUid g_currentUId;
inline bool g_backupScrollEnabled       = 0;
inline bool g_notificationLedAvailable  = false;
inline std::shared_ptr<Screen> g_screen = nullptr;
inline bool g_ftpAvailable              = false;
inline bool g_shouldExitNetworkLoop     = false;
inline std::string g_selectedCheatKey;
inline std::vector<std::string> g_selectedCheatCodes;
inline u32 g_username_dotsize;
inline sort_t g_sortMode         = SORT_ALPHA;
inline const InputState* g_input = nullptr;
inline std::string g_currentFile = "";
inline bool g_isTransferringFile = false;

#endif