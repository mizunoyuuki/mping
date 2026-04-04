PCI接続とは

PCIとはPeripheral Component Interconnectの略で、マザーボードに拡張カードを接続するための規格

PCIスロット1 グラボ
PCIスロット2 サウンドカード
PCIスロット3 NIC

NIC(ネットワークカード) もこのスロットに差し込む拡張カードの一種

今は PCI Express (PCIe)が主流

PCI(1992) -> PCI-X -> PCI Express / PCIe

今回のクラウドのVMなので、物理的なカードはない。仮想的にPCIスロット4番にNICが接続されたことにしている。
NICという意味で、ens4

ens4
=> e: ethernet
=> n: PCI接続
=> s4: スロット4

=> PCIスロットの4番に接続されたEthernet
