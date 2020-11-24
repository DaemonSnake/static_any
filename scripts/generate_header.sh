#!/bin/bash

exclude_list="concepts" #pipe separated

function system_headers() {
    local header="$1"
    local deps="$2"
    clang -E -x c++ $deps $header 2>/dev/null | \
        grep -E '^#([^/]*/)*[^."]*" 1 3' | \
        awk -F '"' '{print $2}' | \
        awk -F '/' '{print $NF}' | \
        grep -vE "^($exclude_list)$" |\
        xargs -i echo "#include <{}>";
}

function download_unconstexpr() {
    if [ ! -d unconstexpr-cpp20 ]; then
        git clone https://github.com/DaemonSnake/unconstexpr-cpp20.git
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