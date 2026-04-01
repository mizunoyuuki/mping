RFC規格やunix-mannualをもとにicmpレイヤーの実装、pingの作成

```
brew install cmake

git clone git@github.com:mizunoyuuki/mping.git && cd mping

cmake -S . -B build

cmake --build build

./build/Main 8.8.8.8
```
