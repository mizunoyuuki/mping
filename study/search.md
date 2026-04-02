実装のイメージ、
# ping実行ファイルの実装: pingドライバ的なもの
# ICMPライブラリ        : ICMPプロトコル本体


pingの実行結果。
```
mizuno-pc@my-cheap-pc:~$ ping 8.8.8.8
PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
64 bytes from 8.8.8.8: icmp_seq=1 ttl=115 time=0.399 ms
64 bytes from 8.8.8.8: icmp_seq=2 ttl=115 time=0.248 ms
64 bytes from 8.8.8.8: icmp_seq=3 ttl=115 time=0.225 ms
```

=> body(ペイロード)部分には56バイトのデータが入っている。(84は不明(差分28))

## ICMP
RFC 792 - Internet Control Message Protocol
=> https://datatracker.ietf.org/doc/html/rfc792

=> Destination Unreachable Message
=> Time Exceeded Message
=> Parameter Problem Message
=> Source Quench Message
=>  Redirect Message
=> Echo or Echo Reply Message
=> Timestamp or Timestamp Reply Message
=> Information Request or Information Reply Message

とりあえずpingの機能って、ピンポンだけと思うのでとりあえずecho, replyだけみてみる。

以下引用

```
Echo or Echo Reply Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Identifier          |        Sequence Number        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Data ...
   +-+-+-+-+-

   IP Fields:

   Addresses

      The address of the source in an echo message will be the
      destination of the echo reply message.  To form an echo reply
      message, the source and destination addresses are simply reversed,
      the type code changed to 0, and the checksum recomputed.

   IP Fields:

   Type

      8 for echo message;

      0 for echo reply message.

   Code

      0

   Checksum

      The checksum is the 16-bit ones's complement of the one's
      complement sum of the ICMP message starting with the ICMP Type.
      For computing the checksum , the checksum field should be zero.
      If the total length is odd, the received data is padded with one
      octet of zeros for computing the checksum.  This checksum may be
      replaced in the future.

   Identifier

      If code = 0, an identifier to aid in matching echos and replies,
      may be zero.

   Sequence Number
```

=> この構造体をicmp.hに定義する。


## IP

RFC 791 Internet Protocol
https://datatracker.ietf.org/doc/html/rfc791

```
2.1.  Relation to Other Protocols

  The following diagram illustrates the place of the internet protocol
  in the protocol hierarchy:


                 +------+ +-----+ +-----+     +-----+
                 |Telnet| | FTP | | TFTP| ... | ... |
                 +------+ +-----+ +-----+     +-----+
                       |   |         |           |
                      +-----+     +-----+     +-----+
                      | TCP |     | UDP | ... | ... |
                      +-----+     +-----+     +-----+
                         |           |           |
                      +--------------------------+----+
                      |    Internet Protocol & ICMP   |
                      +--------------------------+----+
                                     |
                        +---------------------------+
                        |   Local Network Protocol  |
                        +---------------------------+

                         Protocol Relationships

                               Figure 1.
```

=> ほー


headerの定義あった。

```
  A summary of the contents of the internet header follows:


    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Version|  IHL  |Type of Service|          Total Length         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Identification        |Flags|      Fragment Offset    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Time to Live |    Protocol   |         Header Checksum       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       Source Address                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Destination Address                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Options                    |    Padding    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                    Example Internet Datagram Header

                               Figure 4.
```


## socketの作り方

```
SOCKET(2)                                                                       System Calls Manual                                                                       SOCKET(2)

NAME
     socket – create an endpoint for communication

SYNOPSIS
     #include <sys/socket.h>

     int
     socket(int domain, int type, int protocol);

DESCRIPTION
     socket() creates an endpoint for communication and returns a descriptor.

     The domain parameter specifies a communications domain within which communication will take place; this selects the protocol family which should be used.  These families are
     defined in the include file ⟨sys/socket.h⟩.  The currently understood formats are

           PF_LOCAL        Host-internal protocols, formerly called PF_UNIX,
           PF_UNIX         Host-internal protocols, deprecated, use PF_LOCAL,
           PF_INET         Internet version 4 protocols,
           PF_ROUTE        Internal Routing protocol,
           PF_KEY          Internal key-management function,
           PF_INET6        Internet version 6 protocols,
           PF_SYSTEM       System domain,
           PF_NDRV         Raw access to network device,
           PF_VSOCK        VM Sockets protocols

     The socket has the indicated type, which specifies the semantics of communication.  Currently defined types are:

           SOCK_STREAM
           SOCK_DGRAM
           SOCK_RAW

     A SOCK_STREAM type provides sequenced, reliable, two-way connection based byte streams.  An out-of-band data transmission mechanism may be supported.  A SOCK_DGRAM socket
     supports datagrams (connectionless, unreliable messages of a fixed (typically small) maximum length).  SOCK_RAW sockets provide access to internal network protocols and
     interfaces.  The type SOCK_RAW, which is available only to the super-user.

     The protocol specifies a particular protocol to be used with the socket.  Normally only a single protocol exists to support a particular socket type within a given protocol
     family.  However, it is possible that many protocols may exist, in which case a particular protocol must be specified in this manner.  The protocol number to use is
     particular to the “communication domain” in which communication is to take place; see protocols(5).

     Sockets of type SOCK_STREAM are full-duplex byte streams, similar to pipes.  A stream socket must be in a connected state before any data may be sent or received on it.  A
     connection to another socket is created with a connect(2) or connectx(2) call.  Once connected, data may be transferred using read(2) and write(2) calls or some variant of
     the send(2) and recv(2) calls.  When a session has been completed a close(2) may be performed.  Out-of-band data may also be transmitted as described in send(2) and received
     as described in recv(2).

     The communications protocols used to implement a SOCK_STREAM insure that data is not lost or duplicated.  If a piece of data for which the peer protocol has buffer space
     cannot be successfully transmitted within a reasonable length of time, then the connection is considered broken and calls will indicate an error with -1 returns and with
     ETIMEDOUT as the specific code in the global variable errno.  The protocols optionally keep sockets “warm” by forcing transmissions roughly every minute in the absence of
     other activity.  An error is then indicated if no response can be elicited on an otherwise idle connection for a extended period (e.g. 5 minutes).  A SIGPIPE signal is raised
     if a process sends on a broken stream; this causes naive processes, which do not handle the signal, to exit.

     SOCK_DGRAM and SOCK_RAW sockets allow sending of datagrams to correspondents named in send(2) calls.  Datagrams are generally received with recvfrom(2), which returns the
     next datagram with its return address.

     An fcntl(2) call can be used to specify a process group to receive a SIGURG signal when the out-of-band data arrives.  It may also enable non-blocking I/O and asynchronous
     notification of I/O events via SIGIO.

     The operation of sockets is controlled by socket level options.  These options are defined in the file ⟨sys/socket.h⟩.  Setsockopt(2) and getsockopt(2) are used to set and
     get options, respectively.
```
