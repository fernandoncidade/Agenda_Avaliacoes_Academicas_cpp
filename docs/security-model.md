# Modelo de Segurança e Resiliência / Security Model — AGENDA DE AVALIAÇÕES ACADÊMICAS

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher o modelo de segurança em português</summary><br>

# AGENDA DE AVALIAÇÕES ACADÊMICAS — Modelo de Segurança e Integridade

**Versão:** `2026.11.22.0` | **Data:** `22 de novembro de 2026` | **Autor:** Fernando Nillsson Cidade

## 1. Garantias Arquiteturais

1. **Arquitetura 100% Local-First e Offline:** Zero chamadas de rede, zero portas abertas, zero telemetria e zero sincronização em nuvem.
2. **Defesa contra SQL Injection (`QSQLITE`):** Todas as operações SQL em `atividades.db` (`gativ_02`, `gativ_03`, `gativ_05`, `gativ_06`, `gativ_09`) e `listas.db` (`Listas_Personalizadas.cpp`) empregam exclusivamente queries preparadas com placeholders (`prepare()` + `addBindValue()`).
3. **Neutralização de HTML / XSS em Visualização e PDF (`toHtmlEscaped()`):** Tanto `update_textbox` (`gativ_11`) quanto `exportar_para_pdf` (`gativ_10`) aplicam `toHtmlEscaped()` sobre todos os campos inseridos pelo usuário (`data`, `tipo`, `sequencia`, `nome`, `turma`) antes de gerar HTML para `QTextBrowser` ou `QTextDocument::setHtml`.
4. **Isolamento de Caminhos & Rotação Limitada de Logs:** Toda persistência concentra-se em `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\` via `CaminhoPersistenteUtils` (`obter_caminho_persistente()`), enquanto `LogManager` protege a escrita concorrente com `QMutex` e realiza poda automática mantendo no máximo `kMaxLogFiles = 10` arquivos `file_agenda_*.log`.
5. **Isolamento Estreito de Toolchains e Implantação:** Builds MSVC/MinGW rodam sob `PATH` sanitizado em `CMakePresets.json` e `cmake/AgendaTaskRunner.cmake` (prevenindo mistura de DLLs de runtime entre GCC 13/15 ou MSVC/MinGW), e implantações externas carregam somente as DLLs e plugins essenciais (`29` / `27` arquivos).

## 2. Matriz de Riscos e Mitigações

| Vetor Potencial | Risco | Medida Implementada |
|---|---|---|
| **SQL Injection via entradas livres nos combos** | Baixo | Queries parametrizadas em `atividades.db` e nas 7 tabelas de `listas.db` (validado em `test_04`) |
| **Injeção HTML / Scripting na Caixa de Dados ou PDF** | Baixo | `toHtmlEscaped()` obrigatório em todos os campos (validado em `test_06`) |
| **Colisão de conexões `QSqlDatabase` ou corrupção em concorrência** | Baixo | Identificadores únicos via `QUuid::createUuid()` e fechamento RAII + `QMutex` em `LogManager` (validado em `test_02`) |
| **Crescimento ilimitado de logs em disco** | Baixo | `prune_old_log_files()` mantém no máximo 10 arquivos (validado em `test_02`) |
| **Exfiltração pela rede** | Inexistente | Aplicativo sem módulo `Qt6Network` e sem requisições de rede |

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse security model in English</summary><br>

# ACADEMIC EVALUATION SCHEDULE — Security Model & Data Integrity

**Version:** `2026.11.22.0` | **Date:** `November 22, 2026` | **Author:** Fernando Nillsson Cidade

## 1. Core Security Guarantees

1. **100% Local-First & Offline Execution:** Zero network calls, zero listening sockets, zero telemetry.
2. **Parameterized SQLite Queries:** Every read/write against `atividades.db` and `listas.db` binds parameters via `prepare()` + `addBindValue()` (verified with SQL-injection test payloads in `test_04`).
3. **HTML / XSS Escaping (`toHtmlEscaped()`):** All user inputs rendered into `QTextBrowser` (`gativ_11`) and `QTextDocument` / `QPdfWriter` (`gativ_10`) are HTML-escaped (verified in `test_06`).
4. **Mutex-Guarded Bounded Logging (`kMaxLogFiles = 10`):** `LogManager` serializes writes with `QMutex` and prunes `file_agenda_*.log` files to keep at most 10 files (verified across 8 concurrent threads in `test_02`).
5. **Clean Build & App-Local Deployment Isolation:** Sanitized preset/script environments avoid MSVC/MinGW DLL cross-loading; external packages drop unnecessary `Qt6Network` / `Qt6Pdf` / compiler installers.

</details>
