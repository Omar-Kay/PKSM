#ifndef TITLE_HPP
#define TITLE_HPP

#include "SDLHelper.hpp"
#include "account.hpp"
#include "configuration.hpp"
#include "filesystem.hpp"
#include "io.hpp"
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <switch.h>
#include <unordered_map>
#include <utility>
#include <vector>

class Title {
public:
    void init(u8 saveDataType, u64 titleid, AccountUid userID, const std::string& name, const std::string& author);
    ~Title() = default;

    std::string author(void);
    std::pair<std::string, std::string> displayName(void);
    SDL_Texture* icon(void);
    u64 id(void);
    std::string name(void);
    std::string path(void);
    u64 playTimeNanoseconds(void);
    std::string playTime(void);
    void playTimeNanoseconds(u64 playTimeNanoseconds);
    u32 lastPlayedTimestamp(void);
    void lastPlayedTimestamp(u32 lastPlayedTimestamp);
    std::string fullPath(size_t index);
    void refreshDirectories(void);
    u64 saveId();
    void saveId(u64 id);
    std::vector<std::string> saves(void);
    u8 saveDataType(void);
    AccountUid userId(void);
    std::string userName(void);

private:
    u64 mId;
    u64 mSaveId;
    AccountUid mUserId;
    std::string mUserName;
    std::string mName;
    std::string mSafeName;
    std::string mAuthor;
    std::string mPath;
    std::vector<std::string> mSaves;
    std::vector<std::string> mFullSavePaths;
    u8 mSaveDataType;
    std::pair<std::string, std::string> mDisplayName;
    u64 mPlayTimeNanoseconds;
    u32 mLastPlayedTimestamp;
};

void getTitle(Title& dst, AccountUid uid, size_t i);
size_t getTitleCount(AccountUid uid);
void loadTitles(void);
void sortTitles(void);
void rotateSortMode(void);
void refreshDirectories(u64 id);
bool favorite(AccountUid uid, int i);
void freeIcons(void);
SDL_Texture* smallIcon(AccountUid uid, size_t i);
std::unordered_map<std::string, std::string> getCompleteTitleList(void);

#endif