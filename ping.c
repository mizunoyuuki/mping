#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "ip.h"
#include "icmp.h"

int create_socket(void);
uint16_t icmp_checksum(void *data, size_t len);

int ping(char *p){
    // とりあえず固定長
    size_t data_len = 0x42;
    // IPのヘッダ、ペイロードに必要なデータを確保
    void *ip_header = calloc(1, sizeof(IPHeader) + sizeof(ICMPHeader) + data_len);
    // IPヘッダとペイロード(ICMPヘッダ + data)部分に分ける。
    IPHeader *ip = (IPHeader *)ip_header;
    ICMPHeader *icmp = (ICMPHeader *)(ip_header + sizeof(IPHeader));

    //********* ipヘッダ *************
    //
    ip->version__ihl = 0x45; // version: 4, IHL: 5 (20 bytes)
    ip->total_length = sizeof(IPHeader) + sizeof(ICMPHeader) + data_len;
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


    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = PF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(p);
    size_t packet_size = sizeof(IPHeader) + sizeof(ICMPHeader) + data_len;

    int bytes = sendto(sock_fd, ip_header, packet_size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

    //int bytes = sendto(sock_fd, ip_header, packet_size, 0, NULL, 0);



    printf("sent %d bytes to %s\n", bytes, p);


    char recv_buf[1024];
    struct sockaddr src_addr;
    socklen_t addr_len = sizeof(src_addr);

    int recv_bytes = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&src_addr, &addr_len);

    IPHeader   *recv_ip   = (IPHeader *)recv_buf;
    int ip_header_len = (recv_ip->version__ihl & 0x0F) * 4;

    ICMPHeader *recv_icmp = (ICMPHeader *)(recv_buf + sizeof(IPHeader));

    printf("received %d bytes from %s: type=%d ttl=%d\n",
         recv_bytes,
         inet_ntoa(src_addr.sin_addr),
         recv_icmp->type,
         recv_ip->time_to_live);



    return 0;
}

int create_socket(void){
    int sockfd = socket(PF_INET, SOCK_RAW, 1);
    if (sockfd < 0){
        perror("socket create error");
        return -1;
    }

    int on = 1;
    setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));

    return sockfd;
}


uint16_t icmp_checksum(void *data, size_t len) {
      uint16_t *buf = data;
      uint32_t sum = 0;

      while (len > 1) {
          sum += *buf++;
          len -= 2;
      }
      if (len == 1) {
          sum += *(uint8_t *)buf;
      }

      sum = (sum >> 16) + (sum & 0xFFFF);
      sum += (sum >> 16);

      return ~sum;
  }
