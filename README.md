RFC規格やunix-mannualをもとにicmpレイヤーの実装、pingの作成

- MacBook Pro M2 / macOS Tahoe 26.3
- Homebrew 5.1.3
- Apple clang version 17.0.0 (clang-1700.6.3.2)
- cmake version 4.3.1

```
brew install cmake

git clone git@github.com:mizunoyuuki/mping.git && cd mping

cmake -S . -B build

cmake --build build

./build/Main 8.8.8.8
```
