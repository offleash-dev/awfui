#!/usr/bin/env bash

# Default blank lines for .cpp files (3)
BLANK_CPP=${1:-3}
BLANK_CPP_STR=""
for ((i=0; i<BLANK_CPP; i++)); do
    BLANK_CPP_STR="${BLANK_CPP_STR}\n"
done

# Always 2 blank lines for .h files
BLANK_H_STR="\n\n"

# ----------------------------------------
# PASS 1: .cpp files
# ----------------------------------------
find . -type f -name "*.cpp" | while read -r file; do
    # clang-format first
    clang-format -i "$file"

    # Perl transform: 3 blank lines after function definitions
    perl -0777 -i -pe "
        s/
            \}              # closing brace
            \s*\n           # optional whitespace + newline
            (?=             # lookahead: next line is a function signature
                \s*
                [A-Za-z_]
                [A-Za-z0-9_:<>~* &]+
                \(
            )
        /\}\n$BLANK_CPP_STR/xg;
    " "$file"

    # Ensure 3 blank lines after the last include block
    perl -0777 -i -pe '
        s/(#include[^\n]*\n)(?!\n{3})/$1\n\n\n/g;
    ' "$file"

    echo "Formatted (cpp): $file"
done

# ----------------------------------------
# PASS 2: .h files
# ----------------------------------------
find . -type f -name "*.h" | while read -r file; do
    clang-format -i "$file"

    # 1. Two blank lines after inline method definitions
    perl -0777 -i -pe "
        s/
            \}
            \s*\n
            (?=
                \s*
                [A-Za-z_]
                [A-Za-z0-9_:<>~* &]+
                \(
            )
        /\}\n$BLANK_H_STR/xg;
    " "$file"

    # 2. Two blank lines before access specifiers
    perl -0777 -i -pe '
        s/(?<!\n\n)(?=\s*(private|protected|public)\s*:)/\n\n/g;
    ' "$file"

    # 3. Three blank lines after include block
    perl -0777 -i -pe '
        s/(#include[^\n]*\n)(?!\n{3})/$1\n\n\n/g;
    ' "$file"

    echo "Formatted (header): $file"
done
