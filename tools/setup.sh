#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TOOLS_DIR="${ROOT_DIR}/tools"
DEV_DIR="${ROOT_DIR}/development"

log() { printf '%s\n' "$*"; }
warn() { printf 'WARN: %s\n' "$*" >&2; }
die() { printf 'ERROR: %s\n' "$*" >&2; exit 1; }

install_homebrew() {
    if command -v brew >/dev/null 2>&1; then
        return 0
    fi

    log "Homebrew not found. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

    if [[ -x /opt/homebrew/bin/brew ]]; then
        eval "$(/opt/homebrew/bin/brew shellenv)"
    elif [[ -x /usr/local/bin/brew ]]; then
        eval "$(/usr/local/bin/brew shellenv)"
    fi

    command -v brew >/dev/null 2>&1 || die "Homebrew install finished, but brew is still not available."
}

install_avrdude_macos() {
    install_homebrew

    log "Installing avrdude with Homebrew..."
    brew update
    brew install avrdude

    command -v avrdude >/dev/null 2>&1 || die "avrdude install finished, but avrdude is still not available."
}

install_avrdude_linux() {
    if command -v avrdude >/dev/null 2>&1; then
        return 0
    fi

    if command -v sudo >/dev/null 2>&1; then
        log "Installing avrdude with apt..."
        sudo apt-get update
        sudo apt-get install -y avrdude
    else
        die "avrdude is missing and sudo is not available. Install avrdude manually."
    fi

    command -v avrdude >/dev/null 2>&1 || die "avrdude install finished, but avrdude is still not available."
}

copy_script_bundle() {
    local source_dir="$1"
    local target_dir="${DEV_DIR}/scripts"

    [[ -d "${source_dir}" ]] || die "Script bundle not found: ${source_dir}"

    mkdir -p "${target_dir}"
    find "${target_dir}" -mindepth 1 -maxdepth 1 -exec rm -rf {} +
    cp -R "${source_dir}"/. "${target_dir}"/
    find "${target_dir}" -name '*.sh' -exec chmod +x {} +
}

setup_linux() {
    log "Detected Linux"
    install_avrdude_linux
    copy_script_bundle "${TOOLS_DIR}/Linux/scripts"
    log "Linux scripts copied into development/scripts"
}

setup_macos() {
    log "Detected macOS"
    install_avrdude_macos
    copy_script_bundle "${TOOLS_DIR}/macOS/scripts"
    log "macOS scripts copied into development/scripts"
    log "You can now flash from the host using avrdude and a /dev/cu.* port."
}

case "$(uname -s)" in
    Darwin)
        setup_macos
        ;;
    Linux)
        setup_linux
        ;;
    *)
        warn "Unsupported platform for this setup script"
        ;;
esac