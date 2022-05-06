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
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} # 连接 "log-lib" 变量下的库。
        fmod # 会自动拼接成libfmod.so
        fmodL # 会自动拼接成libfmodL.so
        )