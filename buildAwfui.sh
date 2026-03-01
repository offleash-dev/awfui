#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

CLEAN=""
TARGET=""
PRESET=""

for arg in "$@"; do
    case "$arg" in
        -a)  CLEAN="--clean-first" ;;
        -aw) CLEAN="--clean-first"; TARGET="--target awfui" ;;
        -as) CLEAN="--clean-first"; TARGET="--target awfui_sdl" ;;
        -ae) CLEAN="--clean-first"; TARGET="--target awfui_demo --target awfui_test --target awfui_justsdl --target awfui_keypad" ;;
        *)   [ -z "$PRESET" ] && PRESET="$arg" ;;
    esac
done

PRESET="${PRESET:-sdl-ninja}"

if [ -n "$CLEAN" ]; then
    echo "=== Rebuilding AWFUI (${PRESET}) ==="
else
    echo "=== Building AWFUI (${PRESET}) ==="
fi

cmake --preset "${PRESET}" -S "${SCRIPT_DIR}"
cmake --build --preset "${PRESET}" ${CLEAN} ${TARGET} || cmake --build "${SCRIPT_DIR}/build/${PRESET}" ${CLEAN} ${TARGET}
