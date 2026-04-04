#include<stdio.h>
#include<sys/ioctl.h>
#include<string.h>
#include<net/if.h>
#include<unistd.h>

// int ioctl(int fd, unsigned long request, ...);

int main(){
    // struct ifreq
    // struct ifreq {
    //     char ifr_name[IFNAMESIZ]; / Interface name /
    //     union {
    //         struct sockaddr ifr_addr;
    //         struct sockaddr ifr_dstaddr;
    //         struct sockaddr ifr_broadaddr;
    //         struct sockaddr ifr_netmask;
    //         struct sockaddr ifr_hwaddr;
    //         short           ifr_flags;
    //         int             ifr_ifindex;
    //         int             ifr_metric;
    //         int             ifr_mtu;
    //         struct ifmap    ifr_map;
    //         char            ifr_slave[IFNAMSIZ];
    //         char            ifr_newname[IFNAMSIZ];
    //         char            *ifr_data;
    //     };
    // };
    // ifreqは、NICそのものを操作するための構造体
    //
    // 使い方の特徴としては、ifr.ifr_nameに インターフェース名を入れる。
    // 該当MACアドレスを、ifr.ifr_hwaddr.sa_dataに書き込む
    struct ifreq ifr;

    memset(&ifr, 0, sizeof(ifr));
    char *iface = "ens4";

    // memcpyとstrncpyの違い
    // 何をコピーするか
    // memcpy  => バイト列(なんでも)
    // strncpy => 文字列
    // strncpyは、nバイトか、\0に達するまでのデータをコピーする
    // memcpyにとって、\0は単なるバイト
    strncpy(ifr.ifr_name, iface, IFNAMSIZ);
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    ioctl(fd, SIOCGIFHWADDR, &ifr);

    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    close(fd);
    return 0;
}

