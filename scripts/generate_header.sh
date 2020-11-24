#!/bin/bash

function system_headers() {
    local header="$1"
    local deps="$2"
    clang -E -x c++ $deps $header 2>/dev/null | \
        grep -E '^#([^/]*/)*[^."]*" 1 3' | \
        awk -F '"' '{print $2}' | \
        awk -F '/' '{print $NF}' | \
        xargs -i echo "#include <{}>";
}

function download_unconstexpr() {
    if [ ! -d unconstexpr-cpp20 ]; then
        mkdir -p build_header;
        cd build_header;
        cmake -DCMAKE_CXX_COMPILER_VERSION=10.0 ..; #force meet the requirements
        cd -;
        rm -fr build_header;
    fi;
}

function precompile_without_system_headers() {
    local header="$1"
    local deps="$2"
    clang++ $deps -E -P -nostdinc -nostdinc++ $header;
}

function gen_header() {
    local header="$1"
    local deps="$2"
    echo "#pragma once";
    system_headers $1 $2
    precompile_without_system_headers $1 $2
}

download_unconstexpr;
gen_header "inc/result.hpp" "-Iunconstexpr-cpp20/include/" | cat -s