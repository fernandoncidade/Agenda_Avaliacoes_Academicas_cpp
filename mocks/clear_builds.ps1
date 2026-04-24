[CmdletBinding(SupportsShouldProcess = $true)]
param()

$ErrorActionPreference = 'Stop'

$workspaceRoot = 'C:\Users\ferna\DEV\CPP\Agenda_Avaliacoes_Academicas_cpp'
$resolvedWorkspaceRoot = [System.IO.Path]::GetFullPath(
    (Resolve-Path -LiteralPath $workspaceRoot).ProviderPath
).TrimEnd('\')

$buildDirectories = @(
    'build_AGENDA_msvc',
    'build_AGENDA_mingw',
    'mocks\VersionEditor_msvc',
    'mocks\VersionEditor_mingw',
    'mocks\CompileTranslations_msvc',
    'mocks\CompileTranslations_mingw'
)

foreach ($directoryName in $buildDirectories) {
    $expectedPath = [System.IO.Path]::GetFullPath(
        (Join-Path -Path $resolvedWorkspaceRoot -ChildPath $directoryName)
    ).TrimEnd('\')

    if (-not $expectedPath.StartsWith($resolvedWorkspaceRoot + '\', [System.StringComparison]::OrdinalIgnoreCase)) {
        Write-Warning "Ignorado fora do workspace: $expectedPath"
        continue
    }

    if ($expectedPath -eq $resolvedWorkspaceRoot) {
        Write-Warning "Ignorado caminho inseguro: $expectedPath"
        continue
    }

    if (-not (Test-Path -LiteralPath $expectedPath)) {
        Write-Host "Ignorado (nao existe): $expectedPath"
        continue
    }

    $resolvedPath = [System.IO.Path]::GetFullPath(
        (Resolve-Path -LiteralPath $expectedPath).ProviderPath
    ).TrimEnd('\')

    if ($resolvedPath -ne $expectedPath) {
        Write-Warning "Ignorado caminho inesperado: $resolvedPath"
        continue
    }

    if ($PSCmdlet.ShouldProcess($resolvedPath, 'Remover diretorio de build')) {
        Remove-Item -LiteralPath $resolvedPath -Recurse -Force
        Write-Host "Removido: $resolvedPath"
    }
}

Write-Host 'Limpeza concluida.'
