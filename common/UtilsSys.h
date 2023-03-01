#pragma once
#include <string>

void ReadFile(const std::string &path, std::string &content);

inline int GetIndexOfPath(const std::string &path)
{
    int index = path.find_last_of('/');
    if (-1 == index)
    {
        index = path.find_last_of('\\');
    }

    return index;
}

inline std::string GetFileName(const std::string &path)
{
    int index = GetIndexOfPath(path);
    if (-1 == index)
        return path;

    return path.substr(index + 1);
}

inline std::string GetBasePath(const std::string &path)
{
    int index = GetIndexOfPath(path);
    if (-1 == index)
        return "";

    return path.substr(0, index + 1);
}
