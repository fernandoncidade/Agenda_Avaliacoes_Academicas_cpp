# Sugestões de Issues e Backlog / Suggested Issues & Backlog — AGENDA DE AVALIAÇÕES ACADÊMICAS

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher sugestões em português</summary><br>

# Backlog Técnico e Sugestões para Contribuidores

**Versão:** `2026.11.22.0` | **Data:** `22 de novembro de 2026` | **Autor:** Fernando Nillsson Cidade

Este documento reúne melhorias técnicas reais identificadas durante a auditoria do código-fonte da **AGENDA DE AVALIAÇÕES ACADÊMICAS**:

1. **Barra de Busca e Filtros Dinâmicos na Caixa de Dados (`ui`):** Filtrar atividades exibidas por disciplina, tipo de avaliação ou intervalo de datas diretamente acima do `QTextBrowser` (`gativ_11`).
2. **Exportação para Calendário `.ics` (iCalendar) e `.csv` (`export`):** Complementar a exportação PDF (`gativ_10`) com exportação padronizada iCalendar/CSV para importação em agendas externas pelo usuário.
3. **Sincronização Dinâmica da Versão no Cabeçalho "Sobre" (`ui`):** Substituir a string fixa `2026.4.24.0` em `ginter_03_exibir_sobre.cpp` e uniformizar a capitalização `"Notas de Versão"` para refletir automaticamente `PROJECT_VERSION` e traduzir o rótulo da aba ao trocar para `en_US`.
4. **Parametrização de Conexão/Caminho em `Listas_Personalizadas` (`database`):** Refatorar a flag estática `g_tabelasCriadas` e o cache `g_logFile` de `LogManager.cpp` em instâncias configuráveis ou verificação por caminho de banco, facilitando múltiplas trocas de diretório dentro do mesmo processo sem reinicialização.
5. **Limpeza de Código Legado em `TrialManager` e Prefixos de Log (`cleanup`):** Remover a chave legada `EisenhowerOrganizer` em `TrialManager` e atualizar o prefixo histórico `FileCompression:` em `LogManager.cpp` para `AgendaAvaliacoesAcademicas:`.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse suggested issues in English</summary><br>

# Technical Backlog & Contributor Suggestions

**Version:** `2026.11.22.0` | **Date:** `November 22, 2026` | **Author:** Fernando Nillsson Cidade

Actionable engineering improvements identified from direct codebase inspection:

1. **Live Search & Filter Controls (`ui`):** Filter displayed schedule entries by subject, assessment type, or date range above the Data Box.
2. **`.ics` (iCalendar) and `.csv` Export (`export`):** Complement `QPdfWriter` output with portable iCalendar and UTF-8 CSV exports.
3. **Dynamic Version String & Tab Label Case Fix in About Dialog (`ui`):** Replace the hardcoded version literal in `ginter_03_exibir_sobre.cpp` with `PROJECT_VERSION` and align `"Notas de versão"` casing with `manager_compression_en_US.ts` (`"Notas de Versão"`).
4. **Multi-Root Path Testability (`database`, `logging`):** Key `g_tabelasCriadas` and `LogManager` state off the resolved directory path instead of process-global booleans.
5. **Legacy Identifier Cleanup (`utils`):** Retire the historical `EisenhowerOrganizer` registry key in `TrialManager` and rename the historical `FileCompression:` log tag in `LogManager.cpp`.

</details>
