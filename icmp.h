#include <stdint.h>

typedef struct ICMPHeader ICMPHeader;

struct ICMPHeader {
    // 4バイト
    uint8_t  type;
    uint8_t  code;
    uint16_t checksum;

    // 4バイト
    uint16_t identifier;
    uint16_t sequence_number;

    // 可変長バイト
    uint8_t  data[];
};

// 構造体の最後には可変長の配列をおくことができる。(mccでは未対応)
