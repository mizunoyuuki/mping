### socket

#### socket - create an endpoint for communication

#include<sys/sockete.h>

int
socket(int domain, int type, int protocol);

socket() create an endpoint for communicatoin and returns a descriptor

The domain parameter specifies a communications domain within which communicatoin will take place;
this is selects the protocol family which should be used. these families are defined in the include file

- PF_LOCAL     Host-internal protocols, formerly called PF_UNIX
- PF_UNIX      Host-internal protocols, deprecated, use PF_LOCAL
- PF_INET      Internet version 4 protocols
- PF_ROUTE     Internal Routing protocol
- PF_KEY       Internal key-management fucntion,
- PF_INET6     Internet version 6 protocols
- PF_STSTEM    System domain, 
- PF_NDRV      Raw access to network device,
- PF_VSOCK     VM Sockets protocols

The socket has the indicated type, which specifies the semantics of communication.
Currently defined types are

- SOCK_STREAM
- SOCK_DGRAM
- SOCK_RAW

checksum
チェックサムは、ICMPタイプ（ICMP Type）から始まるICMPメッセージ全体の「1の補数和（one's complement sum）」をとった、その「1の補数（16ビット）」である。チェックサムの計算においては、チェックサム・フィールドの値はゼロとして扱うものとする。このチェックサム方式は、将来的に置き換えられる可能性がある。


1の補数和とは

2の補数は、ビット反転させて1を加える。
2の補数はCPUの加算器で引き算を足し算として扱うための表現方法。

- 前提: CPUの数値とは桁に上限がある。=> 最後のビットが繰り上がったら、その繰り上がりはどっかに行く。

現実
0001(1) + 1111(15) = 10000(16)

CPU
0001(1) + 1111(15) = 0000(0) => 繰り上がったビットは捨てられる。(こういう回路になってる。エラーとかじゃなく消える)

=> 2の補数による減算を加算として扱う方法
0001(1) - 0001(1) => 0001(1) + (~0001(1) -> 1110(ビット反転) + 1 -> 1111 ) => 000(0)

では、1の補数とは?

1の補数はビット反転をさせるだけ。

16bitワードに対する(2バイト)1の補数和とは。

ICMP全体のデータを2バイトずつに区切る(奇数バイトだった場合は、最後に0(16bit)を追加して偶数バイトととする。)

=> 
