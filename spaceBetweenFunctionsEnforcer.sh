#!/usr/bin/env bash

PATH_ROOT="${1:-.}"

is_ignored_path() {
    case "$1" in
        */build/*) return 0 ;;
        */.*)      return 0 ;;
    esac
    return 1
}

is_function_signature() {
    [[ "$1" =~ ^[[:space:]]*([A-Za-z0-9_:<>~\*\&]+[[:space:]]+)+[A-Za-z_][A-Za-z0-9_]*[[:space:]]*\( ]]
}

replace_blank_range() {
    local file="$1"
    local start="$2"
    local existing="$3"
    local desired="$4"

    awk -v s="$start" -v e="$existing" -v d="$desired" '
        NR < s { print; next }
        NR >= s && NR < s+e { next }
        NR == s+e {
            for (i=0; i<d; i++) print ""
        }
        { print }
    ' "$file" > "$file.tmp" && mv "$file.tmp" "$file"
}

process_cpp() {
    local file="$1"
    local lines=()
    mapfile -t lines < "$file"

    local i=0
    while (( i < ${#lines[@]} )); do
        local t="${lines[$i]//[$'\t\r\n ']/}"

        # --- 3 blank lines after includes ---
        if [[ "${lines[$i]}" =~ ^[[:space:]]*#include ]]; then
            local last=$i
            local j=$((i+1))
            while (( j < ${#lines[@]} )) && [[ "${lines[$j]}" =~ ^[[:space:]]*#include ]]; do
                last=$j
                ((j++))
            done

            local after=$((last+1))
            local blank=0
            while (( after+blank < ${#lines[@]} )) && [[ -z "${lines[$((after+blank))]// }" ]]; do
                ((blank++))
            done

            if (( blank != 3 )); then
                replace_blank_range "$file" "$((after+1))" "$blank" 3
                mapfile -t lines < "$file"
            fi

            i=$((after+3))
            continue
        fi

        # --- header comment block (//// only) ---
        if [[ "${lines[$i]}" =~ ^[[:space:]]*// ]]; then
            local start=$i
            local block=()
            local k=$i

            while (( k < ${#lines[@]} )); do
                local trim="${lines[$k]//[$'\t\r\n ']/}"
                if [[ -z "$trim" || "${lines[$k]}" =~ ^[[:space:]]*// ]]; then
                    block+=("${lines[$k]}")
                    ((k++))
                else
                    break
                fi
            done

            local header=true
            for l in "${block[@]}"; do
                local t="${l#"${l%%[![:space:]]*}"}"
                if [[ -n "$t" && ! "$t" =~ ^//// ]]; then
                    header=false
                    break
                fi
            done

            if $header; then
                local next=$k
                while (( next < ${#lines[@]} )) && [[ -z "${lines[$next]// }" ]]; do
                    ((next++))
                done

                if (( next < ${#lines[@]} )) && is_function_signature "${lines[$next]}"; then
                    local prev=$((start-1))
                    while (( prev >= 0 )) && [[ -z "${lines[$prev]// }" ]]; do
                        ((prev--))
                    done

                    local first=$((prev+1))
                    local blank=$((start-first))

                    if (( blank != 3 )); then
                        replace_blank_range "$file" "$((first+1))" "$blank" 3
                        mapfile -t lines < "$file"
                    fi
                fi
            fi
        fi

        ((i++))
    done
}

process_header() {
    local file="$1"
    local lines=()
    mapfile -t lines < "$file"

    local i=0
    while (( i < ${#lines[@]} )); do
        local line="${lines[$i]}"
        local trim="${line#"${line%%[![:space:]]*}"}"

        # --- access specifiers ---
        if [[ "$trim" =~ ^(public|protected|private): ]]; then
            local kind="${BASH_REMATCH[1]}"

            # remove indent
            lines[$i]="${trim}"

            local prev=$((i-1))
            while (( prev >= 0 )) && [[ -z "${lines[$prev]// }" ]]; do
                ((prev--))
            done

            local first=$((prev+1))
            local blank=$((i-first))

            if [[ "$kind" == "public" ]]; then
                if (( blank > 0 )); then
                    replace_blank_range "$file" "$((first+1))" "$blank" 0
                    mapfile -t lines < "$file"
                fi
            else
                if (( blank != 2 )); then
                    replace_blank_range "$file" "$((first+1))" "$blank" 2
                    mapfile -t lines < "$file"
                fi
            fi
        fi

        ((i++))
    done
}

export -f is_ignored_path replace_blank_range process_cpp process_header is_function_signature

find "$PATH_ROOT" -type f \( -name "*.cpp" -o -name "*.h" \) | while read -r file; do
    if is_ignored_path "$file"; then
        continue
    fi

    echo "Post-format spacing $file"

    if [[ "$file" == *.cpp ]]; then
        process_cpp "$file"
    else
        process_header "$file"
    fi
done
