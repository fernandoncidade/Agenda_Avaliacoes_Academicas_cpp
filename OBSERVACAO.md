<!-- Multilanguage OBSERVACAO.md for Agenda_Avaliacoes_Academicas_cpp -->

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> |
  <a href="#enus">🇺🇸 English (US)</a> |
  <a href="#tree">🌳 Árvore de Arquivos / File Tree</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher a Observação Técnica em português</summary>

# AGENDA DE AVALIAÇÕES ACADÊMICAS — OBSERVAÇÃO TÉCNICA E POSICIONAMENTO ESTRATÉGICO

Versão documentada: `2026.11.22.0`  
Data do snapshot técnico: `22 de novembro de 2026`  
Repositório: `Agenda_Avaliacoes_Academicas_cpp`  
Autor identificado no projeto: `Fernando Nillsson Cidade`  
Licença do repositório: `GNU AGPLv3 (GNU Affero General Public License v3)`  
Licença da Edição Microsoft Store: `EULA Oficial do Autor + CLC`  

## 1. Estado Atual do Projeto e Posicionamento

A **AGENDA DE AVALIAÇÕES ACADÊMICAS** consolida uma aplicação desktop nativa para Windows, de caráter pessoal, educacional e acadêmico, desenvolvida em **C++23 moderno com Qt 6.12.0**, persistência local **SQLite** (`QSQLITE`), suporte bilíngue em tempo de execução (`pt_BR` / `en_US`) e exportação vetorial em PDF (`Qt6::PrintSupport` / `QPdfWriter`).

Sua proposta de valor resolve de forma direta uma dor recorrente de estudantes e docentes durante o período letivo:
> **“Quais avaliações acadêmicas estão agendadas para o semestre em ordem cronológica, com destaque visual imediato por disciplina, preservação de grades personalizadas e exportação limpa em PDF sem depender de nuvem?”**

O aplicativo opera sob arquitetura **100% Local-First e Offline** (zero nuvem, zero telemetria, zero rastreamento).

## 2. Destaques Técnicos da Consolidação C++23 / Qt 6.12.0 (`v2026.11.22.0`)

- **Modernização Integral de Plataforma & Dual Toolchain:** Elevação para **C++23** e **Qt 6.12.0** (`Core`, `Gui`, `Widgets`, `Sql`, `PrintSupport`, `LinguistTools`, `Test`) com suporte simultâneo a **MSVC x64** (Visual Studio 18 / `14.52.37025` / Windows SDK `10.0.28000.0`) e **MinGW-w64 x64** (GCC `15.1.0` em `Tools/mingw1510_64` relativo à instalação do Qt) via presets isolados em `CMakePresets.json` e resolução multi-configuração (`agenda_resolve_qt_tool_location`).
- **Distribuições Externas Ultra-Limpas & Instaladores Enxutos:** O alvo `DeployAgenda` gera pacotes enxutos (`29` arquivos / `30,84 MB` no MSVC e `27` arquivos / `37,62 MB` no MinGW) aplicando `--skip-plugin-types`, `--exclude-plugins`, 5 DLLs app-local do CRT MSVC (dispensando `vc_redist.x64.exe` nos scripts `library/*.iss` e reduzindo o instalador MSVC para `11,08 MB`, menor que o MinGW com `13,11 MB`) ou as 3 DLLs exatas do MinGW GCC `15.1.0` (prevenindo conflito com DLLs GCC 13 antigas no `bin` do Qt).
- **Arquitetura Modular com Biblioteca Compartilhada `AgendaCore`:** Todo o código de domínio, bancos SQLite, GUI, traduções e utilitários compila em `AgendaCore` (`STATIC`), consumida tanto pelo executável `AgendaAvaliacoesAcademicas.exe` quanto pelos alvos de teste em `tests/`.
- **Suíte Profissional de Engenharia (`tests/`):** **9 executáveis CTest/QtTest isolados** compilados em `builds/build_suite_msvc` e `builds/build_suite_mingw` via tarefas dedicadas no VS Code (`Qt6: Compilar e Executar SUITE (MSVC)` e `Qt6: Compilar e Executar SUITE (MinGW)`), sem geração de distribuição externa para testes.
- **Duplo Licenciamento & Documentação Completa:** Adoção da **GNU AGPLv3** em `LICENSE`, liberdade expressa para adaptação de `source/assets/` em forks, **EULA Oficial + CLC** para pacotes Store, documentação bilíngue completa em `README.md`, `MANUAL.md`, `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `SECURITY.md`, `.github/` e 9 documentos técnicos em `docs/` (sem referência a CLI, inaplicável a este projeto puramente GUI).
- **Consolidação Arquitetural CMake & Organização de Pastas (`v2026.11.22.0`):** Extração da lógica de build/formatação/orquestração para `cmake/` (`AgendaToolchain.cmake`, `AgendaTranslations.cmake`, `AgendaDeployment.cmake`, `AgendaFormatting.cmake` substituindo `.clang-format`, `AgendaTaskRunner.cmake` e `AgendaClearBuilds.cmake` substituindo `.vscode/qt_task.ps1` e `.vscode/clear_builds.ps1`), realocação de `resources.rc` para `source/resources.rc`, organização de scripts `.iss` e referências em `library/`, unificação de `mocks/` em `builds/build_MOCKS_{msvc,mingw}` e `.vscode/tasks.json` enxuto com exatamente 6 tarefas visíveis, subtarefas ocultas e sem `"detail"`.

## 3. Resumo Estrutural Consolidado do Código-Fonte

| Camada | Subdiretório | Descrição Técnica e Responsabilidade |
|---|---|---|
| **Persistência e Catálogos** | `source/BancoDeDados/` | `Banco_Dados` (idioma, mesclagem, cores por código/nome), `Listas_Personalizadas` (`listas.db` com 7 tabelas e exclusão hierárquica) e `DicionariosListasBancoDeDados/` (`CURSOS_PT`, `CURSOS_EN`, `dicionario_de_cores`, `dicionario_disciplinas`, listas e mapeamentos) |
| **Regras de Atividades** | `source/GerenciamentoAtividades/` | `GerenciamentoAtividades` e módulos `gativ_01`..`gativ_21` (`atividades.db` CRUD, ordenação cronológica `dd/MM/yyyy`, HTML escaping, modo `coloridas`/`preto`, `config.json`, PDF `QPdfWriter`, diálogos editar/excluir/detalhes, cascading updates) |
| **Interface Gráfica** | `source/GerenciamentoInterfaceGrafica/` | `InterfaceGerenciadorAtividades`, `SobreDialog` (`ginter_01`), `OpcoesSobre` (`ginter_02`), `exibir_sobre` (`ginter_03`), `create_widgets` (`ginter_06`), `create_menu` (`ginter_08`), `change_language` (`ginter_09`), `change_color_mode` (`ginter_10`), `retranslate_ui` (`ginter_11`), `excluir_item_lista_personalizada` (`ginter_12`) |
| **Internacionalização** | `source/language/` | `GerenciadorTraducao` (`pt_BR`/`en_US`, `language.json`, `QTranslator`) e catálogos `translations/manager_compression_{pt_BR,en_US}.{ts,qm}` |
| **Utilitários & Inicialização** | `source/utils/`, `source/` | `ApplicationPathUtils`, `CaminhoPersistenteUtils`, `ComboBoxUtils`, `IconUtils`, `LogManager` (`kMaxLogFiles = 10`), `TrialManager`, `src_01_InicializadorMain`, `resources.rc` |
| **Testes Automatizados** | `tests/` | **9 alvos CTest/QtTest** (`test_01_FrameworkAndPlugins`..`test_09_MocksAndDeploymentIntegrity`) + `TestSupport.hpp` com isolamento por `LOCALAPPDATA` temporário |
| **Ferramentas Auxiliares** | `mocks/` | `find_VersionEditor` (auditor e sincronizador de versões/datas em 32 especificações PT/EN incluindo `library/` e `docs/`) e `compileTranslations` (GUI para `.ts` -> `.qm`) compilados de forma unificada em `builds/build_MOCKS_{msvc,mingw}` via `mocks/CMakeLists.txt` |

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse Technical Observation in English</summary>

# ACADEMIC EVALUATION SCHEDULE — TECHNICAL OBSERVATION AND STRATEGIC POSITIONING

Documented Version: `2026.11.22.0`  
Technical Snapshot Date: `November 22, 2026`  
Repository: `Agenda_Avaliacoes_Academicas_cpp`  
Author Identified in Project: `Fernando Nillsson Cidade`  
Repository License: `GNU AGPLv3 (GNU Affero General Public License v3)`  
Microsoft Store Edition License: `Author's Official EULA + CLC`  

## 1. Current Project State and Positioning

**ACADEMIC EVALUATION SCHEDULE (AGENDA DE AVALIAÇÕES ACADÊMICAS)** delivers a native Windows desktop utility built in **modern C++23 with Qt 6.12.0**, local **SQLite** storage (`QSQLITE`), live bilingual localization (`pt_BR` / `en_US`), and vector PDF export (`Qt6::PrintSupport` / `QPdfWriter`). Operating **100% Local-First and Offline**, it organizes semester deadlines chronologically with subject color coding, cascading curriculum catalogs, and persistent user-defined lists.

## 2. Technical Consolidation Highlights (`v2026.11.22.0`)

- **Full C++23 & Qt 6.12.0 Dual-Toolchain Baseline:** Supports both **MSVC x64** (`14.52.37025` / Windows SDK `10.0.28000.0`) and **MinGW-w64 x64** (GCC `15.1.0` at `Tools/mingw1510_64` relative to the Qt installation) via self-contained presets in `CMakePresets.json`.
- **Ultra-Clean External Deployment & Lean Installers:** Filtered `windeployqt` deployment + 5 app-local MSVC CRT DLLs (no `vc_redist.x64.exe` inside Inno Setup installers -> `11.08 MB` MSVC vs. `13.11 MB` MinGW) and clean GCC `15.1.0` runtime DLL copying (`29` MSVC / `27` MinGW files).
- **Shared `AgendaCore` Architecture & 9-Suite Engineering Test Harness (`tests/`):** Isolated CTest/QtTest targets built under `builds/build_suite_msvc` and `builds/build_suite_mingw` via VS Code tasks (`Qt6: Compilar e Executar SUITE (MSVC)` and `Qt6: Compilar e Executar SUITE (MinGW)`), with zero external distribution for tests.
- **Dual Licensing & Complete Documentation:** GNU AGPLv3 (`LICENSE`) for public source code, explicit developer freedom for `source/assets/`, Official Store EULA + CLC, complete bilingual docs across root, `.github/`, `docs/` (omitting any CLI reference doc), and `source/assets/`.
- **CMake Modularization & Project Layout Consolidation (`v2026.11.22.0`):** Consolidated reusable modules and PowerShell-free task orchestration into `cmake/` (`AgendaToolchain.cmake`, `AgendaTranslations.cmake`, `AgendaDeployment.cmake`, `AgendaFormatting.cmake` replacing `.clang-format`, `AgendaTaskRunner.cmake`, and `AgendaClearBuilds.cmake`), relocated `resources.rc` into `source/resources.rc`, grouped installer scripts and references under `library/`, unified auxiliary builds in `builds/build_MOCKS_{msvc,mingw}`, and streamlined `.vscode/tasks.json` to 6 visible tasks (hidden subtasks, no `"detail"`).

</details>

---

## <a id="tree"></a>🌳 Árvore de Arquivos / File Tree

<details>
<summary>Clique para expandir / recolher a árvore estrutural do projeto</summary>

```text
Agenda_Avaliacoes_Academicas_cpp/
├── .gitattributes
├── .gitignore
├── CMakeLists.txt
├── CMakePresets.json
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
├── LICENSE                         # GNU AGPLv3
├── MANUAL.md
├── OBSERVACAO.md
├── README.md
├── SECURITY.md
├── main.cpp
├── main.hpp
├── .github/
│   ├── PULL_REQUEST_TEMPLATE.md
│   ├── ISSUE_TEMPLATE/
│   │   ├── bug_report.md
│   │   ├── config.yml
│   │   ├── feature_request.md
│   │   └── question.md
│   └── workflows/
│       └── update-readme-version.yml
├── .vscode/
│   ├── c_cpp_properties.json
│   ├── cmake-kits.json
│   ├── settings.json
│   └── tasks.json
├── cmake/                          # Módulos e scripts CMake reutilizáveis & estilo clang-format
│   ├── AgendaClearBuilds.cmake
│   ├── AgendaDeployment.cmake
│   ├── AgendaFormatting.cmake
│   ├── AgendaTaskRunner.cmake
│   ├── AgendaToolchain.cmake
│   └── AgendaTranslations.cmake
├── docs/                           # Documentação técnica (9 documentos bilíngues)
│   ├── architecture.md
│   ├── compliance.md
│   ├── licensing.md
│   ├── privacy-policy.md
│   ├── release-v2026.11.22.0.md
│   ├── security-model.md
│   ├── suggested-issues.md
│   ├── technical-audit-and-roadmap.md
│   └── user-guide.md
├── library/                        # Instaladores Inno Setup & referências auxiliares
│   ├── Agenda_Avaliacoes_Academicas.iss
│   ├── Agenda_Avaliacoes_Academicas_cpp.txt
│   ├── Agenda_Avaliacoes_Academicas_mingw.iss
│   ├── Agenda_Avaliacoes_Academicas_msvc.iss
│   └── emojis.md
├── mocks/                          # Ferramentas auxiliares independentes
│   ├── CMakeLists.txt
│   ├── clear_builds.ps1
│   ├── compileTranslations.{cpp,hpp}
│   └── find_VersionEditor.{cpp,hpp}
├── repo/                           # Prompts de engenharia do projeto
│   └── prompt_Agenda_Avaliacoes_Academicas_cpp.txt
├── source/
│   ├── resources.rc
│   ├── src_01_InicializadorMain.{cpp,hpp}
│   ├── BancoDeDados/
│   │   ├── Banco_Dados.{cpp,hpp}
│   │   ├── Listas_Personalizadas.{cpp,hpp}
│   │   └── DicionariosListasBancoDeDados/
│   ├── GerenciamentoAtividades/
│   │   ├── Gerenciador_Atividades.{cpp,hpp}
│   │   └── GerenciadorAtividades/gativ_01..21.*
│   ├── GerenciamentoInterfaceGrafica/
│   │   ├── Interface_Grafica.{cpp,hpp}
│   │   └── GerenciadorInterfaceGrafica/ginter_01..12.*
│   ├── assets/
│   │   ├── ABOUT/
│   │   ├── CLC/
│   │   ├── COPYRIGHT/
│   │   ├── EULA/
│   │   ├── LICENSES/
│   │   ├── MANUAL/
│   │   ├── NOTICES/
│   │   ├── PRIVACY_POLICY/
│   │   ├── RELEASE/
│   │   └── icones/
│   ├── language/
│   │   ├── tr_01_gerenciadorTraducao.{cpp,hpp}
│   │   └── translations/manager_compression_{en_US,pt_BR}.{ts,qm}
│   └── utils/
│       ├── ApplicationPathUtils.{cpp,hpp}
│       ├── CaminhoPersistenteUtils.{cpp,hpp}
│       ├── ComboBoxUtils.{cpp,hpp}
│       ├── IconUtils.{cpp,hpp}
│       ├── LogManager.{cpp,hpp}
│       └── TrialManager.{cpp,hpp}
└── tests/                          # Suíte modular CTest / QtTest (builds/build_suite_{msvc,mingw})
    ├── CMakeLists.txt
    ├── TestSupport.hpp
    ├── test_01_FrameworkAndPlugins.cpp
    ├── test_02_UtilsAndPersistence.cpp
    ├── test_03_DictionariesAndMappings.cpp
    ├── test_04_CustomListsDatabase.cpp
    ├── test_05_BancoDadosIntegration.cpp
    ├── test_06_ActivityManagerCRUD.cpp
    ├── test_07_TranslationsAndAssets.cpp
    ├── test_08_GuiMainWindowAndWorkflows.cpp
    └── test_09_MocksAndDeploymentIntegrity.cpp
```

</details>
