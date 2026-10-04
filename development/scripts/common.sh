#!/bin/bash

# ANSI Escape Colors

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\x1b[33m'
CLEAR='\033[0m'

HOME_DIR=/workspace
DEV_DIR=${HOME_DIR}/development
ARTS_DIR="${DEV_DIR}/artifacts"
APPS_DIR="${DEV_DIR}/applications"

# ("APP" should be defined as per application makefile)

APP_DIR="${APPS_DIR}/${APP}"
APP_BUILD_DIR="${APP_DIR}/build"