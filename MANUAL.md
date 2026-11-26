# MANUAL DO USUÁRIO — AGENDA DE AVALIAÇÕES ACADÊMICAS
# OPERATIONS & USER MANUAL — ACADEMIC EVALUATION SCHEDULE

**Versão / Version:** `2026.11.22.0`  
**Data / Date:** `22 de novembro de 2026` / `November 22, 2026`  
**Ambiente Tecnológico / Tech Stack:** C++23 | Qt 6.12.0 (`Core`, `Gui`, `Widgets`, `Sql`, `PrintSupport`, `LinguistTools`) | SQLite  
**Plataforma / Platform:** Windows 11 / Windows 10 x64  
**Autor / Author:** Fernando Nillsson Cidade (`linceu_lighthouse@outlook.com`)  
**Repositório Oficial / Official Repository:** https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp  

---

## Sumário / Table of Contents

- [PARTE 1: MANUAL DE INSTRUÇÕES (PORTUGUÊS - BR)](#parte-1-manual-de-instruções-português---br)
  - [1. Introdução e Arquitetura Local](#1-introdução-e-arquitetura-local)
  - [2. Cadastro em Cascata e Listas Personalizadas](#2-cadastro-em-cascata-e-listas-personalizadas)
  - [3. Gerenciamento de Avaliações (Editar, Excluir, Limpar)](#3-gerenciamento-de-avaliações-editar-excluir-limpar)
  - [4. Colorização Automática, Idiomas ao Vivo e Exportação PDF](#4-colorização-automática-idiomas-ao-vivo-e-exportação-pdf)
  - [5. Janela Sobre, Armazenamento Local, Desinstalação Limpa e FAQ](#5-janela-sobre-armazenamento-local-desinstalação-limpa-e-faq)
- [PART 2: OPERATIONS MANUAL (ENGLISH - US)](#part-2-operations-manual-english---us)
  - [1. Introduction & Local-First Architecture](#1-introduction--local-first-architecture)
  - [2. Cascading Assessment Input & Custom Lists](#2-cascading-assessment-input--custom-lists)
  - [3. Managing Assessments (Edit, Delete, Clear)](#3-managing-assessments-edit-delete-clear)
  - [4. Color Highlighting, Runtime Languages & PDF Export](#4-color-highlighting-runtime-languages--pdf-export)
  - [5. About Dialog, Local Persistence, Clean Uninstall & FAQ](#5-about-dialog-local-persistence-clean-uninstall--faq)

---

# PARTE 1: MANUAL DE INSTRUÇÕES (PORTUGUÊS - BR)

## 1. Introdução e Arquitetura Local

A **AGENDA DE AVALIAÇÕES ACADÊMICAS** é um aplicativo desktop nativo para Windows que centraliza todas as provas, trabalhos, listas, seminários, apresentações e projetos do semestre em ordem cronológica crescente (`dd/MM/yyyy`). O software opera **100% offline (Local-First)** sem contas externas ou telemetria.

- **Bancos de Dados Locais:** Todas as informações são gravadas localmente em `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\`:
  - `atividades.db`: armazena as avaliações agendadas (`data`, `tipo`, `sequencia`, `nome`, `turma`);
  - `listas.db`: armazena valores personalizados digitados pelo usuário, separados por idioma (`pt_BR` / `en_US`) e por categoria;
  - `config.json`: guarda o modo de cores ativo (`"coloridas"` ou `"preto"`);
  - `language.json`: guarda o idioma escolhido (`"pt_BR"` ou `"en_US"`);
  - `logs\file_agenda_yyyyMMdd_hhmmss.log`: registros diagnósticos locais rotacionados automaticamente (máximo de 10 arquivos).

---

## 2. Cadastro em Cascata e Listas Personalizadas

O painel esquerdo da janela principal (`InterfaceGerenciadorAtividades`) apresenta os seletores de preenchimento:
1. **Curso (`combo_curso`):** Selecione um curso pré-cadastrado (como `Engenharia Civil`) ou digite livremente o nome do seu curso.
2. **Ementa (`entry_ementa`):** Atualizada automaticamente em cascata conforme o Curso selecionado; aceita entrada personalizada.
3. **Semestre (`entry_semestre`):** Atualizado automaticamente conforme Curso e Ementa; aceita entrada personalizada.
4. **Disciplina (`entry_disciplina`):** Lista as disciplinas da grade selecionada (com códigos como `CM201`, `TC023`, `TH045`) ou permite digitar uma nova disciplina.
5. **Turma da Disciplina (`entry_codigo`), Tipo de Atividade Avaliativa (`combo_tipo`) e Sequência da Atividade (`combo_sequencia`):** Todos os campos possuem botão de limpeza (`setClearButtonEnabled(true)`) e aceitam edição livre sem inserir duplicatas acidentais enquanto digita (`QComboBox::NoInsert`).
6. **Data da Atividade (`calendar`):** Selecione a data no calendário mensal (formato salvo `dd/MM/yyyy`).

Ao clicar em **Registrar Definições** (ou **Arquivo → Registrar Definições**):
- É verificado se os campos obrigatórios (**Tipo**, **Sequência**, **Disciplina** e **Turma**) estão preenchidos;
- Quaisquer valores inéditos digitados nos campos são automaticamente persistidos em `listas.db` para o idioma atual, preservando a hierarquia `Curso -> Ementa -> Semestre -> Disciplina`;
- A avaliação é gravada em `atividades.db` e a **Caixa de Dados** à direita é atualizada em ordem cronológica imediata.

---

## 3. Gerenciamento de Avaliações (Editar, Excluir, Limpar)

- **Editar Item(ns):** Abre lista com caixas de seleção ordenada cronologicamente. Ao confirmar os itens marcados, o diálogo **Editar Detalhes da Atividade** permite alterar data, tipo, sequência, nome da disciplina (incluindo disciplinas relacionadas do mesmo semestre) e turma, persistindo via `UPDATE` em `atividades.db`.
- **Excluir Item(ns):** Exibe todas as avaliações registradas para seleção múltipla e remoção definitiva após confirmação em **OK**.
- **Limpar Última Entrada:** Remove a última atividade inserida (maior `id` em `atividades.db`) com confirmação visual de sucesso ou alerta quando a agenda estiver vazia.
- **Limpar Tudo:** Apaga integralmente os registros da tabela `atividades`, limpa a visualização na Caixa de Dados e redefine os controles ao estado inicial.
- **Configurações → Excluir Item:** Menu com submenus para cada uma das 7 categorias personalizadas (`Curso`, `Ementa`, `Semestre`, `Disciplina`, `Turma da Disciplina`, `Tipo de Atividade Avaliativa` e `Sequência da Atividade`). Itens hierárquicos são exibidos como caminho completo (ex.: `Curso > Ementa > Semestre > Disciplina`) e solicitam confirmação explícita (**Sim** / **Não**) antes da exclusão em `listas.db`.

---

## 4. Colorização Automática, Idiomas ao Vivo e Exportação PDF

### 4.1. Modo de Cores (`Configurações → Cores`)
- **Coloridas (Padrão):** Associa automaticamente até 24 cores distintas às disciplinas pelo código de identificação (`([A-Z]{1,3}\d{3,4})$`) ou pelo nome completo da disciplina. O destaque colore tanto o título quanto o sublinhado de cada linha na Caixa de Dados e no PDF.
- **Preto:** Aplica cor monocromática uniforme para impressão de alto contraste em preto e branco.

### 4.2. Troca de Idioma em Tempo de Execução (`Configurações → Idiomas`)
- Alterne a qualquer momento entre **Português (Brasil)** (`pt_BR`) e **English (United States)** (`en_US`). Todos os rótulos, botões, menus, botões de diálogo (`OK`/`Cancelar`/`Sim`/`Não`), calendário (`QLocale`) e listas de cursos/ementas/semestres são retraduzidos ao vivo sem reiniciar o aplicativo.

### 4.3. Exportação para PDF (`Exportar para PDF`)
- Gera um documento PDF formatado via `Qt6::PrintSupport` (`QPdfWriter`, página Carta/Letter com margens de `10 mm`) em ordem cronológica crescente, preservando o modo cromático ativo e escapando caracteres especiais com segurança.

---

## 5. Janela Sobre, Armazenamento Local, Desinstalação Limpa e FAQ

- **Janela Sobre (`Sobre → Sobre`):** Exibe cabeçalho institucional e 6 abas informativas carregadas dinamicamente de `source/assets/`: **Histórico**, **Detalhes**, **Licenças**, **Avisos**, **Política de Privacidade** e **Notas de Versão**.
- **Desinstalação Limpa:** Caso instalado via pacote oficial da Microsoft Store ou instalador Windows Inno Setup, utilize **Configurações do Windows → Aplicativos → Aplicativos instalados** (ou **Menu Iniciar → Desinstalar**). O executável não instala drivers nem serviços residentes.
- **Backup Manual:** Para migrar seus dados entre computadores, copie os arquivos `atividades.db`, `listas.db`, `config.json` e `language.json` da pasta `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\`.

### Perguntas Frequentes (FAQ)
1. **O aplicativo precisa de internet?** Não. Funciona 100% offline.
2. **Qual a diferença entre `Excluir Item(ns)` e `Configurações → Excluir Item`?** `Excluir Item(ns)` apaga avaliações agendadas da agenda (`atividades.db`); `Configurações → Excluir Item` apaga sugestões personalizadas digitadas nos menus suspensos (`listas.db`).
3. **As ementas embutidas substituem o plano de ensino da minha faculdade?** Não. Elas atuam como ponto de partida auxiliar; utilize digitação livre para adaptar perfeitamente à matriz curricular da sua instituição.

---

# PART 2: OPERATIONS MANUAL (ENGLISH - US)

## 1. Introduction & Local-First Architecture

**ACADEMIC EVALUATION SCHEDULE (AGENDA DE AVALIAÇÕES ACADÊMICAS)** is a native Windows desktop application that consolidates all semester exams, quizzes, assignments, reports, seminars, and project deadlines in ascending chronological order (`dd/MM/yyyy`). It runs **100% offline (Local-First)** with zero telemetry or cloud dependencies.

- **Local Persistence Directory (`%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\`):**
  - `atividades.db`: stores scheduled activities (`data`, `tipo`, `sequencia`, `nome`, `turma`);
  - `listas.db`: stores user-entered custom values scoped per language (`pt_BR` / `en_US`) across 7 SQLite tables;
  - `config.json`: persists active color mode (`"coloridas"` or `"preto"`);
  - `language.json`: persists active UI language (`"pt_BR"` or `"en_US"`);
  - `logs\file_agenda_yyyyMMdd_hhmmss.log`: automatically pruned local diagnostic logs (at most 10 files).

---

## 2. Cascading Assessment Input & Custom Lists

1. **Course (`combo_curso`), Syllabus (`entry_ementa`), Semester (`entry_semestre`), and Subject (`entry_disciplina`):** Cascading dropdowns populated dynamically from built-in curricula merged with custom SQLite lists. Every field supports free-text typing without accidental insert-on-keystroke (`QComboBox::NoInsert`) and includes an inline clear button.
2. **Class (`entry_codigo`), Type of Assessment Activity (`combo_tipo`), Activity Sequence (`combo_sequencia`), and Calendar Date (`calendar`):** Select predefined entries or type custom labels and choose the deadline on the monthly calendar.
3. Click **Register Entries** (or **File → Register Entries**): validates required fields (Type, Sequence, Subject, Class), persists any new custom values to `listas.db` under the active language, inserts the activity into `atividades.db`, and refreshes the chronologically sorted **Data Box**.

---

## 3. Managing Assessments (Edit, Delete, Clear)

- **Edit Item(s):** Select one or more scheduled activities to open **Edit Activity Details**, updating date, type, sequence, subject, or class directly inside `atividades.db`.
- **Delete Item(s):** Select multiple scheduled items and confirm removal.
- **Clear Last Entry:** Deletes the most recently inserted activity by highest database ID.
- **Clear All:** Deletes all scheduled entries in `atividades.db`, resets form controls, and clears the Data Box.
- **Settings → Delete Item:** Dynamic per-category submenus (`Course`, `Syllabus`, `Semester`, `Subject`, `Class`, `Type of Assessment Activity`, `Activity Sequence`) displaying hierarchical paths (`Course > Syllabus > Semester > Subject`) and localized **Yes** / **No** confirmation before deleting from `listas.db`.

---

## 4. Color Highlighting, Runtime Languages & PDF Export

- **Colors (`Settings → Colors`):** Toggle live between **Colorful** (automatic subject color highlight via code regex `([A-Z]{1,3}\d{3,4})$` or full subject match across a 24-color palette) and **Black** (high-contrast monochrome), persisted to `config.json`.
- **Languages (`Settings → Languages`):** Switch live between **Português (Brasil)** and **English (United States)** (`GerenciadorTraducao`, `.qm` catalogs, calendar `QLocale`, and curriculum dictionaries) without restarting.
- **Export to PDF (`Export to PDF`):** Generates a chronological formatted PDF via `QPdfWriter` (Letter paper, `10 mm` margins) preserving the active color scheme and HTML-escaped text.

---

## 5. About Dialog, Local Persistence, Clean Uninstall & FAQ

- **About Dialog (`About → About`):** Displays institutional details and 6 tabs dynamically loaded from `source/assets/` (**History**, **Details**, **Licenses**, **Notices**, **Privacy Policy**, and **Release Notes**).
- **Clean Uninstallation:** Installed builds (Microsoft Store MSIX or Windows Inno Setup) uninstall cleanly from **Windows Settings → Apps → Installed Apps** (or **Start Menu → Uninstall**) leaving zero background services or drivers behind.
- **Manual Backup:** Copy `atividades.db`, `listas.db`, `config.json`, and `language.json` from `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\`.
