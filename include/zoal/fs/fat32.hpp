#pragma once

#ifndef ZOAL_FS_FAT32_HPP
#define ZOAL_FS_FAT32_HPP

#include "mbr.hpp"

namespace zoal { namespace fs {
    struct __attribute__((__packed__)) bios_param_block {
        uint16_t bytes_per_sector;
        uint8_t sectors_per_cluster;
        uint16_t reserved_sector_count;
        uint8_t fat_count;
        uint16_t root_dir_entry_count;
        uint16_t total_sectors_fat16;
        uint8_t media_type;
        uint16_t sectors_per_fat_16;
        uint16_t sectors_per_track;
        uint16_t head_count;
        uint32_t hidden_sectors;
        uint32_t total_sectors_fat32;
        uint32_t sectors_per_fat32;
        uint16_t ext_flags;
        uint16_t fat32_version;
        uint32_t fat32_root_cluster;
        uint16_t fat32_info;
        uint16_t fat32_backup_boot_block;
        uint8_t fat32_reserved[12];
    };

    struct __attribute__((__packed__)) volume_boot_sector {
        uint8_t jmp_boot[3];
        char oem_name[8];
        bios_param_block bpb;
    };

    struct entry_attribute {
        static constexpr uint8_t read_only = 0x01;
        static constexpr uint8_t hidden = 0x02;
        static constexpr uint8_t system = 0x04;
        static constexpr uint8_t volume_id = 0x08;
        static constexpr uint8_t directory = 0x10;
        static constexpr uint8_t archive = 0x20;
        static constexpr uint8_t long_name = read_only | hidden | system | volume_id;
        static constexpr uint8_t long_name_mask = read_only | hidden | system | volume_id | directory;

        static inline bool is_volume(uint8_t attr) {
            return (attr & volume_id) == volume_id;
        }

        static inline bool is_file(uint8_t attr) {
            return (attr & directory) == 0;
        }

        static inline bool is_directory(uint8_t attr) {
            return (attr & directory) == directory;
        }

        static inline bool is_long_name(uint8_t attr) {
            return (attr & long_name_mask) == long_name;
        }
    };

    struct __attribute__((packed)) entry {
        uint8_t name[11];
        uint8_t attributes;
        uint8_t reserved_nt;
        uint8_t creation_time_tenths;
        uint16_t creation_time;
        uint16_t creation_date;
        uint16_t last_access_date;
        uint16_t first_cluster_high;
        uint16_t last_write_time;
        uint16_t last_write_date;
        uint16_t first_cluster_low;
        uint32_t file_size;

        inline bool is_volume() const {
            return entry_attribute::is_volume(attributes);
        }

        inline bool is_delete() const {
            return name[0] == 0xE5;
        }

        inline bool is_free() const {
            return name[0] == 0x00;
        }

        inline bool is_long_name() const {
            return entry_attribute::is_long_name(attributes);
        }
    };

    struct __attribute__((packed)) long_name_entry {
        uint8_t order;
        uint8_t name[10];
        uint8_t attributes;
        uint8_t type;
        uint8_t checksum;
        uint8_t name2[12];
        uint16_t first_cluster_low;
        uint8_t name3[4];

        inline bool is_first() const {
            return order == 0x01;
        }

        inline bool is_last() const {
            return (order & 0x40) == 0x40;
        }

        void extract_long_name(char *str) const {
            int offset = ((order & 0x0F) - 1) * 13;
            char *dst = str + offset;
            const char *src = reinterpret_cast<const char *>(name);
            dst[0] = src[0];
            dst[1] = src[2];
            dst[2] = src[4];
            dst[3] = src[6];
            dst[4] = src[8];

            src = reinterpret_cast<const char *>(name2);
            dst[5] = src[0];
            dst[6] = src[2];
            dst[7] = src[4];
            dst[8] = src[6];
            dst[9] = src[8];
            dst[10] = src[10];

            src = reinterpret_cast<const char *>(name3);
            dst[11] = src[0];
            dst[12] = src[2];
        }
    };

    template<class API>
    class volume {
    public:
        using api = API;

        template<class T>
        int read_partitions(T callback) {
            int result = api::read_block(0, buffer);
            if (result) {
                return result;
            }

            auto mbr = reinterpret_cast<zoal::fs::master_boot_record *>(buffer);
            for (auto &p : mbr->partitions) {
                int exit = callback(p);
                if (exit) {
                    return 0;
                }
            }

            return 0;
        }

        int mount_partition(uint32_t partition_sector) {
            first_partition_sector = partition_sector;

            int result = api::read_block(first_partition_sector, buffer);
            if (result) {
                return result;
            }

            auto vbr = reinterpret_cast<zoal::fs::volume_boot_sector *>(buffer);
            blocks_per_cluster = vbr->bpb.sectors_per_cluster;
            blocks_per_fat = vbr->bpb.sectors_per_fat32;
            fat_start_block = first_partition_sector + vbr->bpb.reserved_sector_count;
            first_data_block = fat_start_block + vbr->bpb.fat_count * blocks_per_fat;

            root_dir_start_block = first_block_of_cluster(vbr->bpb.fat32_root_cluster);

            return 0;
        }

        static constexpr uint8_t exclude_mask = entry_attribute::volume_id | entry_attribute::hidden;

        template<class T>
        int enum_entries(uint32_t entry_cluster, T callback, uint8_t exclude = exclude_mask) {
            int result = api::read_block(entry_cluster, buffer);
            if (result) {
                return result;
            }

            auto entry = reinterpret_cast<zoal::fs::entry *>(buffer);
            int exit = 0;
            for (int i = 0; !exit && (i < 512 / (int)sizeof(zoal::fs::entry)); i++, entry++) {
                if (entry->is_free()) {
                    break;
                }

                if (entry->is_delete()) {
                    continue;
                }

                if (entry->is_long_name()) {
                    auto lne = reinterpret_cast<zoal::fs::long_name_entry *>(entry);
                    if (lne->is_last()) {
                        memset(long_name, 0, sizeof(long_name));
                    }

                    lne->extract_long_name(long_name);
                    continue;
                }

                if (long_name[0] == 0) {
                    transform_short_name(entry);
                }

                if ((entry->attributes & exclude) == 0) {
                    exit = callback(entry, long_name);
                }
                long_name[0] = 0;
            }

            return 0;
        }

        template<class T>
        inline int enum_root(T callback, uint8_t exclude = exclude_mask) {
            return this->enum_entries(root_dir_start_block, callback, exclude);
        }

    private:
        uint32_t first_block_of_cluster(uint32_t cluster) {
            return first_data_block + (cluster - 2) * blocks_per_cluster;
        }

        static char keep_case(char ch) {
            return ch;
        }

        static char lowercase(char ch) {
            if (ch >= 'A' && ch <= 'Z') {
                return (char)(ch + ('a' - 'A'));
            }
            return ch;
        }

        void transform_short_name(zoal::fs::entry *e) {
            auto case_fn = &keep_case;
            if ((e->reserved_nt & 0x08) == 0x08) {
                case_fn = lowercase;
            }

            char *dst = long_name;
            for (size_t i = 0; i < sizeof(e->name); i++) {
                if (e->name[i] == ' ') {
                    continue;
                }

                if (i == 8) {
                    *dst++ = '.';
                    if ((e->reserved_nt & 0x10) == 0x10) {
                        case_fn = lowercase;
                    }
                }

                *dst++ = case_fn((char)e->name[i]);
            }

            *dst++ = 0;
        }

        uint32_t first_partition_sector;
        uint8_t blocks_per_cluster;
        uint32_t blocks_per_fat;
        uint32_t fat_start_block;
        uint32_t first_data_block;
        uint32_t root_dir_start_block;

        uint8_t buffer[512];
        char long_name[256];

        zoal::fs::entry current_dir;
    };
}}

#endif
