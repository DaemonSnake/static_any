#!/bin/bash

function system_headers() {
    local header="$1"
    local deps="$2"
    cpp -x c++ $deps $header 2>/dev/null | \
        grep -E '^#.*[^.h]" 1 3' | \
        awk -F '"' '{print $2}' | \
        awk -F '/' '{print $NF}' | \
        xargs -i echo "#include <{}>";
}

function precompile_without_system_headers() {
    local header="$1"
    local deps="$2"
    clang++ $deps -P -E -nostdinc -nostdinc++ $header 2>/dev/null;
}

function gen_header() {
    local header="$1"
    local deps="$2"
    system_headers $1 $2
    precompile_without_system_headers $1 $2
}

gen_header "inc/result.hpp" "-Iunconstexpr-cpp20/include/" | cat -s