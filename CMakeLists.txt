
cmake_minimum_required(VERSION 3.10)  #　CMakeの最低バージョンをしてい

project(StudyCmake VERSION 1.0 LANGUAGES C) # プロジェクトの名前とバージョン
add_executable(Main main.c)               # 実行ファイルとソースファイル

# add_library(Lib lib.c) # ライブラリの追加
# target_link_libraries(Mylib STATIC Lib) # ライブラリのリンク
# target_include_directories(Main PRIVATE include/)    # includeディレクトリの追加
# set(CMAKE_C_STANDARD 11) # 変数の設定

# ビルド手順
# 1. CMakeLists.txtを作成
# 2. buildディレクトリを作成 
     # mkdir build && cd build
# 3. cmake ..
# 4. cmake --build .
# 5. ./Main

# --- CTest ---
enable_testing()

# テスト1: main の実行テスト（前と同じ）
add_test(NAME run_main COMMAND Main)

# テスト2: Cで書いた自作テスト
# add_executable(test_calc test_calc.c)   # テスト用バイナリを作る
