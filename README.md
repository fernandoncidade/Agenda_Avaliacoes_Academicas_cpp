<!-- Multilanguage README.md for Agenda_Avaliacoes_Academicas_cpp -->

# Agenda de Avaliações Acadêmicas / Academic Evaluation Schedule

**Utilitário desktop nativo para Windows em C++23 moderno e Qt 6.12.0 destinado ao cadastro, organização cronológica automática, filtragem por grades curriculares em cascata, destaque visual por cores e exportação em PDF de atividades avaliativas acadêmicas (`pt-BR` / `en-US`), com persistência 100% local e offline em SQLite (`atividades.db` e `listas.db`).**

[![Latest Release](https://img.shields.io/badge/Release-v2026.11.22.0-blue.svg)](https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp/releases/tag/v2026.11.22.0)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=c%2B%2B)](CMakeLists.txt)
[![Qt 6.12.0](https://img.shields.io/badge/Qt-6.12.0-41CD52?logo=qt)](CMakeLists.txt)
[![SQLite](https://img.shields.io/badge/Storage-SQLite_Local-003B57?logo=sqlite)](docs/architecture.md)
[![Platform Windows](https://img.shields.io/badge/Platform-Windows_11_%2F_10-0078D4?logo=windows)](README.md)
[![License GNU AGPLv3](https://img.shields.io/badge/License-GNU_AGPLv3-blue.svg)](LICENSE)
[![Official Store EULA](https://img.shields.io/badge/License-Official_Store_EULA_%2B_CLC-purple.svg)](docs/licensing.md)
[![100% Offline](https://img.shields.io/badge/Security-100%25_Offline-success.svg)](docs/security-model.md)

<p align="center">
  <a href="docs/user-guide.md">📖 Guia do Usuário / User Guide</a> •
  <a href="docs/architecture.md">🏗️ Arquitetura / Architecture</a> •
  <a href="docs/compliance.md">📋 Conformidade / Compliance</a> •
  <a href="docs/security-model.md">🛡️ Privacidade &amp; Segurança</a> •
  <a href="docs/licensing.md">⚖️ Duplo Licenciamento / Dual Licensing</a><br>
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> •
  <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

> **Observação:** Este repositório contém o código-fonte aberto e a documentação de conformidade da **AGENDA DE AVALIAÇÕES ACADÊMICAS** (implementação nativa em **C++23** e **Qt 6.12.0** com persistência local em **SQLite**), disponibilizada sob arquitetura transparente de **Duplo Licenciamento** (**GNU AGPLv3** para o código-fonte público no GitHub e **EULA Oficial + CLC** para o pacote binário assinado distribuído oficialmente na Microsoft Store, com execução 100% local e offline).

<details>
<summary><b>Clique para expandir / recolher o README em Português (BR)</b></summary><br>

# AGENDA DE AVALIAÇÕES ACADÊMICAS

Versão: v2026.11.22.0
Autor: Fernando Nillsson Cidade

A **AGENDA DE AVALIAÇÕES ACADÊMICAS** (Versão `2026.11.22.0`) é uma aplicação desktop nativa para Windows, desenvolvida em **C++23 moderno** e **Qt 6.12.0**, criada para estudantes, docentes e coordenações consolidarem em um único painel cronológico todas as avaliações do semestre (provas, testes, trabalhos, relatórios, seminários, apresentações e projetos), evitando dispersão em planilhas, recados e agendas paralelas.

```text
Fluxo de Dados Local-First (100% Offline)
 ├── Seleção em Cascata / Texto Livre: Curso -> Ementa -> Semestre -> Disciplina + Turma + Tipo + Sequência + Data
 ├── Persistência Dupla SQLite (%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\):
 │    ├── atividades.db  (atividades: id, data, tipo, sequencia, nome, turma)
 │    └── listas.db      (7 tabelas personalizadas por idioma pt_BR / en_US com unicidade case-insensitive)
 ├── Preferências & Logs: config.json ("coloridas"|"preto"), language.json ("pt_BR"|"en_US"), logs/ (máx. 10 arquivos)
 └── Apresentação & Exportação: QTextBrowser cronológico (com HTML escaping) + QPdfWriter / Qt PrintSupport (.pdf)
```

---

### ⚖️ Manifesto e Autorização Legal do Titular dos Direitos Autorais

> **Declaração Expressa de Fernando Nillsson Cidade:**  
> Eu, **Fernando Nillsson Cidade**, na qualidade de único autor, criador original e titular exclusivo dos direitos autorais do projeto **AGENDA DE AVALIAÇÕES ACADÊMICAS**, **autorizo o uso, estudo, cópia, modificação, compilação e distribuição do código-fonte público deste repositório** nos exatos termos do arquivo **[LICENSE](LICENSE)** (**GNU Affero General Public License v3 — GNU AGPLv3**, identificador SPDX `AGPL-3.0-only`), ressalvada a proteção nominal de nome e marca sob a Seção 7(e) (`"AGENDA DE AVALIAÇÕES ACADÊMICAS"`).  
> Qualquer versão modificada, extensão ou obra derivada do código-fonte público deve preservar a atribuição da autoria original e disponibilizar o código-fonte correspondente sob a mesma licença **GNU AGPLv3**.  
> **Liberdade para Documentação Institucional em `source/assets/`:** Os textos de conformidade contidos em [`source/assets/`](source/assets/COPYRIGHT/AVISO%20DE%20COPYRIGHT%20E%20MARCA%20REGISTRA_pt_BR.txt) (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `MANUAL`, `NOTICES`, `PRIVACY_POLICY` e `RELEASE`) foram elaborados para viabilizar a distribuição oficial na Microsoft Store e em instaladores Windows; o autor **autoriza expressamente** que desenvolvedores e forks alterem, excluam, adicionem ou adaptem tais documentos em suas próprias distribuições, observada a legislação de suas jurisdições e diretrizes de cada plataforma.  
> **Limite do Binário Oficial Assinado (Microsoft Store):** A edição empacotada e assinada digitalmente pelo autor para a Microsoft Store é regida pelo respectivo **[EULA Oficial](source/assets/EULA/EULA_pt_BR.txt)** (em conformidade com os Termos de Licença Padrão para Aplicativos da Microsoft Store) e **[Esclarecimento Comercial — CLC](source/assets/CLC/CLC_pt_BR.txt)**. É expressamente vedada a revenda comercial paga sem autorização, a apropriação indevida da marca ou a republicação não autorizada por terceiros do pacote binário oficial assinado pelo autor.  
> **Canal de Suporte, Independência da Microsoft e Isenção Legal:** A **Microsoft NÃO presta suporte ao cliente nem cobre este software sob a Política de Privacidade da Microsoft**. Todo eventual suporte técnico do autor é prestado **exclusivamente por e-mail (`linceu_lighthouse@outlook.com`) e pelas abas Issues/Discussions do GitHub**, **em regime voluntário de melhores esforços (best-effort), conforme disponibilidade e sem obrigação de urgência, prazo ou SLA**. Como ferramenta auxiliar gratuita de organização acadêmica, é fornecida **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sem garantias, devendo o usuário conferir sempre datas e ementas oficiais junto à sua instituição de ensino. Consulte [`docs/licensing.md`](docs/licensing.md#ptbr).

---

### 📌 Ficha Técnica do Projeto

- **Nome Oficial:** AGENDA DE AVALIAÇÕES ACADÊMICAS (`Academic Evaluation Schedule`)
- **Autor e Mantenedor:** Fernando Nillsson Cidade *(Desenvolvedor Individual / Pessoa Física)*
- **E-mail Oficial de Suporte (voluntário / best-effort, sem SLA):** `linceu_lighthouse@outlook.com`
- **Repositório Oficial:** [https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp](https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp)
- **Idiomas Suportados em Tempo de Execução:** Português (Brasil — `pt_BR`) · English (United States — `en_US`)
- **Requisitos de Sistema (Execução Binária):** Windows 11 / Windows 10 x64 (sem necessidade de instalar compilador, SDK do Qt ou `vc_redist.x64.exe` nas distribuições limpas app-local)
- **Alvos, Módulos e Presets CMake (`CMakeLists.txt`, `cmake/*.cmake` e `CMakePresets.json`):**
  - `AgendaCore` (biblioteca estática compartilhada pelo aplicativo principal e pela suíte de testes) + `AgendaAvaliacoesAcademicas.exe` com recurso embutido em `source/resources.rc` (`qt-msvc` -> `builds/build_AGENDA_msvc/` e `qt-mingw` -> `builds/build_AGENDA_mingw/`)
  - `DeployAgenda`: gera distribuições externas ultra-limpas em `../AgendaAvaliacoesAcademicas_{msvc,mingw}` (configurável via `AGENDA_DEPLOY_DIR`, somente runtime Qt mínimo, plugins essenciais, 5 DLLs app-local do CRT MSVC ou 3 DLLs do MinGW GCC `15.1.0`, 13 assets de runtime e 2 catálogos `.qm`)
  - **Instaladores Inno Setup (`library/Agenda_Avaliacoes_Academicas*.iss`):** empacotamento direto das distribuições limpas sem embutir `vc_redist.x64.exe`
  - `VersionEditor.exe` e `CompileTranslations.exe` (`mocks/` -> `builds/build_MOCKS_{msvc,mingw}/`): ferramentas GUI independentes de auditoria/sincronização de versões (32 especificações PT/EN) e compilação `.ts` -> `.qm`
  - **Suíte Modular CTest / QtTest (`suite-msvc` -> `builds/build_suite_msvc` / `suite-mingw` -> `builds/build_suite_mingw`):** **9 suítes independentes** em `tests/` (sem geração de distribuição externa para testes)

---

### 🌟 Principais Recursos Implementados

1. **Cadastro em Cascata & Listas Personalizadas Persistentes (`BancoDeDados`):**
   - Preenchimento hierárquico de **Curso -> Ementa -> Semestre -> Disciplina** mais **Turma**, **Tipo de Atividade Avaliativa**, **Sequência** e **Data** (`QCalendarWidget`), com catálogos completos em `pt_BR` e `en_US` (`CURSOS_PT`, `CURSOS_EN`);
   - Todos os campos aceitam tanto seleção das listas oficiais quanto entrada digitada livremente, persistida por idioma em 7 tabelas SQLite dedicadas dentro de `listas.db` com normalização *case-insensitive*;
   - Submenus dinâmicos **Configurações > Excluir Item** para remoção granular com confirmação traduzida (`Sim`/`Não` e `Yes`/`No`).
2. **Gerenciamento Cronológico, Edição em Lote, Cores & Segurança (`GerenciamentoAtividades`):**
   - Armazenamento transacional local em `atividades.db` (`QSQLITE`), exibição consolidada ordenada cronologicamente por data (`dd/MM/yyyy`) e prevenção de injeção HTML via `toHtmlEscaped()`;
   - **Colorização Automática (`Coloridas` vs. `Preto`):** mapeia códigos de disciplina via expressão regular `([A-Z]{1,3}\d{3,4})$` ou nome completo para uma paleta de 24 cores em `dicionario_de_cores`, com persistência em `config.json`;
   - Ações completas de **Registrar Definições**, **Editar Item(ns)** (em lote com modal de edição detalhada por registro), **Excluir Item(ns)**, **Limpar Última Entrada** e **Limpar Tudo**.
3. **Exportação Direta para PDF (`Qt6::PrintSupport` / `QPdfWriter`):**
   - Geração nativa de relatórios PDF (`QPageSize::Letter`, margens de `10 mm`) preservando a ordenação cronológica e o esquema cromático ativo.
4. **Internacionalização ao Vivo (`Qt Linguist` `.ts`/`.qm`), Janela Sobre & Logs Diagnósticos:**
   - Alternância instantânea em tempo de execução entre `pt_BR` e `en_US` (`GerenciadorTraducao`, `retranslate_ui()`, `language.json`);
   - Diálogo **Sobre** com 6 abas dinâmicas (`Histórico`, `Detalhes`, `Licenças`, `Avisos`, `Política de Privacidade` e `Notas de Versão`) carregadas de `source/assets/`;
   - Rotação automática de logs em `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\logs\file_agenda_*.log` (máximo de `10` arquivos).

---

### 🛡️ Distribuição Microsoft Store, Privacidade e Desinstalação Limpa

> [!IMPORTANT]
> **Conformidade Oficial Microsoft Store (Partner Center / Políticas v7.20):**
> - **Política de Privacidade Independente (Política 10.5):** **A Microsoft NÃO fornece uma política de privacidade padrão para a AGENDA DE AVALIAÇÕES ACADÊMICAS, e este software NÃO é coberto pela [Política de Privacidade da Microsoft](https://www.microsoft.com/pt-br/privacy/privacystatement).** Todo processamento ocorre localmente no dispositivo e é regido pela [Política de Privacidade do Desenvolvedor](docs/privacy-policy.md#ptbr).
> - **Suporte do Desenvolvedor Independente:** **A Microsoft NÃO presta suporte ao cliente nem assistência técnica para este aplicativo.** Todo suporte ocorre voluntariamente em regime best-effort pelo autor (`linceu_lighthouse@outlook.com` / [GitHub Issues](https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp/issues)).
> - **Garantia de Desinstalação Limpa (Política 10.2.7):** Remoção limpa a qualquer momento via **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou Menu Iniciar → Desinstalar), sem deixar serviços ou processos em segundo plano.

---

### 🚀 Compilação, Execução, Distribuição Limpa e Suíte de Testes

#### Pré-requisitos
- **Sistema Operacional:** Windows 11 / Windows 10 x64
- **Compiladores C++23:** **MSVC x64** (`14.52.37025` + Windows SDK `10.0.28000.0`, Visual Studio 18) e/ou **MinGW-w64 x64** (GCC `15.1.0` em `Tools/mingw1510_64/bin` relativo à instalação do Qt)
- **Qt 6.12.0:** Módulos `Core`, `Gui`, `Widgets`, `Sql`, `PrintSupport`, `LinguistTools` e `Test` (`msvc2022_64` e/ou `mingw_64`)
- **Ferramentas:** **CMake 3.28+** e **Ninja**

#### Comandos CMake / CTest / VS Code

```powershell
# 1. Compilar o aplicativo e gerar a distribuição externa limpa (MSVC ou MinGW)
cmake -DTOOLCHAIN=msvc -DACTION=BuildAgenda -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=BuildAgenda -P cmake/AgendaTaskRunner.cmake

# 2. Compilar e executar os utilitários auxiliares unificados (builds\build_MOCKS_{msvc,mingw})
cmake -DTOOLCHAIN=msvc -DACTION=RunMocks -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=RunMocks -P cmake/AgendaTaskRunner.cmake

# 3. Compilar e executar toda a SUÍTE de testes (9 alvos CTest, SEM distribuição externa)
#    MSVC -> builds\build_suite_msvc | MinGW -> builds\build_suite_mingw
cmake -DTOOLCHAIN=msvc -DACTION=RunSuite -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=RunSuite -P cmake/AgendaTaskRunner.cmake

# 4. Limpar diretórios de build e caches via CMake (opcional)
cmake -P cmake/AgendaClearBuilds.cmake
```

No **VS Code**, utilize diretamente as **6 tarefas visíveis** cadastradas em [`.vscode/tasks.json`](.vscode/tasks.json) (com subtarefas de compilação/execução ocultas automaticamente):
- `Qt6: Compilar & Executar AGENDA (MSVC)` / `Qt6: Compilar & Executar AGENDA (MinGW)`
- `Qt6: Compilar e Executar MOCKS (MSVC)` / `Qt6: Compilar e Executar MOCKS (MinGW)`
- `Qt6: Compilar e Executar SUITE (MSVC)` / `Qt6: Compilar e Executar SUITE (MinGW)`

As distribuições externas limpas são geradas exclusivamente pelos builds do aplicativo em:
- `../AgendaAvaliacoesAcademicas_msvc` (configurável via `AGENDA_DEPLOY_DIR`, `29` arquivos com CRT app-local, sem `vc_redist.x64.exe`)
- `../AgendaAvaliacoesAcademicas_mingw` (configurável via `AGENDA_DEPLOY_DIR`, `27` arquivos com runtime GCC `15.1.0`)

---

### 📚 Documentação Técnica e Conformidade

- 🏗️ [Arquitetura do Sistema e Esquemas SQLite](docs/architecture.md#ptbr)
- 📋 [Conformidade Normativa, Policial Microsoft Store & Qt LGPLv3](docs/compliance.md#ptbr)
- 🔒 [Política de Privacidade e Soberania de Dados](docs/privacy-policy.md#ptbr)
- ⚖️ [Modelo de Duplo Licenciamento e Checklist Microsoft Store](docs/licensing.md#ptbr)
- 🛡️ [Modelo de Segurança e Resiliência em C++23](docs/security-model.md#ptbr)
- 📖 [Guia do Usuário](docs/user-guide.md#ptbr) · [Manual Completo (MANUAL.md)](MANUAL.md#parte-1-manual-de-instruções-português---br)
- 💡 [Sugestões de Evolução e Backlog Técnico](docs/suggested-issues.md#ptbr)
- 🔬 [Auditoria Técnica, ADRs, Matriz V&V e Roadmap](docs/technical-audit-and-roadmap.md)
- 🔭 [Observação Técnica e Árvore de Arquivos (OBSERVACAO.md)](OBSERVACAO.md#ptbr)
- 🚀 [Notas de Lançamento da Versão 2026.11.22.0](docs/release-v2026.11.22.0.md#ptbr)

---

### ⚖️ Licenciamento e Suporte

- **Código-Fonte Público (GitHub):** Licenciado sob **GNU Affero General Public License v3 (GNU AGPLv3)** ([`LICENSE`](LICENSE)).
- **Pacote Oficial Microsoft Store:** Regido pelo **[EULA Oficial do Autor](source/assets/EULA/EULA_pt_BR.txt)** e **[CLC](source/assets/CLC/CLC_pt_BR.txt)**.
- **Componentes de Terceiros:** Qt 6.12.0 (`GNU LGPLv3`), SQLite (`Public Domain`), Windows SDK / CRT MSVC e runtime MinGW-w64 (`GCC 15.1.0`) — atribuições completas em [`source/assets/NOTICES/NOTICE_pt_BR.txt`](source/assets/NOTICES/NOTICE_pt_BR.txt) e [`source/assets/LICENSES/`](source/assets/LICENSES).
- **Contribuição e Suporte:** Veja [`CONTRIBUTING.md`](CONTRIBUTING.md#ptbr) e [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md#ptbr). Contato: `linceu_lighthouse@outlook.com`.

---

Pequena nota: esta edição consolidada em C++23 / Qt 6.12.0 (v2026.11.22.0) entrega distribuições externas ultra-limpas em MSVC e MinGW, arquitetura de duplo licenciamento, documentação técnica bilíngue completa e uma suíte modular CTest/QtTest abrangendo todos os módulos do aplicativo.

---

</details>

<h2 id="enus"></a>🇺🇸 English (US)</h2>

> **Note:** This repository contains the open-source codebase and compliance documentation for **ACADEMIC EVALUATION SCHEDULE** (**AGENDA DE AVALIAÇÕES ACADÊMICAS** — native **C++23** and **Qt 6.12.0** implementation with local **SQLite** storage), published under a transparent **Dual-Licensing** architecture (**GNU AGPLv3** for public GitHub source code and the Author's **Official EULA + CLC** for digitally signed official packages distributed on the Microsoft Store, executing 100% locally and offline).

<details>
<summary><b>Click to expand / collapse README in English (US)</b></summary><br>

# ACADEMIC EVALUATION SCHEDULE

Version: v2026.11.22.0
Author: Fernando Nillsson Cidade

**ACADEMIC EVALUATION SCHEDULE** (Version `2026.11.22.0`) is a native Windows desktop utility built in **modern C++23** and **Qt 6.12.0** that helps students, educators, and academic coordinators gather every semester assessment (exams, quizzes, assignments, reports, seminars, presentations, and capstone milestones) into a single chronological dashboard.

```text
Local-First Dataflow (100% Offline)
 ├── Cascading / Free-Text Input: Course -> Syllabus -> Semester -> Subject + Class + Type + Sequence + Date
 ├── Dual SQLite Storage (%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\):
 │    ├── atividades.db  (atividades: id, data, tipo, sequencia, nome, turma)
 │    └── listas.db      (7 language-scoped custom tables with case-insensitive deduplication)
 ├── Preferences & Logs: config.json ("coloridas"|"preto"), language.json ("pt_BR"|"en_US"), logs/ (max 10 files)
 └── Display & Export: Chronological QTextBrowser (with HTML escaping) + QPdfWriter / Qt PrintSupport (.pdf)
```

---

### ⚖️ Copyright Holder Manifesto & Legal Authorization

> **Express Statement by Fernando Nillsson Cidade:**  
> I, **Fernando Nillsson Cidade**, as sole author, original creator, and exclusive copyright holder of **ACADEMIC EVALUATION SCHEDULE (AGENDA DE AVALIAÇÕES ACADÊMICAS)**, **authorize the use, study, copying, modification, compilation, and distribution of the public source code in this repository** strictly under **[LICENSE](LICENSE)** (**GNU Affero General Public License v3 — GNU AGPLv3**, SPDX identifier `AGPL-3.0-only`), reserving name and trademark protection under Section 7(e).  
> Any modified version or derivative work must preserve original authorship attribution and make its corresponding source code available under **GNU AGPLv3**.  
> **Freedom for Institutional Documentation in `source/assets/`:** Compliance texts under [`source/assets/`](source/assets/COPYRIGHT/COPYRIGHT%20AND%20TRADEMARK%20NOTICE_en_US.txt) (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `MANUAL`, `NOTICES`, `PRIVACY_POLICY`, and `RELEASE`) were authored to support official Microsoft Store and signed Windows installer compliance; the author **expressly grants developers and forks full freedom** to modify, delete, add, or adapt these files for their own distributions under applicable laws and platform policies.  
> **Boundary for the Author's Signed Official Binary (Microsoft Store):** Packaged and digitally signed Store binaries published by the author are governed by the Author's **[Official EULA](source/assets/EULA/EULA_en_US.txt)** (in harmony with Microsoft Store Standard Application License Terms) and **[Commercial Licensing Clarification — CLC](source/assets/CLC/CLC_en_US.txt)**. Unauthorized paid commercial resale or brand misappropriation of the author's official signed package is prohibited.  
> **Support Channel, Microsoft Independence & Legal Disclaimer:** **Microsoft does NOT provide customer support and does NOT cover this software under Microsoft's Privacy Statement.** Any developer support is strictly voluntary on a best-effort basis via `linceu_lighthouse@outlook.com` and GitHub Issues/Discussions with no SLA. Provided **"AS IS"** without warranties; always confirm official deadlines and syllabi directly with instructors and academic institutions. See [`docs/licensing.md`](docs/licensing.md#enus).

---

### 📌 Project Fact Sheet

- **Official Name:** ACADEMIC EVALUATION SCHEDULE (`AGENDA DE AVALIAÇÕES ACADÊMICAS`)
- **Author & Maintainer:** Fernando Nillsson Cidade *(Individual Developer)*
- **Support Email (voluntary best-effort, no SLA):** `linceu_lighthouse@outlook.com`
- **Official Repository:** [https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp](https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp)
- **Runtime Dynamic Languages:** Português (Brasil — `pt_BR`) · English (United States — `en_US`)
- **System Requirements:** Windows 11 / Windows 10 x64 (end users of clean external distributions do not need Qt SDK, compilers, or `vc_redist.x64.exe`)
- **CMake Targets, Modules & Presets (`CMakeLists.txt`, `cmake/*.cmake`, and `CMakePresets.json`):**
  - `AgendaCore` (static core library shared between app and test suite) + `AgendaAvaliacoesAcademicas.exe` embedding `source/resources.rc` (`qt-msvc` -> `builds/build_AGENDA_msvc/` and `qt-mingw` -> `builds/build_AGENDA_mingw/`)
  - `DeployAgenda`: creates ultra-clean external distributions under `../AgendaAvaliacoesAcademicas_{msvc,mingw}` (configurable via `AGENDA_DEPLOY_DIR`, minimal Qt DLLs, required plugins, 5 app-local MSVC CRT DLLs or 3 MinGW GCC `15.1.0` DLLs, 13 runtime assets, and 2 `.qm` files)
  - **Inno Setup Installers (`library/Agenda_Avaliacoes_Academicas*.iss`):** package clean distributions without embedding `vc_redist.x64.exe`
  - `VersionEditor.exe` and `CompileTranslations.exe` (`mocks/` -> `builds/build_MOCKS_{msvc,mingw}/`): independent utilities for version auditing/updates (32 PT/EN specs) and `.ts` -> `.qm` translation compilation
  - **Modular CTest / QtTest Suite (`suite-msvc` -> `builds/build_suite_msvc` / `suite-mingw` -> `builds/build_suite_mingw`):** **9 standalone test suites** under `tests/` (generating zero external distribution for tests)

---

### 🌟 Key Capabilities Implemented

1. **Cascading Curricula & Persistent Custom Lists (`BancoDeDados`):**
   - Hierarchical selection for **Course -> Syllabus -> Semester -> Subject** plus **Class**, **Assessment Type**, **Sequence**, and **Date** (`QCalendarWidget`), backed by built-in `pt_BR` and `en_US` catalogs (`CURSOS_PT`, `CURSOS_EN`);
   - All combos accept free-text custom values persisted per language across 7 SQLite tables in `listas.db` with case-insensitive deduplication;
   - Dynamic **Settings > Delete Item** submenus for granular custom-entry removal with localized confirmation prompts.
2. **Chronological Schedule Manager, Batch Editing, Color Modes & Security (`GerenciamentoAtividades`):**
   - Local transactional storage in `atividades.db` (`QSQLITE`), chronological sorting by `dd/MM/yyyy`, and HTML escaping (`toHtmlEscaped()`) in display and PDF output;
   - **Automatic Color Highlighting (`Colorful` vs. `Black`):** maps subject codes (`([A-Z]{1,3}\d{3,4})$`) or full names onto a 24-color palette persisted via `config.json`;
   - Full CRUD operations: **Register Entries**, **Edit Item(s)** (batch + per-item detail editor), **Delete Item(s)**, **Clear Last Entry**, and **Clear All**.
3. **Direct PDF Export (`Qt6::PrintSupport` / `QPdfWriter`):**
   - Native PDF export (`QPageSize::Letter`, `10 mm` margins) preserving chronological ordering and the active color scheme.
4. **Live Internationalization (`.ts`/`.qm`), About Dialog & Bounded Logs:**
   - Runtime UI + list language switching (`GerenciadorTraducao`, `retranslate_ui()`, `language.json`);
   - 6-tab **About** dialog loaded dynamically from `source/assets/`;
   - Rotating local logs under `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\logs\file_agenda_*.log` (capped at `10` files).

---

### 🚀 Building, Clean External Deployment & Running the Test Suite

#### Prerequisites
- **OS:** Windows 11 / Windows 10 x64
- **C++23 Toolchains:** **MSVC x64** (`14.52.37025` + Windows SDK `10.0.28000.0`, Visual Studio 18) and/or **MinGW-w64 x64** (GCC `15.1.0` at `Tools/mingw1510_64/bin` relative to the Qt installation)
- **Qt 6.12.0:** `Core`, `Gui`, `Widgets`, `Sql`, `PrintSupport`, `LinguistTools`, and `Test`
- **Build Tools:** **CMake 3.28+** and **Ninja**

#### Commands & VS Code Tasks

```powershell
# 1. Build app and generate clean external distribution (MSVC or MinGW)
cmake -DTOOLCHAIN=msvc -DACTION=BuildAgenda -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=BuildAgenda -P cmake/AgendaTaskRunner.cmake

# 2. Build and launch unified maintenance tools (builds\build_MOCKS_{msvc,mingw})
cmake -DTOOLCHAIN=msvc -DACTION=RunMocks -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=RunMocks -P cmake/AgendaTaskRunner.cmake

# 3. Build and run the entire automated TEST SUITE (9 CTest executables, NO external distribution)
#    MSVC -> builds\build_suite_msvc | MinGW -> builds\build_suite_mingw
cmake -DTOOLCHAIN=msvc -DACTION=RunSuite -P cmake/AgendaTaskRunner.cmake
cmake -DTOOLCHAIN=mingw -DACTION=RunSuite -P cmake/AgendaTaskRunner.cmake

# 4. Clean build directories and caches via CMake (optional)
cmake -P cmake/AgendaClearBuilds.cmake
```

In **VS Code**, run the **6 visible tasks** registered in [`.vscode/tasks.json`](.vscode/tasks.json) (build/run subtasks are hidden automatically):
- `Qt6: Compilar & Executar AGENDA (MSVC)` / `Qt6: Compilar & Executar AGENDA (MinGW)`
- `Qt6: Compilar e Executar MOCKS (MSVC)` / `Qt6: Compilar e Executar MOCKS (MinGW)`
- `Qt6: Compilar e Executar SUITE (MSVC)` / `Qt6: Compilar e Executar SUITE (MinGW)`

---

### 📚 Documentation & Compliance Index

- 🏗️ [System Architecture & SQLite Schemas](docs/architecture.md#enus)
- 📋 [Compliance, Microsoft Store Policies & Qt LGPLv3](docs/compliance.md#enus)
- 🔒 [Privacy Policy & Local Data Sovereignty](docs/privacy-policy.md#enus)
- ⚖️ [Dual-Licensing Model & Microsoft Store Checklist](docs/licensing.md#enus)
- 🛡️ [Security Model & C++23 Resilience](docs/security-model.md#enus)
- 📖 [User Guide](docs/user-guide.md#enus) · [Complete Manual (MANUAL.md)](MANUAL.md#part-2-operations-manual-english---us)
- 💡 [Suggested Issues & Contributor Backlog](docs/suggested-issues.md#enus)
- 🔬 [Technical Audit, ADRs, V&V Matrix & Roadmap](docs/technical-audit-and-roadmap.md)
- 🔭 [Technical Observation & Repository Tree (OBSERVACAO.md)](OBSERVACAO.md#enus)
- 🚀 [Release Notes v2026.11.22.0](docs/release-v2026.11.22.0.md#enus)

---

### ⚖️ Licensing & Support

- **Public GitHub Source:** Licensed under **GNU Affero General Public License v3 (GNU AGPLv3)** ([`LICENSE`](LICENSE)).
- **Official Microsoft Store Binary:** Governed by **[Author's Official EULA](source/assets/EULA/EULA_en_US.txt)** and **[CLC](source/assets/CLC/CLC_en_US.txt)**.
- **Third-Party Notices:** Qt 6.12.0 (`GNU LGPLv3`), SQLite (`Public Domain`), MSVC CRT / MinGW-w64 (`GCC 15.1.0`) — see [`source/assets/NOTICES/NOTICE_en_US.txt`](source/assets/NOTICES/NOTICE_en_US.txt) and [`source/assets/LICENSES/`](source/assets/LICENSES).
- **Contributing & Support:** See [`CONTRIBUTING.md`](CONTRIBUTING.md#enus) and [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md#enus). Contact: `linceu_lighthouse@outlook.com`.

---

Small note: this consolidated C++23 / Qt 6.12.0 edition (v2026.11.22.0) delivers ultra-clean MSVC and MinGW external deployments, transparent dual licensing, complete bilingual documentation, and a 9-target CTest/QtTest engineering suite covering all application modules.

---

</details>
