param(
    [string]$Path = "."
)

function Is-IgnoredPath {
    param([string]$Path)
    $parts = $Path -split '[\\/]'
    foreach ($p in $parts) {
        if ($p -eq 'build') { return $true }
        if ($p.StartsWith('.')) { return $true }
    }
    return $false
}

function Is-FunctionSignature {
    param([string]$line)
    return $line -match '^\s*(?:[\w:<>~*&]+\s+)+\w+\s*\('
}

function Replace-BlankRange {
    param(
        [string[]]$lines,
        [int]$startIndex,
        [int]$existingCount,
        [int]$desiredCount
    )

    $before = if ($startIndex -gt 0) { $lines[0..($startIndex-1)] } else { @() }

    $afterIndex = $startIndex + $existingCount
    $after = if ($afterIndex -lt $lines.Count) { $lines[$afterIndex..($lines.Count-1)] } else { @() }

    $blanks = @()
    for ($i = 0; $i -lt $desiredCount; $i++) { $blanks += '' }

    return @($before + $blanks + $after)
}

# ---------------------------------------------------------
#   DETECT TRUE HEADER COMMENT BLOCKS (//// only)
# ---------------------------------------------------------
function Get-MergedCommentBlock {
    param([string[]]$lines, [int]$start)

    $i = $start
    $count = $lines.Count

    # Must start with a comment
    if (-not ($lines[$i].Trim().StartsWith('//'))) {
        return @{ isHeader = $false; end = $start }
    }

    # First pass: merge comment + blank + comment
    $blockLines = @()
    while ($i -lt $count) {
        $t = $lines[$i].Trim()

        if ($t -eq '') {
            $blockLines += $t
            $i++
            continue
        }

        if ($t.StartsWith('//')) {
            $blockLines += $t
            $i++
            continue
        }

        break
    }

    # Now determine if EVERY non-blank line starts with ////
    $nonBlank = $blockLines | Where-Object { $_ -ne '' }
    $allAreHeader = $true
    foreach ($l in $nonBlank) {
        if (-not $l.StartsWith('////')) {
            $allAreHeader = $false
            break
        }
    }

    return @{
        isHeader = $allAreHeader
        end      = $i
    }
}

# ---------------------------------------------------------
#   CPP SPACING
# ---------------------------------------------------------
function Fix-CppSpacing {
    param([string[]]$lines)

    $i = 0
    while ($i -lt $lines.Count) {
        $trim = $lines[$i].Trim()

        # --- 3 blank lines after last include block ---
        if ($trim -match '^#\s*include\b') {
            $lastInclude = $i
            $j = $i + 1
            while ($j -lt $lines.Count -and $lines[$j].Trim() -match '^#\s*include\b') {
                $lastInclude = $j
                $j++
            }

            $after = $lastInclude + 1
            $blankCount = 0
            while ($after + $blankCount -lt $lines.Count -and $lines[$after + $blankCount].Trim() -eq '') {
                $blankCount++
            }

            if ($blankCount -ne 3) {
                $lines = Replace-BlankRange $lines $after $blankCount 3
            }

            $i = $after + 3
            continue
        }

        # --- Header comment block (//// only) ---
        if ($trim.StartsWith('//')) {
            $block = Get-MergedCommentBlock $lines $i

            if ($block.isHeader) {
                $end = $block.end

                # Next non-blank line must be a function signature
                $j = $end
                while ($j -lt $lines.Count -and $lines[$j].Trim() -eq '') { $j++ }
                if ($j -lt $lines.Count -and (Is-FunctionSignature $lines[$j])) {

                    # Enforce 3 blank lines before header block
                    $prev = $i - 1
                    while ($prev -ge 0 -and $lines[$prev].Trim() -eq '') { $prev-- }

                    $firstBlank = $prev + 1
                    $blankCount = $i - $firstBlank

                    if ($blankCount -ne 3) {
                        $lines = Replace-BlankRange $lines $firstBlank $blankCount 3
                        $i = $firstBlank + 3
                        continue
                    }
                }
            }

            $i++
            continue
        }

        $i++
    }

    return $lines
}

# ---------------------------------------------------------
#   HEADER (.h) SPACING
# ---------------------------------------------------------
function Fix-HeaderSpacing {
    param([string[]]$lines)

    $i = 0
    while ($i -lt $lines.Count) {
        $line = $lines[$i]
        $trim = $line.Trim()

        # --- public/protected/private: no indent + spacing ---
        $m = [regex]::Match($line, '^\s*(public|protected|private)\s*:')
        if ($m.Success) {
            $kind = $m.Groups[1].Value

            $lines[$i] = ($lines[$i] -replace '^\s*', '')

            $prev = $i - 1
            while ($prev -ge 0 -and $lines[$prev].Trim() -eq '') { $prev-- }

            $firstBlank = $prev + 1
            $blankCount = $i - $firstBlank

            if ($kind -eq 'public') {
                if ($blankCount -gt 0) {
                    $lines = Replace-BlankRange $lines $firstBlank $blankCount 0
                    $i = $firstBlank
                    continue
                }
            } else {
                if ($blankCount -ne 2) {
                    $lines = Replace-BlankRange $lines $firstBlank $blankCount 2
                    $i = $firstBlank + 2
                    continue
                }
            }
        }

        # --- Inline method spacing (no header comment logic here) ---
        if ($trim -eq '}') {
            $j = $i + 1

            while ($j -lt $lines.Count) {
                $t = $lines[$j].Trim()
                if ($t -eq '' -or $t.StartsWith('//') -or $t.StartsWith('/*')) {
                    $j++
                    continue
                }
                break
            }

            if ($j -lt $lines.Count -and (Is-FunctionSignature $lines[$j])) {
                $firstBlank = $i + 1
                $blankCount = $j - $firstBlank

                if ($blankCount -ne 2) {
                    $lines = Replace-BlankRange $lines $firstBlank $blankCount 2
                    $i = $firstBlank + 2
                    continue
                }
            }
        }

        $i++
    }

    return $lines
}

# ---------------------------------------------------------
#   MAIN DRIVER
# ---------------------------------------------------------
Get-ChildItem -Path $Path -Recurse -Include *.cpp, *.h |
Where-Object { -not (Is-IgnoredPath $_.FullName) } |
ForEach-Object {

    $file = $_.FullName
    Write-Host "Post-format spacing $file"

    # Read original file EXACTLY as-is (preserves CRLF/LF)
    $original = Get-Content -LiteralPath $file -Raw

    # Read as lines for processing
    $lines = Get-Content -LiteralPath $file

    # Apply your transformations
    if ($file.ToLower().EndsWith(".cpp")) {
        $lines = Fix-CppSpacing $lines
    } elseif ($file.ToLower().EndsWith(".h")) {
        $lines = Fix-HeaderSpacing $lines
    }

    # Detect original line ending style
    $eol =
        if ($original -match "`r`n") { "`r`n" }
        elseif ($original -match "`n") { "`n" }
        else { "`r`n" }

    # Reassemble using original EOL
    $new = ($lines -join $eol)

    # Only write if content actually changed
    if ($new -ne $original) {
        Set-Content -LiteralPath $file -Value $new -NoNewline
    }
}
