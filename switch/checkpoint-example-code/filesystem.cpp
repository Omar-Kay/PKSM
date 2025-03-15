#include "filesystem.hpp"

Result FileSystem::mount(FsFileSystem* fileSystem, u64 titleID, AccountUid userID)
{
    return fsOpen_SaveData(fileSystem, titleID, userID);
}

int FileSystem::mount(FsFileSystem fs)
{
    return fsdevMountDevice("save", fs);
}

void FileSystem::unmount(void)
{
    fsdevUnmountDevice("save");
}