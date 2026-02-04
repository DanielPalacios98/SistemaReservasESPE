param(
    [string]$Root = "$PSScriptRoot"
)

$targets = @(
    (Join-Path $Root "lib"),
    (Join-Path $Root "gui"),
    (Join-Path $Root "tests")
)

$files = @()
foreach ($t in $targets) {
    if (Test-Path $t) {
        $files += Get-ChildItem -Path $t -Recurse -File -Include *.cpp,*.h
    }
}

$pattern = '\[|\]'
$hits = @()
foreach ($f in $files) {
    $m = Select-String -Path $f.FullName -Pattern $pattern -AllMatches
    if ($m) { $hits += $m }
}

if ($hits.Count -gt 0) {
    Write-Host "ERROR: Se encontraron corchetes '[' o ']' en el codigo propio:" -ForegroundColor Red
    $hits | Select-Object Path, LineNumber, Line | Format-Table -AutoSize | Out-String | Write-Host
    exit 1
}

Write-Host "OK: No hay '[' ni ']' en lib/gui/tests." -ForegroundColor Green
exit 0
