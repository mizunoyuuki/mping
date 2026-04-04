
// インターフェース名("eth0", "ens3"など)を渡すと
// 自分のmacアドレスとインターフェースインデックスを戻り引数で返してくれる関数。
// ioctlのシステムコールを使う。

// man ioctl
// int ioctl(int fd, unsigned long request, ...);
#include<sys/ioctl.h>

// man netdevice
#include<net/if.h>
#include<stdint.h>
#include<unistd.h>
#include<string.h>

void get_iface_info(const char*, uint8_t*, int*);

void get_iface_info(const char *iface, uint8_t  *mac, int *ifindex){
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, iface, IFNAMSIZ); // インターフェース名をセット
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    ioctl(fd, SIOCGIFINDEX, &ifr);
    *ifindex = ifr.ifr_ifindex;

    ioctl(fd, SIOCGIFHWADDR, &ifr);
    memcpy(mac, ifr.ifr_hwaddr.sa_data , 6);

    close(fd);

    return;
}

