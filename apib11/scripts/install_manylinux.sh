#!/usr/bin/env bash

cd /tmp

curl -o cheese.rpm http://www.nosuchhost.net/~cheese/fedora/packages/epel-9/x86_64/cheese-release-9-1.el9.noarch.rpm

rpm -Uvh ./cheese.rpm

dnf install --setopt=install_weak_deps=False -y cmake proj proj-devel gdal-devel libomp libomp-devel xerces-c xerces-c-devel

mkdir -p /project/src/MMVII/bin/
cp /usr/lib64/flexiblas64/libflexiblas_*.so /project/src/MMVII/bin/

manylinux-install-clang -v 22.1.4.0

