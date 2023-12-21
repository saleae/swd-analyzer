#ifndef VERSIONINFO_H
#define VERSIONINFO_H

#include <cinttypes>

struct VersionInfo
{
    uint8_t major;
    uint8_t minor;
    uint16_t patch;
    uint32_t build;
    char *date;
    char *time;
    char *hash;
};

#endif // VERSIONINFO_H
