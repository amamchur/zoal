#pragma once

#ifndef ZOAL_FS_MBR_HPP
#define ZOAL_FS_MBR_HPP

#include <stdint.h>

namespace zoal { namespace fs {
    struct __attribute__((packed)) partition_table {
        uint8_t boot;
        uint8_t begin_head;
        uint8_t begin_sector : 6;
        uint8_t begin_cylinder_high : 2;
        uint8_t begin_cylinder_low;
        uint8_t type;
        uint8_t end_head;
        uint8_t end_sector : 6;
        uint8_t end_cylinder_high : 2;
        uint8_t end_cylinder_low;
        uint32_t first_sector;
        uint32_t total_sectors;
    };

    struct __attribute__((packed)) master_boot_record {
        uint8_t bootstrap[446];
        partition_table partitions[4];
        uint8_t boot_signature[2];
    };
}}

#endif
