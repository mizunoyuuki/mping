//struct ICMPHeader {
//    uint8_t  type;
//    uint8_t  code;
//    uint16_t checksum;
//    uint16_t identifier;
//    uint16_t sequence_number;
//    uint8_t  data[];
//};
//

#include<stdlib.h>
#include<string.h>
#include "icmp.h"

uint16_t icmp_checksum(void);

ICMPHeader *create_icmp_header(uint8_t type, uint8_t code, uint16_t identifier, uint16_t sequence_number, uint8_t *data, size_t data_length) {
    // ヘッダ + データサイズのメモリ
    ICMPHeader *header = malloc(sizeof(ICMPHeader) + data_length);
    header->type = type;
    header->code = code;
    header->checksum = icmp_checksum();
    header->identifier = identifier;
    header->sequence_number = sequence_number;
    memcpy(header->data, data, data_length);
    return header;
}

uint16_t icmp_checksum(){
    return 0;
}
