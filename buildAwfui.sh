f#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PRESET="${1:-sdl-ninja}"

cmake --preset "${PRESET}" -S "${SCRIPT_DIR}" ${CMAKE_ARGS:-}
cmake --build --preset "${PRESET}" || cmake --build "${SCRIPT_DIR}/build/${PRESET}"
