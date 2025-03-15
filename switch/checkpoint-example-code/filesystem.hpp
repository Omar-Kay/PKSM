#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "account.hpp"
#include <switch.h>

namespace FileSystem {
    Result mount(FsFileSystem* fileSystem, u64 titleID, AccountUid userID);
    int mount(FsFileSystem fs);
    void unmount(void);
}

#endif