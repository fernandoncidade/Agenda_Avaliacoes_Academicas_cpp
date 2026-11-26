# Política de Segurança / Security Policy — AGENDA DE AVALIAÇÕES ACADÊMICAS

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher a política em português</summary><br>

# Política de Segurança

**Versão:** `2026.11.22.0` | **Data:** `22 de novembro de 2026` | **Autor:** Fernando Nillsson Cidade

## 1. Versões com Suporte

| Versão | Suportada |
|:---:|:---:|
| `v2026.11.x` | :white_check_mark: |
| `< v2026.11` | :x: |

## 2. Como Relatar uma Vulnerabilidade

Se identificar uma vulnerabilidade de segurança (como injeção SQL em consultas locais, injeção de HTML em renderização/PDF, desserialização insegura de JSON ou falha de carregamento de DLLs na implantação Windows):

1. **NÃO divulgue publicamente em issues abertas.**
2. Relate privadamente pelo recurso **GitHub Security Advisories** em [Security](https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp/security/advisories/new) ou diretamente via e-mail (`linceu_lighthouse@outlook.com`).
3. Inclua passos para reprodução, impacto estimado e versão/toolchain (`MSVC` ou `MinGW`).

## 3. Compromissos Arquiteturais de Segurança

- **100% Local-First e Offline:** Zero chamadas de rede, zero telemetria, zero contas remotas.
- **Consultas SQLite Parametrizadas:** Todas as inserções, buscas, atualizações e exclusões em `atividades.db` e `listas.db` utilizam *prepared statements* (`prepare()` + `addBindValue()`), prevenindo SQL Injection mesmo diante de nomes de disciplinas contendo aspas ou metacaracteres.
- **Sanitização HTML e PDF (`toHtmlEscaped()`):** Entradas textuais exibidas em `QTextBrowser` e enviadas para `QTextDocument` / `QPdfWriter` sofrem escape de caracteres HTML (`<`, `>`, `&`, `"`).
- **Persistência e Logs Limitados:** Dados gravados sob `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\` e logs rotacionados (`kMaxLogFiles = 10` em `LogManager.cpp`) protegidos por `QMutex`.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse the policy in English</summary><br>

# Security Policy

**Version:** `2026.11.22.0` | **Date:** `November 22, 2026` | **Author:** Fernando Nillsson Cidade

## 1. Supported Versions

| Version | Supported |
|:---:|:---:|
| `v2026.11.x` | :white_check_mark: |
| `< v2026.11` | :x: |

## 2. Reporting a Vulnerability

If you discover a potential security issue (such as SQL injection in local queries, HTML injection in schedule rendering/PDF export, unsafe JSON parsing, or Windows DLL loading issues):

1. **Do NOT disclose it in public GitHub issues.**
2. Report privately via **GitHub Security Advisories** under [Security](https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp/security/advisories/new) or via email (`linceu_lighthouse@outlook.com`).
3. Include reproduction steps, affected toolchain (`MSVC` or `MinGW`), and impact details.

## 3. Core Architectural Security Commitments

- **100% Local-First & Offline:** Zero network connections, zero telemetry, zero remote servers.
- **Parameterized SQLite Queries:** Every SQL operation against `atividades.db` and `listas.db` uses `prepare()` and `addBindValue()`.
- **HTML/PDF Escaping (`toHtmlEscaped()`):** User-supplied strings rendered into `QTextBrowser` and `QPdfWriter` are escaped to neutralize markup injection.
- **Bounded Thread-Safe Logs:** Local runtime logs under `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\logs\` are synchronized by `QMutex` and capped at `kMaxLogFiles = 10`.

</details>
