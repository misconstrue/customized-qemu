#!/bin/bash
###
 # @Author: lj 1007515184@qq.com
 # @Date: 2025-05-30 14:48:22
 # @LastEditors: lj 1007515184@qq.com
 # @LastEditTime: 2025-05-30 15:21:43
 # @FilePath: /customized-qemu/build.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
### 

get_root_path() {
    related_path=$BASH_SOURCE
    abs_path=$(readlink -f $related_path)
    abs_dir=$(dirname $abs_path)
}

get_root_path
root_dir=$abs_dir
mkdir -p ${root_dir}/build
cmd=$1
arch=$2
prefix=$(readlink -f $root_dir/..)
if [ ! -z "$3" ];then
    prefix=$(readlink -f $3)
fi

config() {
    cd ${root_dir}/build
    ../configure --disable-vnc --prefix=${prefix} \
    --target-list=${arch}-softmmu --enable-slirp
}

echo $root_dir
build() {
    make -C $root_dir/build -j16
}

install() {
    make -C $root_dir/build install
}

# aarch64 riscv64 x86_64
if [[ "$cmd" == "config" ]]; then
    config $arch
elif [[ "$cmd" == "make" ]]; then
    build
elif [[ "$cmd" == "install" ]]; then
    install
fi
