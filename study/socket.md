socketシステムコール

int
socket(int domain, int type, int protocol);

- domain
AF_INET: IPV4でのインターネットプロトロルのソケット ip(7)
AF_UNIX: プロセス間通信のためのソケット             unix(7)
AF_PACKET: Lowレイヤーのパケットインターフェース    packet(7)

など

- type
SOCK_STREAM
SOCK_DGRAM
SOCK_RAW

#### domain, typeでどういうソケットを作るかが大きく決まる。
AF_INET    SOCK_STREAM => TCP    => TCPをカーネルに任せる
AF_INET    SOCK_DGRAM  => UDP    => UDPをカーネルに任せる
AF_INET    SOCK_RAW    => IPヘッダから自作
AF_PACKET  SOCK_RAW    => Ethernetから自作
AF_PACKET  SOCK_DGRAM  => Ethernetヘッダはカーネルに任せる。
AF_UNIX    SOCK_STREAM => ローカルプロセス間通信


#### domain, typeでなんの値を選んだかでかなり変わる。
- protocol

// 組み合わせだけで、TCP/UDPが決まるのでprotocolの引数は0でいい。
AF_INET + SOCK_STREAM : => IPv4でstreamのタイプ     => TCP
AF_INET + SOCK_DGRAM  : => IPv4でdata_gramのタイプ  => UDP

=> socket(AF_INET, SOCK_STREAM, 0);
=> socket(AF_INET, SOCK_DGRAM, 0);

// AF_INET, SOCK_RAWの場合

socket(AF_INET, SOCK_RAW, 1)  // ICMP  => icmpレイヤーは自分で書く必要がある。
socket(AF_INET, SOCK_RAW, 6)  // TCP   => tcpレイヤーは自分で書く必要がある。

=> 基本的にSOCK_RAWを選択すると、osはipヘッダを勝手につけてくれる。
=> IP_HDRINCLをオンにすると、ipヘッダも自分でつける必要がある。ただチェックサムとかの補助はしてくれる。

