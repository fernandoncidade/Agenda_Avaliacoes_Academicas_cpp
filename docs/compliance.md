# Conformidade Regulatória, Licenças e Validação / Compliance & Verification — AGENDA DE AVALIAÇÕES ACADÊMICAS

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher o documento em português</summary><br>

# AGENDA DE AVALIAÇÕES ACADÊMICAS — Conformidade Técnica, Legal e de Distribuição

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Autor:** Fernando Nillsson Cidade  

---

## 1. Matriz de Conformidade Legal e de Plataformas

| Domínio / Norma | Requisito Aplicável | Evidência / Implementação no Projeto |
|---|---|---|
| **LGPD (Lei nº 13.709/2018) & GDPR** | Minimização, operação local solicitada pelo usuário (art. 7º, V e IX) e transparência | Execução 100% offline em `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\`; zero coleta ou envio de dados pessoais ([`privacy-policy.md`](privacy-policy.md#ptbr)) |
| **Microsoft Store Policies v7.20 — Política 10.5.1** | Política de privacidade própria declarando independência frente à Microsoft | Declaração expressa em [`Privacy_Policy_pt_BR.txt`](../source/assets/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt) e [`privacy-policy.md`](privacy-policy.md) de que a Microsoft não cobre o software nem presta suporte |
| **Microsoft Store — Política 10.2.7** | Garantia de desinstalação limpa | Remoção íntegra sem serviços em segundo plano nem drivers via Configurações do Windows ou instaladores Inno Setup |
| **Código-Fonte Aberto — GNU AGPLv3** | Texto integral, avisos de copyright e copyleft | [`LICENSE`](../LICENSE) (`AGPL-3.0-only`) e manifestos em [`README.md`](../README.md#ptbr) / [`licensing.md`](licensing.md#ptbr) |
| **Qt 6.12.0 — GNU LGPLv3** | Vinculação dinâmica permitindo substituição de bibliotecas Qt pelo usuário final | DLLs Qt 6.12.0 dinâmicas (`Qt6Core.dll`, `Qt6Gui.dll`, `Qt6Widgets.dll`, `Qt6Sql.dll`, `Qt6PrintSupport.dll`) implantadas via `windeployqt` com [`GNU LESSER GENERAL PUBLIC LICENSE Version 3.txt`](../source/assets/LICENSES/GNU%20LESSER%20GENERAL%20PUBLIC%20LICENSE%20Version%203.txt) e [`NOTICES`](../source/assets/NOTICES/NOTICE_pt_BR.txt) |
| **SQLite — Domínio Público** | Atribuição e preservação de aviso upstream | Driver `qsqlite.dll` acompanhado de [`SQLite Is Public Domain.txt`](../source/assets/LICENSES/SQLite%20Is%20Public%20Domain.txt) |
| **Implantação Windows App-Local** | Distribuição limpa sem dependências supérfluas e sem `vc_redist.x64.exe` redundante | Pacotes limpos MSVC (`29` arquivos, 5 DLLs CRT `14.52.37029.0`) e MinGW (`27` arquivos, 3 DLLs GCC `15.1.0`) testados sob `PATH` isolado do Windows |

---

## 2. Cobertura da Suíte de Testes de Engenharia (`tests/`)

Todas as funcionalidades, persistência SQLite, plugins Qt 6.12.0, dicionários, internacionalização, utilitários e interface gráfica são verificadas continuamente por **9 executáveis modulares QtTest/CTest** executados tanto em **MSVC x64** (`builds/build_suite_msvc`) quanto em **MinGW-w64 x64** (`builds/build_suite_mingw`):

| Suíte CTest | Escopo Verificado | Módulos / Frameworks / Plugins Cobertos |
|---|---|---|
| `test_01_FrameworkAndPlugins` | Versões C++23 e Qt 6.12.0, plugins físicos e funcionais em tempo de execução | `QSQLITE`, `qwindows`, `qoffscreen`, `qmodernwindowsstyle`, `qico`, `QPdfWriter`, `QTranslator` |
| `test_02_UtilsAndPersistence` | Utilitários de caminho, combos, concorrência e rotação de logs, registro Windows seguro | `CaminhoPersistenteUtils`, `ApplicationPathUtils`, `IconUtils`, `ComboBoxUtils`, `LogManager` (200 writes em 8 threads + poda <= 10), `TrialManager` (com guarda RAII de Registro) |
| `test_03_DictionariesAndMappings` | Integridade estrutural dos currículos, paletas e traduções automáticas | `CURSOS_PT`, `CURSOS_EN`, `dicionario_de_cores`, `dicionario_disciplinas`, `mapeamento_*`, `obter_cor_por_nome` |
| `test_04_CustomListsDatabase` | Persistência `listas.db`, unicidade case-insensitive, isolamento de idioma e anti-SQLi | `Listas_Personalizadas` (todas as 7 tabelas SQLite + remoção hierárquica + strings maliciosas) |
| `test_05_BancoDadosIntegration` | Mesclagem de listas fixas/personalizadas, resolução em cascata e flags seletivas | `Banco_Dados` (`obter_cursos/ementas/semestres/disciplinas/turmas/avaliacoes/sequencias`, `atualizar_listas_exportadas`) |
| `test_06_ActivityManagerCRUD` | CRUD `atividades.db`, ordenação cronológica, modo `preto`/`coloridas`, escape HTML e escala | `GerenciamentoAtividades` (`gativ_01`..`08`, `11`, `18`..`20`), benchmark de 250 atividades |
| `test_07_TranslationsAndAssets` | Alternância `.qm` `pt_BR <-> en_US`, conteúdo das 6 abas Sobre e auditoria documental | `GerenciadorTraducao`, `ginter_02_OpcoesSobre`, `README.md`, `LICENSE`, `docs/*.md` |
| `test_08_GuiMainWindowAndWorkflows` | Interface principal completa, cascades, diálogos modais, `SobreDialog` e pipeline PDF | `InterfaceGerenciadorAtividades`, `submiter`, `limpar_*`, `excluir_item`, `editar_item`, `popular_menu_itens_personalizados`, `SobreDialog`, `QPdfWriter` |
| `test_09_MocksAndDeploymentIntegrity` | Auditoria `VersionEditor` (32 specs), compilação real `.ts -> .qm` (`lrelease`), scripts CMake e invariantes de tarefas | `mocks/find_VersionEditor`, `AGENDA_LRELEASE_EXECUTABLE`, `cmake/AgendaTaskRunner.cmake`, `cmake/AgendaClearBuilds.cmake`, `.github/workflows/update-readme-version.yml`, `.vscode/tasks.json`, `CMakePresets.json` |

---

## 3. Escopo Auxiliar e Responsabilidade ("AS IS")

A aprovação integral das suítes automatizadas comprova conformidade técnica de software nas condições testadas, não implicando garantia institucional de equivalência com calendários acadêmicos oficiais. O aplicativo permanece ferramenta auxiliar gratuita fornecida **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")** ([EULA](../source/assets/EULA/EULA_pt_BR.txt)).

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse compliance in English</summary><br>

# ACADEMIC EVALUATION SCHEDULE — Technical, Legal & Distribution Compliance

**Version:** `2026.11.22.0` | **Date:** `November 22, 2026` | **Author:** Fernando Nillsson Cidade

## 1. Compliance Matrix
- **LGPD / GDPR Local-First:** Zero network telemetry; all databases/configs live exclusively in `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\`.
- **Microsoft Store Policies v7.20 (10.2.7, 10.5.1):** Dedicated developer Privacy Policy stating Microsoft non-coverage, voluntary best-effort developer support (`linceu_lighthouse@outlook.com`), and clean uninstallation.
- **GNU AGPLv3 + Qt 6.12.0 LGPLv3 + SQLite Public Domain:** Source licensed under `AGPL-3.0-only`, dynamic linking preserves user relinking rights for Qt DLLs, and SQLite Public Domain notices are shipped.
- **App-Local Clean Distribution:** Minimal Windows distributions (`29` MSVC files with 5 CRT DLLs vs. `27` MinGW files with 3 GCC `15.1.0` DLLs), eliminating the `vc_redist.x64.exe` payload from Inno Setup installers.

## 2. Automated Verification Matrix
All 9 modular QtTest/CTest suites (`test_01_FrameworkAndPlugins` through `test_09_MocksAndDeploymentIntegrity`) validate Qt plugins, persistence, concurrency, curriculum catalogs, SQLite databases, runtime `.qm` switching, modal GUI flows, PDF rendering, and `VersionEditor`/`lrelease` integrity across both `builds/build_suite_msvc` and `builds/build_suite_mingw`.

</details>
