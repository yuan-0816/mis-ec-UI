cmake_minimum_required(VERSION 3.10)
project(msi_ec_control_gui)

# 設定 C 標準
set(CMAKE_C_STANDARD 11)

# 查找 GTK3 庫
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# 包含 GTK3 的標頭文件
include_directories(${GTK3_INCLUDE_DIRS})

# 添加編譯選項
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

# 添加可執行文件
add_executable(msi_ec_control_gui src/msi_ec_control_gui.c)

# 連接 GTK3 庫
target_link_libraries(msi_ec_control_gui ${GTK3_LIBRARIES})
