#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

#include "ip.h"
#include "icmp.h"
#include "eth.h"

int create_socket(void);
uint16_t icmp_checksum(void *data, size_t len);
void get_iface_info(const char *iface, uint8_t *mac, int *ifindex );

int ping(char *p){
    //*****************イーサネットフレームのデータ領域確保******************
    size_t data_len = 0x42;
    
    // Ethernetのヘッダ、IPのヘッダ、ペイロードに必要なデータを確保
    void *ethernet_frame = calloc(1, sizeof(EthernetHeader) + sizeof(IPHeader) + sizeof(ICMPHeader) + data_len);

    // 大きなeathernet_packetを分ける
    EthernetHeader *ethernet = (EthernetHeader *)ethernet_frame;
    //IPHeader *ip = (IPHeader *) (ethernet + sizeof(EthernetHeader));
    IPHeader *ip = (IPHeader*) ((uint8_t *)ethernet + sizeof(EthernetHeader)); ICMPHeader *icmp = (ICMPHeader*) ((uint8_t*)ip + sizeof(IPHeader)); //***********Ethernetヘッダ******************
    // macアドレスの確保
    uint8_t src_addr[6];
    uint8_t des_addr[6] = { 0x42, 0x01, 0x0a, 0x92, 0x00, 0x01 };
    int ifindex;
    get_iface_info("ens4", src_addr, &ifindex);

    memcpy(ethernet->src_mac, src_addr, 6);
    memcpy(ethernet->dest_mac, des_addr, 6);
    ethernet->ethertype = htons(ETH_P_IP);



    //********* ipヘッダ *************
    ip->version__ihl = 0x45; // version: 4, IHL: 5 (20 bytes)
    uint16_t total_len = sizeof(IPHeader) + sizeof(ICMPHeader) + data_len;
    ip->total_length = total_len;
    ip->time_to_live = 255;
    // ペイロードに入っているプロトコルを指定
    // ICMP: 1
    // TCP: 6
    // UDP: 17
    ip->protocol = 1;
    // inet_addr "8.8.8.8"を32ビットのバイトオーダーに変換
    ip->destination_address = inet_addr(p);

    //********** ICMPヘッダ *************
    //
    icmp->type = 8;
    icmp->code = 0;
    icmp->identifier = 0x1234;
    icmp->sequence_number = 1;
    icmp->checksum = icmp_checksum(icmp, sizeof(ICMPHeader) + data_len);

    int sock_fd = create_socket();

    size_t packet_size = sizeof(EthernetHeader) + sizeof(IPHeader) + sizeof(ICMPHeader) + data_len;

    struct sockaddr_ll dest_ll;
    memset(&dest_ll, 0, sizeof(dest_ll));
    dest_ll.sll_family   = AF_PACKET;
    dest_ll.sll_ifindex  = ifindex;
    dest_ll.sll_halen    = ETH_ALEN;
    memcpy(dest_ll.sll_addr, des_addr, ETH_ALEN);

    int bytes = sendto(sock_fd, ethernet_frame, packet_size, 0, (struct sockaddr *)&dest_ll, sizeof(dest_ll));

    printf("sent %d bytes to %s\n", bytes, p);


    char recv_buf[1024];
    int recv_bytes = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, NULL, NULL);

    IPHeader   *recv_ip   = (IPHeader *)(recv_buf + sizeof(EthernetHeader));
    int ip_header_len = (recv_ip->version__ihl & 0x0F) * 4;

    ICMPHeader *recv_icmp = (ICMPHeader *)(recv_buf + sizeof(EthernetHeader) + sizeof(IPHeader));

    struct in_addr src_ip;
    src_ip.s_addr = recv_ip->source_address;

    printf("received %d bytes from %s: type=%d ttl=%d\n",
         recv_bytes,
         inet_ntoa(src_ip),
         recv_icmp->type,
         recv_ip->time_to_live);



    return 0;
}

int create_socket(void){
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0){
        perror("socket create error");
        return -1;
    }

    return sockfd;
}

uint16_t add_ones_complement(uint16_t sum, uint16_t val);

uint16_t icmp_checksum(void *data, size_t len) {
    uint16_t *buf = (uint16_t *)data;
    uint16_t sum = 0;

    while (len > 1) {
        uint16_t val = *buf++;

        sum = add_ones_complement(sum, val);
        len -= 2;
    }
    if (len == 1) {
        uint16_t val = *(uint16_t *)buf;
        sum = add_ones_complement(sum, val);
    }

    return ~sum;


}

uint16_t add_ones_complement(uint16_t sum, uint16_t val){
    if (sum > (uint16_t)(sum + val)){
        return sum + val + 1;
    } else {
        return sum + val;
    }
}
