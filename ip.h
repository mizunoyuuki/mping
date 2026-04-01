#include <stdint.h>

typedef struct IPHeader IPHeader;

struct IPHeader {
    uint8_t version__ihl;  // いきなり1バイトに二つの情報を詰め込む
    uint8_t type_of_searvice;
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags__fragment_offset; // 2バイトを3ビット、13ビットの分割して格納
    uint8_t time_to_live;
    uint8_t protocol;
    uint16_t header_checksum;
    uint32_t source_address;
    uint32_t destination_address;
    uint8_t options[];    // 可変長のoptionらしい。
};
