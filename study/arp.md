man arp

Address Resolution Protocol

物理的なLANカードの住所との橋渡しをするプロトコル

#### ip link show
```
yuki0322mizuuri@my-cheap-pc:~/mping (master)$ ip link show
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: ens4: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1460 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether 42:01:0a:92:00:02 brd ff:ff:ff:ff:ff:ff
3: virbr0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default qlen 1000
    link/ether 52:54:00:16:f5:f1 brd ff:ff:ff:ff:ff:ff
4: br-2bdf269f156b: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default
    link/ether d6:f8:2a:fe:52:7f brd ff:ff:ff:ff:ff:ff
5: docker0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default
    link/ether c2:ea:a3:bf:89:70 brd ff:ff:ff:ff:ff:ff
```

2: カーネル内のインターフェースインデックス
=> ioctl(SIOCGIIFINDEX) で取得する番号のこと

ens4: インターフェース名(NICのデバイス名)

UP: インターフェースが起動している
LOWER_UP: 物理リンクが繋がっている (ケーブル/無線が接続中)
mtu 1460: 一度に送れる最大フレームサイズ
link/ether  MACアドレス(ioctl(SIGOCGIFHWADDR)で取得しているもの)

brd
ff:ff:ff:ff:ff:ff : ブロードキャストMACアドレス
=> このネットワークの全員に届けてくれという特殊なMACアドレス

通常のユニキャスト
PC-A -> (宛先 AA:BB:CC:DD:EE:FF) -> PC-Bだけ受け取る。
=> スイッチが特定のポートにだけ転送

ブロードキャスト
PC-A -> (宛先 FF:FF:FF:FF:FF:FF) -> 同じネットワークの全員が受け取る
=> スイッチが全ポートに転送

#### ip neigh show
スイッチ：
スイッチは優先(イーサネット)だけ

スイッチ機能
(LANポート1-4)

Wi-Fiアクセスポイント機能 <= 無線は別チップ、別機能

ルーター機能
ipをみて、外部のWANへデータを転送する。
送り元MACを自分のMACアドレス、
送り先のMACを送り先のMACにしてNICに書き込む、て感じ

