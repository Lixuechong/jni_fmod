# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2) # 最低支持的CMake版本

# Declares and names the project.

project("jni_fmod")

file(GLOB allCPP *.c *.h *.cpp) # 批量导入所有源文件,(防止源文件过多，一个一个手动录入。)

# TODO 第一步: 导入头文件
include_directories(inc)# 导入头文件,使用的是相对路径（相对于该文件的路径为根）.

# TODO 第二步: 导入库文件 (给头文件配置一个C++环境变量，让头文件的API自动寻找库文件.)
# CMAKE_CXX_FLAGS: 得到之前配置的C++的环境变量。一般都需要有。
# -L: 可以理解为系统环境变量里每个变量之间的分号分隔符。
# CMAKE_SOURCE_DIR: 得到CMakeLists.txt的磁盘路径（不包含文件本身）。
# CMAKE_ANDROID_ARCH_ABI: 自动配置jniLibs路径中的不同架构的库文件。
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}") # 配置C++环境变量
# set(CMAKE_C_FLAGS ) # 配置C环境变量

add_library(
        # Sets the name of the library.
        native-lib # 生成库的名称"libnative-lib.so"库。前面的lib是自动拼接的。而后缀是因为下面的SHARED标识是动态库标识。

        # Sets the library as a shared library.
        SHARED # 动态库

        # Provides a relative path to your source file(s).
        #             native-lib.cpp # 源文件名称
        ${allCPP}
)

# 寻找Android sdk 中的liblog.so库。（用于打印日志）
find_library( # Sets the name of the path variable.
        log-lib # 这个是把查找到的库给到 "log-lib" 变量

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# TODO 第三步: 链接打包库的api。（把${log-lib}对应的库"liblog.so"打包到"libnative_lib.so"库中。)
target_link_libraries( # Specifies the target library.
        native-lib # 被链接的总库。把下面的库文件链接到总库中。

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} # 链接 "log-lib" 变量下的库。
        fmod # 会自动拼接成libfmod.so
        fmodL # 会自动拼接成libfmodL.so
        )


#-------------------------------------------CMake的语言---------------------------------------------
# STATUS标识表示非重要消息。
# WARING 表示警告消息。
message(STATUS "输入日志")

#todo CMake变量 所有变量的类型都是string类型。

# 声明变量 set(变量名 变量值)
set(name li)

# 引用变量，打印
message("var before = ${name}")

# 移除变量
unset(name)

# 移除后使用，就没有值了，并不会崩溃。
message("var after = ${name}")

# todo CMake 列表（lists）所有变量的类型都是string类型。
# 声明列表 set(列表名 值1 值2 ...)
set(list_name 1 2 3 4 5)
# 或者
set(list_name2 "1;2;3;4;5")

message("list_name = ${list_name}")
message("list_name2 = ${list_name2}")

# todo CMake流程控制 - 条件命令
# 非0为true
# 0
set(if_tag OFF) # 定义一个变量if_tag 为false
set(elseif_tag ON) # 定义一个变量elseif_tag 为true

if (${if_tag})
    message("if")
elseif (${elseif_tag})
    message("elseif")
else (${if_tag}) # ${if_tag} 可以不加
    message("else")
endif (${if_tag}) # 结束if 可以不加

# todo CMake流程控制 - 循环命令
set(a "")
while (NOT a STREQUAL "xxx") # a STREQUAL "xxx" ：表示a是否等于xxx
    set(a "${a}x")
    message(">>>>>>>a = ${a}")
endwhile ()
# 注意：
# break() 可以跳出循环。
# continue() 可以继续下一次当前循环。

# todo foreach
foreach (item 1 2 3)
    message("1item = ${item}")
endforeach (item)

foreach (item RANGE 2) # 默认从0开始， 所以从0开始，到2结束
    message("2item = ${item}")
endforeach ()

foreach (item RANGE 1 6 2) # 最后一个参数表示步长，从 1 到 6，每次跳2，所以打印1 3 5
    message("3item = ${item}")
endforeach ()

set(list_va3 1 2 3)
foreach (item IN LISTS list_va3)
    message("4item = ${item}")
endforeach ()

# todo CMake自定义函数

