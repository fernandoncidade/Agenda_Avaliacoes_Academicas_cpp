[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet('msvc', 'mingw')]
    [string]$Toolchain,

    [Parameter(Mandatory = $true)]
    [ValidateSet('BuildAgenda', 'RunAgenda', 'VersionEditor', 'CompileTranslations')]
    [string]$Action
)

$ErrorActionPreference = 'Stop'

$workspaceRoot = (Resolve-Path -LiteralPath (Join-Path -Path $PSScriptRoot -ChildPath '..')).ProviderPath

function Invoke-CheckedCommand {
    param(
        [Parameter(Mandatory = $true)]
        [string]$FilePath,

        [Parameter(ValueFromRemainingArguments = $true)]
        [string[]]$Arguments
    )

    & $FilePath @Arguments
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

function Get-VcVars64Path {
    $vswhere = Join-Path -Path ([Environment]::GetFolderPath('ProgramFilesX86')) -ChildPath 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswhere)) {
        $vswhere = Join-Path -Path $env:ProgramFiles -ChildPath 'Microsoft Visual Studio\Installer\vswhere.exe'
    }
    if (-not (Test-Path -LiteralPath $vswhere)) {
        throw 'vswhere.exe nao encontrado.'
    }

    $vsPath = & $vswhere -version '[17.0,18.0)' -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $vsPath) {
        $vsPath = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    }
    if (-not $vsPath) {
        throw 'Visual Studio/Build Tools com C++ nao encontrado.'
    }

    $vcvars = Join-Path -Path $vsPath -ChildPath 'VC\Auxiliary\Build\vcvars64.bat'
    if (-not (Test-Path -LiteralPath $vcvars)) {
        throw "vcvars64.bat nao encontrado: $vcvars"
    }

    return $vcvars
}

function Invoke-CMakeForToolchain {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Preset,

        [Parameter(Mandatory = $true)]
        [string]$Target
    )

    if ($Toolchain -eq 'msvc') {
        $env:VSLANG = '1033'
        $env:PATH = 'C:\Qt\6.11.0\msvc2022_64\bin;C:\Qt\Tools\Ninja;' + $env:PATH
        $vcvars = Get-VcVars64Path
        $cmd = 'call "' + $vcvars + '" && cmake --preset ' + $Preset + ' && cmake --build --preset ' + $Preset + ' --target ' + $Target
        & cmd.exe /d /c $cmd
        if ($LASTEXITCODE -ne 0) {
            exit $LASTEXITCODE
        }
        return
    }

    $env:PATH = 'C:\Qt\6.11.0\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;' + $env:PATH
    Invoke-CheckedCommand cmake --preset $Preset
    Invoke-CheckedCommand cmake --build --preset $Preset --target $Target
}

function Invoke-VersionEditorForToolchain {
    $versionEditorSourceDir = Join-Path -Path $workspaceRoot -ChildPath 'mocks'
    $versionEditorBuildDir = Join-Path -Path $workspaceRoot -ChildPath $versionEditorDir

    if ($Toolchain -eq 'msvc') {
        $env:VSLANG = '1033'
        $env:PATH = 'C:\Qt\6.11.0\msvc2022_64\bin;C:\Qt\Tools\Ninja;' + $env:PATH
        $vcvars = Get-VcVars64Path
        $cmd = 'call "' + $vcvars + '" && cmake -S "' + $versionEditorSourceDir + '" -B "' + $versionEditorBuildDir + '" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_PREFIX_PATH=C:/Qt/6.11.0/msvc2022_64 -DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/Ninja/ninja.exe && cmake --build "' + $versionEditorBuildDir + '" --target VersionEditor'
        & cmd.exe /d /c $cmd
        if ($LASTEXITCODE -ne 0) {
            exit $LASTEXITCODE
        }
        return
    }

    $env:PATH = 'C:\Qt\6.11.0\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;' + $env:PATH
    $configureArgs = @(
        '-S', $versionEditorSourceDir,
        '-B', $versionEditorBuildDir,
        '-G', 'Ninja',
        '-DCMAKE_BUILD_TYPE=Release',
        '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON',
        '-DCMAKE_PREFIX_PATH=C:/Qt/6.11.0/mingw_64',
        '-DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1310_64/bin/g++.exe',
        '-DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/Ninja/ninja.exe'
    )
    $buildArgs = @('--build', $versionEditorBuildDir, '--target', 'VersionEditor')
    Invoke-CheckedCommand cmake @configureArgs
    Invoke-CheckedCommand cmake @buildArgs
}

function Invoke-CompileTranslationsForToolchain {
    $compileTranslationsSourceDir = Join-Path -Path $workspaceRoot -ChildPath 'mocks'
    $compileTranslationsBuildDir = Join-Path -Path $workspaceRoot -ChildPath $compileTranslationsDir

    if ($Toolchain -eq 'msvc') {
        $env:VSLANG = '1033'
        $env:PATH = 'C:\Qt\6.11.0\msvc2022_64\bin;C:\Qt\Tools\Ninja;' + $env:PATH
        $vcvars = Get-VcVars64Path
        $cmd = 'call "' + $vcvars + '" && cmake -S "' + $compileTranslationsSourceDir + '" -B "' + $compileTranslationsBuildDir + '" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_PREFIX_PATH=C:/Qt/6.11.0/msvc2022_64 -DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/Ninja/ninja.exe && cmake --build "' + $compileTranslationsBuildDir + '" --target CompileTranslations'
        & cmd.exe /d /c $cmd
        if ($LASTEXITCODE -ne 0) {
            exit $LASTEXITCODE
        }
        return
    }

    $env:PATH = 'C:\Qt\6.11.0\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;' + $env:PATH
    $configureArgs = @(
        '-S', $compileTranslationsSourceDir,
        '-B', $compileTranslationsBuildDir,
        '-G', 'Ninja',
        '-DCMAKE_BUILD_TYPE=Release',
        '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON',
        '-DCMAKE_PREFIX_PATH=C:/Qt/6.11.0/mingw_64',
        '-DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1310_64/bin/g++.exe',
        '-DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/Ninja/ninja.exe'
    )
    $buildArgs = @('--build', $compileTranslationsBuildDir, '--target', 'CompileTranslations')
    Invoke-CheckedCommand cmake @configureArgs
    Invoke-CheckedCommand cmake @buildArgs
}

function Start-Executable {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ExecutablePath,

        [string]$MissingHint = 'Rode a tarefa de compilacao primeiro.'
    )

    if (-not (Test-Path -LiteralPath $ExecutablePath)) {
        throw "Executavel nao encontrado: $ExecutablePath. $MissingHint"
    }

    Start-Process -FilePath $ExecutablePath -WorkingDirectory (Split-Path -Path $ExecutablePath)
}

Set-Location -LiteralPath $workspaceRoot

$preset = if ($Toolchain -eq 'msvc') { 'qt-msvc' } else { 'qt-mingw' }
$versionEditorDir = if ($Toolchain -eq 'msvc') { 'mocks\VersionEditor_msvc' } else { 'mocks\VersionEditor_mingw' }
$compileTranslationsDir = if ($Toolchain -eq 'msvc') { 'mocks\CompileTranslations_msvc' } else { 'mocks\CompileTranslations_mingw' }
$deployDir = if ($Toolchain -eq 'msvc') {
    'D:\MISCELANEAS\VSCode\AGENDA\AgendaAvaliacoesAcademicas_msvc'
} else {
    'D:\MISCELANEAS\VSCode\AGENDA\AgendaAvaliacoesAcademicas_mingw'
}
$deployExe = Join-Path -Path $deployDir -ChildPath 'AgendaAvaliacoesAcademicas.exe'

switch ($Action) {
    'BuildAgenda' {
        Invoke-CMakeForToolchain -Preset $preset -Target 'DeployAgenda'
    }
    'RunAgenda' {
        Start-Executable -ExecutablePath $deployExe
    }
    'VersionEditor' {
        Invoke-VersionEditorForToolchain
        $versionEditorExe = Join-Path -Path $workspaceRoot -ChildPath (Join-Path -Path $versionEditorDir -ChildPath 'VersionEditor.exe')
        Start-Executable -ExecutablePath $versionEditorExe
    }
    'CompileTranslations' {
        Invoke-CompileTranslationsForToolchain
        $compileTranslationsExe = Join-Path -Path $workspaceRoot -ChildPath (Join-Path -Path $compileTranslationsDir -ChildPath 'CompileTranslations.exe')
        Start-Executable -ExecutablePath $compileTranslationsExe
    }
}
