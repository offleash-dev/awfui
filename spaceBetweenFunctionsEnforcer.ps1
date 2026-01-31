param(
    [string]$Path = ".",
    [int]$BlankLines = 3
)

# Build the blank-line block for .cpp files
$blankCpp = "`n" * $BlankLines

# Build the blank-line block for .h files (always 2)
$blankH = "`n" * 2

# -----------------------------
# PASS 1: .cpp files
# -----------------------------
Get-ChildItem -Path $Path -Recurse -Include *.cpp | ForEach-Object {
    $file = $_.FullName

    clang-format -i $file

    $content = Get-Content $file -Raw

	# Ensure exactly 3 blank lines after the last include
	$patternIncludes = "(?ms)(#include[^\n]*\n)(?!\n{3})"
	$content = [regex]::Replace($content, $patternIncludes, "`$1`n`n`n")

    # Insert N blank lines after function definitions
    $patternCpp = "(?m)\}\s*\n(?=\s*[A-Za-z_][A-Za-z0-9_:<>~\*\&\s]+\()"

    $newContent = [regex]::Replace($content, $patternCpp, "}`n$blankCpp")

    if ($newContent -ne $content) {
        Set-Content -Path $file -Value $newContent -NoNewline
        Write-Host "Updated spacing in $file (cpp)"
    }
}

# -----------------------------
# PASS 2: .h files
# -----------------------------
Get-ChildItem -Path $Path -Recurse -Include *.h | ForEach-Object {
    $file = $_.FullName

    clang-format -i $file

    $content = Get-Content $file -Raw

	# Ensure exactly 3 blank lines after the last include
	$patternIncludes = "(?ms)(#include[^\n]*\n)(?!\n{3})"
	$content = [regex]::Replace($content, $patternIncludes, "`$1`n`n`n")

    # 1. Two blank lines after inline method definitions
    $patternMethod = "(?m)\}\s*\n(?=\s*[A-Za-z_][A-Za-z0-9_:<>~\*\&\s]+\()"
    $content = [regex]::Replace($content, $patternMethod, "}`n$blankH")

    # 2. Two blank lines before access specifiers
    $patternAccess = "(?m)(?<!\n\n)(?=\s*(private|protected|public)\s*:)"
    $content = [regex]::Replace($content, $patternAccess, "`n`n")

    Set-Content -Path $file -Value $content -NoNewline
    Write-Host "Updated spacing in $file (header)"
}
