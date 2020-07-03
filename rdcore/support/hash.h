#pragma once

#include <rdapi/types.h>

class Hash
{
    public:
        Hash() = delete;
        static u16 crc16(const u8* data, size_t size);
        static u32 crc32(const u8* data, size_t size);
};

