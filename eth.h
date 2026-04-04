// イーサネットフレームのヘッダ構造体
//
#include<stdint.h>

typedef struct EthernetHeader EthernetHeader;

struct EthernetHeader {
	unsigned char dest_mac[6];
	unsigned char src_mac[6];
	uint16_t ethertype;
};
