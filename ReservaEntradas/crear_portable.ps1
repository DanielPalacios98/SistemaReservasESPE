$ErrorActionPreference = "Stop"
$ProjectRoot = $PSScriptRoot
$BuildDir = Join-Path $ProjectRoot "build"
$DistDir = Join-Path $ProjectRoot "Dist"
$ExePath = Join-Path $BuildDir "Release\ReservaEntradas.exe"
$DllSrcDir = Join-Path $BuildDir "vcpkg_installed\x64-windows\bin"

Write-Host "Iniciando creacion de portable..."
Write-Host "Directorio Proyecto: $ProjectRoot"

# 1. Clean/Create Dist
if (Test-Path $DistDir) {
    Write-Host "Limpiando directorio Dist existente..."
    Remove-Item $DistDir -Recurse -Force
}
New-Item -ItemType Directory -Path $DistDir | Out-Null
Write-Host "Directorio Dist creado."

# 2. Check Exe
if (-not (Test-Path $ExePath)) {
    Write-Warning "No se encontro el ejecutable en: $ExePath"
    Write-Warning "Intentando buscar en Debug..."
    $ExePath = Join-Path $BuildDir "Debug\ReservaEntradas.exe"
    if (-not (Test-Path $ExePath)) {
        Write-Error "No se encontro el ejecutable. Asegurese de compilar el proyecto (Release recomendado)."
    }
}

# 3. Copy Exe
Copy-Item $ExePath -Destination $DistDir
Write-Host "Ejecutable copiado: $(Split-Path $ExePath -Leaf)"

# 4. Copy DLLs
if (Test-Path $DllSrcDir) {
    $dlls = Get-ChildItem -Path $DllSrcDir -Filter "*.dll"
    foreach ($dll in $dlls) {
        Copy-Item $dll.FullName -Destination $DistDir
        Write-Host "DLL copiada: $($dll.Name)"
    }
} else {
    Write-Warning "No se encontro carpeta de DLLs en: $DllSrcDir."
    Write-Host "Guia rapida (Windows):"
    Write-Host "  1) Instale vcpkg: git clone https://github.com/microsoft/vcpkg.git C:\vcpkg; luego .\\bootstrap-vcpkg.bat"
    Write-Host "  2) Configure el proyecto:" 
    Write-Host "     cmake -S . -B build -DUSE_MONGO=ON -DCMAKE_TOOLCHAIN_FILE=\"C:/vcpkg/scripts/buildsystems/vcpkg.cmake\""
    Write-Host "  3) Compile en Release:" 
    Write-Host "     cmake --build build --config Release"
    Write-Host "Tras compilar, las DLLs apareceran en: $DllSrcDir"
}

# 5. Copy extra Resources if they exist
$Resources = @("reservas.json", "reservas.txt", "input.txt", ".env", "config.json")
foreach ($res in $Resources) {
    $resPath = Join-Path $ProjectRoot $res
    if (Test-Path $resPath) {
        Copy-Item $resPath -Destination $DistDir
        Write-Host "Recurso copiado: $res"
    }
}

# 6. Check for 'ssl' folder or similar if needed (sometimes CAs are needed)
# For mongo, sometimes we need nothing else.

Write-Host "============================"
Write-Host "PORTABLE CREADO EXITOSAMENT"
Write-Host "Ubicacion: $DistDir"
Write-Host "============================"
