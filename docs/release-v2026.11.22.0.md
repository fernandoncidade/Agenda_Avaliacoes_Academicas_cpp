# Release Notes — AGENDA DE AVALIAÇÕES ACADÊMICAS v2026.11.22.0

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher notas de lançamento em português</summary><br>

# AGENDA DE AVALIAÇÕES ACADÊMICAS — Versão 2026.11.22.0

**Data de Lançamento:** 22 de novembro de 2026  
**Autor:** Fernando Nillsson Cidade (`linceu_lighthouse@outlook.com`)  
**Repositório:** `fernandoncidade/Agenda_Avaliacoes_Academicas_cpp`  
**Licenciamento:** GNU AGPLv3 (Código Aberto) / EULA Oficial + CLC (Microsoft Store)  

---

## Principais Entregas

1. **Modularização CMake & Substituição de `.clang-format`:**
   - Extração das rotinas compartilhadas de toolchain, traduções e implantação para [`cmake/AgendaToolchain.cmake`](../cmake/AgendaToolchain.cmake), [`cmake/AgendaTranslations.cmake`](../cmake/AgendaTranslations.cmake) e [`cmake/AgendaDeployment.cmake`](../cmake/AgendaDeployment.cmake), simplificando `CMakeLists.txt` e `mocks/CMakeLists.txt`.
   - Substituição do arquivo `.clang-format` na raiz pelo módulo [`cmake/AgendaFormatting.cmake`](../cmake/AgendaFormatting.cmake) (`Standard: Latest`, alvos opcionais `AgendaFormat` e `AgendaFormatCheck`).

2. **Orquestração CMake sem PowerShell & Unificação das Tarefas em `.vscode/tasks.json`:**
   - Migração de `.vscode/qt_task.ps1` e `.vscode/clear_builds.ps1` para [`cmake/AgendaTaskRunner.cmake`](../cmake/AgendaTaskRunner.cmake) e [`cmake/AgendaClearBuilds.cmake`](../cmake/AgendaClearBuilds.cmake).
   - Unificação das tarefas de `mocks/` em **`Qt6: Compilar e Executar MOCKS (MSVC)`** e **`Qt6: Compilar e Executar MOCKS (MinGW)`**, gerando builds unificados em `builds/build_MOCKS_msvc` e `builds/build_MOCKS_mingw`.
   - Limpeza do `.vscode/tasks.json` mantendo apenas as 6 tarefas principais visíveis, ocultando subtarefas (`"hide": true`) e removendo campos `"detail"`.

3. **Reorganização Estrutural (`library/` e `source/resources.rc`):**
   - Movimentação dos scripts Inno Setup (`Agenda_Avaliacoes_Academicas*.iss`), `Agenda_Avaliacoes_Academicas_cpp.txt` e `emojis.md` de `repo/` para `library/`.
   - Realocação de `resources.rc` para [`source/resources.rc`](../source/resources.rc) apontando para `assets/icones/ReviewsManager.ico`, mantendo compatibilidade integral com MSVC (`rc.exe`) e MinGW (`windres.exe`).

4. **Expansão do `VersionEditor` (32 Especificações) & Cobertura da Suíte (`tests/`):**
   - Ampliação de `mocks/find_VersionEditor.cpp` para auditar e sincronizar 32 especificações PT/EN (incluindo `library/`, documentos raiz e `docs/*.md`, com detecção/renomeação dinâmica de `docs/release-v*.md`).
   - Ampliação de `tests/test_09_MocksAndDeploymentIntegrity.cpp` validando todas as 32 especificações, os scripts CMake e a estrutura enxuta do `.vscode/tasks.json`, com 100% de aprovação nas 9 suítes em MSVC e MinGW.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse release notes in English</summary><br>

# ACADEMIC EVALUATION SCHEDULE — Version 2026.11.22.0

**Release Date:** November 22, 2026  
**Author:** Fernando Nillsson Cidade (`linceu_lighthouse@outlook.com`)  
**Repository:** `fernandoncidade/Agenda_Avaliacoes_Academicas_cpp`  
**Licensing:** GNU AGPLv3 (Open Source) / Official EULA + CLC (Microsoft Store)  

---

## Major Deliverables

1. **CMake Modularization & `.clang-format` Replacement:**
   - Extracted shared toolchain, translation, and deployment logic into [`cmake/AgendaToolchain.cmake`](../cmake/AgendaToolchain.cmake), [`cmake/AgendaTranslations.cmake`](../cmake/AgendaTranslations.cmake), and [`cmake/AgendaDeployment.cmake`](../cmake/AgendaDeployment.cmake), simplifying `CMakeLists.txt` and `mocks/CMakeLists.txt`.
   - Replaced root `.clang-format` with [`cmake/AgendaFormatting.cmake`](../cmake/AgendaFormatting.cmake) (`Standard: Latest`, opt-in `AgendaFormat` and `AgendaFormatCheck` targets).

2. **PowerShell-Free CMake Task Runner & Unified `.vscode/tasks.json`:**
   - Migrated `.vscode/qt_task.ps1` and `.vscode/clear_builds.ps1` into [`cmake/AgendaTaskRunner.cmake`](../cmake/AgendaTaskRunner.cmake) and [`cmake/AgendaClearBuilds.cmake`](../cmake/AgendaClearBuilds.cmake).
   - Unified `mocks/` workflows into **`Qt6: Compilar e Executar MOCKS (MSVC)`** and **`Qt6: Compilar e Executar MOCKS (MinGW)`**, building into `builds/build_MOCKS_msvc` and `builds/build_MOCKS_mingw`.
   - Streamlined `.vscode/tasks.json` to keep only the 6 primary tasks visible, hide subtasks (`"hide": true`), and remove `"detail"` entries.

3. **Structural Cleanup (`library/` & `source/resources.rc`):**
   - Moved Inno Setup packaging scripts (`Agenda_Avaliacoes_Academicas*.iss`), `Agenda_Avaliacoes_Academicas_cpp.txt`, and `emojis.md` from `repo/` into `library/`.
   - Relocated `resources.rc` to [`source/resources.rc`](../source/resources.rc) referencing `assets/icones/ReviewsManager.ico`, verified on both MSVC (`rc.exe`) and MinGW (`windres.exe`).

4. **Expanded `VersionEditor` (32 Specifications) & Test Suite Coverage (`tests/`):**
   - Expanded `mocks/find_VersionEditor.cpp` to audit and synchronize 32 PT/EN specifications (including `library/`, root docs, and `docs/*.md`, with dynamic `docs/release-v*.md` resolution and renaming).
   - Extended `tests/test_09_MocksAndDeploymentIntegrity.cpp` to verify all 32 specs, CMake runner scripts, and `.vscode/tasks.json` invariants, passing 9/9 suites on both MSVC and MinGW.

</details>
