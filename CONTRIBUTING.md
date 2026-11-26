# Guia de Contribuição / Contributing Guide — AGENDA DE AVALIAÇÕES ACADÊMICAS

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher as instruções em português</summary><br>

# Contribuindo com a AGENDA DE AVALIAÇÕES ACADÊMICAS

**Versão:** `2026.11.22.0` | **Data:** `22 de novembro de 2026` | **Autor:** Fernando Nillsson Cidade

Agradecemos seu interesse em contribuir com a **AGENDA DE AVALIAÇÕES ACADÊMICAS**!

> [!NOTE]
> **Natureza Voluntária e Duplo Licenciamento:** Este repositório é mantido pelo autor (`Fernando Nillsson Cidade`) em caráter pessoal e educacional em C++23 e Qt 6.12.0. Sugestões, Issues e Pull Requests são bem-vindos e analisados conforme disponibilidade de tempo. Nos termos da **[GNU AGPLv3](LICENSE)**, você também possui total liberdade para manter seu próprio *fork*; ademais, os documentos institucionais em [`source/assets/`](docs/licensing.md#ptbr) têm autorização expressa para alteração e adaptação em forks e distribuições próprias.

---

## 1. Código de Conduta

Ao participar deste projeto, concorda-se com o [Código de Conduta](CODE_OF_CONDUCT.md#ptbr).

---

## 2. Formas de Contribuir

- **Relatar Bugs:** Inconsistências de persistência SQLite (`atividades.db` / `listas.db`), ordenação cronológica, renderização PDF, traduções `.ts`/`.qm` ou comportamento de combos em cascata.
- **Novos Catálogos Curriculares:** Expansão de cursos, ementas, semestres e paletas de cores em `source/BancoDeDados/DicionariosListasBancoDeDados/`.
- **Suíte de Testes (`tests/`):** Ampliação de casos de teste automatizados em QtTest (`test_01`..`test_09`).
- **Documentação e Traduções (`pt_BR` / `en_US`):** Melhoria dos documentos bilíngues em `docs/` e manutenção via `CompileTranslations` e `VersionEditor`.

---

## 3. Ambiente de Desenvolvimento (Windows x64)

### 3.1. Pré-requisitos
- **SO:** Windows 11 / Windows 10 x64
- **Compiladores C++23:** **MSVC x64** (Visual Studio 18 / toolset `14.52.37025` / Windows SDK `10.0.28000.0`) ou **MinGW-w64 x64** (`Tools/mingw1510_64/bin` relativo à instalação do Qt — GCC `15.1.0`)
- **Qt 6.12.0:** Módulos `Core`, `Gui`, `Widgets`, `Sql`, `PrintSupport`, `LinguistTools` e `Test` (`msvc2022_64` e/ou `mingw_64`)
- **Ferramentas:** **CMake 3.28+** e **Ninja**

### 3.2. Compilação e Execução da Suíte de Testes via Presets CMake / Script

```powershell
# Clonar o repositório
git clone https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp.git
cd Agenda_Avaliacoes_Academicas_cpp

# Compilar o aplicativo principal e gerar distribuição externa limpa
cmake -DTOOLCHAIN=msvc -DACTION=BuildAgenda -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=BuildAgenda -P cmake/AgendaTaskRunner.cmake

# Compilar e executar os utilitários unificados em builds/build_MOCKS_{msvc,mingw}
cmake -DTOOLCHAIN=msvc -DACTION=RunMocks -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=RunMocks -P cmake/AgendaTaskRunner.cmake

# Compilar e executar toda a suíte CTest/QtTest (builds/build_suite_msvc e builds/build_suite_mingw, SEM distribuição externa)
cmake -DTOOLCHAIN=msvc -DACTION=RunSuite -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=RunSuite -P cmake/AgendaTaskRunner.cmake

# Limpar diretórios de build via CMake
cmake -P cmake/AgendaClearBuilds.cmake
```

---

## 4. Diretrizes de Código e Qualidade

1. **Padrão C++23 e RAII:** Evite ponteiros brutos proprietários (`new`/`delete` sem árvore `QObject` ou ponteiro inteligente). Feche conexões `QSqlDatabase` nomeadas determinísticamente antes de `QSqlDatabase::removeDatabase`.
2. **Separação Modular:** Mantenha regras de banco em `source/BancoDeDados/`, lógica de atividades em `source/GerenciamentoAtividades/`, camada gráfica em `source/GerenciamentoInterfaceGrafica/`, internacionalização em `source/language/` e utilitários transversais em `source/utils/`.
3. **Arquitetura 100% Offline & Sanitização:** Qualquer texto exibido em `QTextBrowser` ou exportado em PDF deve usar `toHtmlEscaped()`, e queries SQLite devem usar `prepare()` + `addBindValue()`. Nunca adicione telemetria nem chamadas de rede.
4. **Formatação, Finais de Linha e Versionamento:** Respeite `.gitattributes` (LF para C++/`.md`/`.txt`; CRLF para `.ps1`/`.bat`) e o estilo C++23 definido em `cmake/AgendaFormatting.cmake` (alvos opcionais `AgendaFormat` e `AgendaFormatCheck`). Ao atualizar versão ou textos do projeto, valide todas as 32 especificações em `VersionEditor` e mantenha os 9 testes em `tests/` 100% aprovados em MSVC e MinGW.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse instructions in English</summary><br>

# Contributing to ACADEMIC EVALUATION SCHEDULE

**Version:** `2026.11.22.0` | **Date:** `November 22, 2026` | **Author:** Fernando Nillsson Cidade

Thank you for your interest in contributing to **ACADEMIC EVALUATION SCHEDULE (AGENDA DE AVALIAÇÕES ACADÊMICAS)**!

> [!NOTE]
> **Voluntary Maintenance & Dual Licensing:** Maintained by `Fernando Nillsson Cidade` as a personal and educational C++23 / Qt 6.12.0 initiative. Issues and Pull Requests are welcome and reviewed based on availability. Under **[GNU AGPLv3](LICENSE)** you are free to maintain your own fork, and institutional files in [`source/assets/`](docs/licensing.md#enus) carry explicit permission for developers to alter or adapt them in derivative distributions.

---

## 1. Code of Conduct

By participating, you agree to follow our [Code of Conduct](CODE_OF_CONDUCT.md#enus).

---

## 2. Development Setup (Windows x64)

- **C++23 Toolchains:** **MSVC x64** (`14.52.37025` / Windows SDK `10.0.28000.0`) or **MinGW-w64 x64** (`Tools/mingw1510_64/bin` relative to the Qt installation, GCC `15.1.0`)
- **Qt 6.12.0:** `Core`, `Gui`, `Widgets`, `Sql`, `PrintSupport`, `LinguistTools`, and `Test`
- **Build System:** **CMake 3.28+** and **Ninja**

```powershell
# Build app & clean external distribution
cmake -DTOOLCHAIN=msvc -DACTION=BuildAgenda -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=BuildAgenda -P cmake/AgendaTaskRunner.cmake

# Build & launch unified auxiliary tools under builds/build_MOCKS_{msvc,mingw}
cmake -DTOOLCHAIN=msvc -DACTION=RunMocks -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=RunMocks -P cmake/AgendaTaskRunner.cmake

# Build and execute the full CTest/QtTest suite (builds/build_suite_msvc & builds/build_suite_mingw, NO external deployment)
cmake -DTOOLCHAIN=msvc -DACTION=RunSuite -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=RunSuite -P cmake/AgendaTaskRunner.cmake

# Clean build directories via CMake
cmake -P cmake/AgendaClearBuilds.cmake
```

---

## 3. Coding Standards & Submission Checklist

1. Keep modules cleanly separated across `BancoDeDados`, `GerenciamentoAtividades`, `GerenciamentoInterfaceGrafica`, `language`, and `utils`.
2. Enforce parameterized `QSqlQuery` bindings (`prepare()` / `addBindValue()`), `toHtmlEscaped()` on HTML/PDF output, and deterministic `QSqlDatabase` cleanup.
3. Keep the application strictly **100% offline / local-first** with zero telemetry.
4. Follow the formatting profile in `cmake/AgendaFormatting.cmake` (`AgendaFormat` / `AgendaFormatCheck`), preserve `mocks/find_VersionEditor.cpp` compatibility (all 32 specifications), and ensure all 9 CTest suites pass on both MSVC and MinGW before opening a Pull Request.

</details>
