#!/bin/bash

if [ $# != 2 ] ; then 
    echo "USAGE: $0 EXE_NAME DMP_NAME" 
    echo " e.g.: $0 test 3872B2CF-983B-4963-AFA9-C8534DFD4C44.dmp" 
    exit 1
fi 

# 获取输入参数
exe_file_name=$1
dmp_file_name=$2

getSymbol() {
    echo "@getSymbol: start get symbol"
    ./dump_syms ./$exe_file_name > $exe_file_name'.sym'
}

getStackTrace() {
    echo "@getStackTrace: start get StackTrace"
    sym_file_name=$exe_file_name'.sym'

    # 获取 $sym_file_name 的第一行
    line1=$(head -n1 $sym_file_name)
    echo $line1

    # 从第一行的字符串中获取版本号
    OIFS=$IFS; IFS=" "; set -- $line1; aa=$1; bb=$2; cc=$3; dd=$4; IFS=$OIFS 
    version_number=$dd

    # 创建标准目录并将 *.sym 移动到该目录中
    mkdir -p ./symbols/$exe_file_name/$version_number
    mv $sym_file_name ./symbols/$exe_file_name/$version_number

    # 打印堆栈跟踪到 std 输出
    ./minidump_stackwalk $dmp_file_name ./symbols &> result.txt
}

main() {
    getSymbol 
    if [ $? == 0 ]; then 
        getStackTrace
    fi
}

# 运行 main
main
