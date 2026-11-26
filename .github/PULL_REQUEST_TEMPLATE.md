# Modelo de Pull Request / Pull Request Template

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

> [!NOTE]
> **Aviso ao Colaborador / Contributor Notice:**  
> 🇧🇷 *Este repositório é um projeto pessoal mantido pelo autor em ritmo próprio. Pull Requests são bem-vindos em caráter voluntário. Nos termos da licença GNU AGPLv3, sinta-se plenamente livre para manter suas melhorias em seu próprio fork.*  
> 🇺🇸 *This repository is a personal project maintained voluntarily by the author. Pull Requests are welcome. Under GNU AGPLv3, feel free to maintain your enhancements in your own fork.*

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details open>
<summary>Clique para expandir / recolher o template em português</summary><br>

## Descrição
Descreva sucintamente o objetivo deste PR, módulos impactados (`BancoDeDados`, `GerenciamentoAtividades`, `GerenciamentoInterfaceGrafica`, `language`, `utils`, `tests`) e issue relacionada.

## Tipo de Alteração
- [ ] Correção de bug (persistência SQLite, combos em cascata, edição/exclusão, exportação PDF)
- [ ] Novo recurso / catálogo curricular ou paleta de cores
- [ ] Atualização de traduções (`pt_BR` / `en_US`) e/ou documentos em `source/assets/` e `docs/`
- [ ] Ampliação da suíte de testes automatizados em `tests/`

## Verificação e Testes Executados
- [ ] `Qt6: Compilar e Executar SUITE (MSVC)` (`builds/build_suite_msvc`) aprovado 100%
- [ ] `Qt6: Compilar e Executar SUITE (MinGW)` (`builds/build_suite_mingw`) aprovado 100%
- [ ] Distribuição externa limpa (`DeployAgenda`) validada sem inclusão de arquivos desnecessários

## Conformidade Arquitetural
- [ ] O software permanece estritamente **100% local-first e offline** (zero telemetria / rede).
- [ ] Consultas SQLite utilizam bindings parametrizados e textos exibidos em HTML usam `toHtmlEscaped()`.

## Issues Relacionadas
Corrige #(número da issue)

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details open>
<summary>Click to expand / collapse template in English</summary><br>

## Description
Briefly describe the goal of this PR, affected modules, and linked issue.

## Verification & Automated Tests
- [ ] `Qt6: Compilar e Executar SUITE (MSVC)` (`builds/build_suite_msvc`) passed 100%
- [ ] `Qt6: Compilar e Executar SUITE (MinGW)` (`builds/build_suite_mingw`) passed 100%
- [ ] Clean external deployment (`DeployAgenda`) verified free of extraneous files

## Architectural Compliance
- [ ] Remains strictly **100% local-first and offline** with zero telemetry/network calls.
- [ ] SQLite statements remain parameterized and rendered HTML escapes untrusted text.

## Related Issues
Fixes #(issue number)

</details>
