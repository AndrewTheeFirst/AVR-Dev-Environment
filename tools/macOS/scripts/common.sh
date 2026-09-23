#!/usr/bin/env bash

# ANSI Escape Colors

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\x1b[33m'
CLEAR='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DEV_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
HOME_DIR="$(cd "${DEV_DIR}/.." && pwd)"
ARTS_DIR="${DEV_DIR}/artifacts"
APPS_DIR="${DEV_DIR}/applications"
TOOL_CHAIN_PATH="${DEV_DIR}/cmake/toolchain-avr.cmake"

# ("APP" should be defined as per application makefile)

APP_DIR="${APPS_DIR}/${APP}"
APP_BUILD_DIR="${APP_DIR}/build"