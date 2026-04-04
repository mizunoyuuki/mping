「通常の read/writeでは表現で機内デバイス固有の操作をするためのシステムコール」

なぜioctlがあるのか
- Unixの設計思想は「全てはファイルである」です。デバイスもファイルとして扱う。

open() -> read()
write() -> close()

しかしデバイスには「読み書き」だけでは表現できない操作がある。

ターミナル   => ウィンドウサイズを取得したい
ネットワーク => MACアドレスを取得したい
ディスク     => パーティション情報を取得したい

これらは read/writeに無理やり詰め込めない、「何でも屋」としてのコマンドとして```ioctl```がある

- 第二引数でrequestで「何をしたいか」を指定
- 第三引数で入出力データを渡す

ioctl(fd, request, data);

MACアドレス取得までの抽象レイヤー

```
ioctl(fd, SIOCGIFHWADDR, &ifr);
システムコール
ifreqを解釈して、NICドライバに問い合わせ
ハードウェアのMAC情報を保持
ROM上にMACアドレスが書かれている。
```

具体的な流れ
- 1. ダミーソケット作成
fd = socket(AF_INET, 'ens4', IFNAMSIZ);
- 2. ifreqに「どのインターフェースか」を伝える
strncp(ifr.ifr_name, "ens4", IFNAMSIZ);
- 3. ioctlでカーネルに問い合わせ
// 「このインターフェースのHW(MAC)アドレスをくれ」という命令のコード
ioctl(fd, SIOCGIFHWADDR, &ifr);
- 4. 結果が、ifr.ifr_hwaddr.sa_dataに入って帰ってくるので書き込む
memcpy(mac, ifr.ifr_hqaddr.sa_data, 6);

---

SIOCGIFHWADDR の意味
- SIOC   -> Socket IOCtl
- G      -> Get
- IF     -> InterFace
- HWADDR -> HardWare ADDRess

命令のコードの名前がそのまま「何をするか」を指している。


