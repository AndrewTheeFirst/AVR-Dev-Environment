#!/usr/bin/env bash

source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/common.sh"

if_cc_add(){
    if [[ ! -f "${APP_BUILD_DIR}/compile_commands.json" ]]; then
        echo -e "${YELLOW}compile_commands.json could not be generated for this build.${CLEAR}"
        exit 1
    fi

    if [[ ! -d "${ARTS_DIR}" ]]; then
        mkdir -p "${ARTS_DIR}"
    fi

    cp "${APP_BUILD_DIR}/compile_commands.json" "${ARTS_DIR}/compile_commands.json"
    echo -e "${GREEN}compile_commands.json updated successfully.${CLEAR}"
}

on_failure(){
    echo -e "${RED}Build Finished with Errors.${CLEAR}"
}

trap 'on_failure' ERR
trap 'if_cc_add' EXIT

set -euo pipefail

cd "${APP_DIR}"

echo Starting Build... 

cmake -B build -DCMAKE_TOOLCHAIN_FILE="${TOOL_CHAIN_PATH}" ${CMAKE_BUILD_ARGS:-}

cmake --build build

echo -e "${GREEN}Build Finished Sucessfully.${CLEAR}"