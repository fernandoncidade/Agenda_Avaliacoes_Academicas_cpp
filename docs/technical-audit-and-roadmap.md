# Auditoria Técnica, ADRs, Matriz V&V e Roadmap / Technical Audit, ADRs & Roadmap — AGENDA DE AVALIAÇÕES ACADÊMICAS

**Versão / Version:** `2026.11.22.0`  
**Data / Date:** `22 de novembro de 2026` / `November 22, 2026`  
**Autor / Author:** Fernando Nillsson Cidade  

Este documento consolida a auditoria técnica de engenharia de software, decisões arquiteturais (ADRs), rastreabilidade completa de verificação e validação (V&V), achados reais de código e roadmap da **AGENDA DE AVALIAÇÕES ACADÊMICAS**.

---

## A. Inventário Funcional e Classificação por Módulo

| Módulo / Subsistema | Escopo Real Verificado | Status | Evidência Principal | Testes Associados |
|---|---|---|---|---|
| **Dicionários e Catálogos (`DicionariosListasBancoDeDados`)** | `CURSOS_PT`, `CURSOS_EN`, 24 cores `#RRGGBB`, listas e mapeamentos PT/EN | Implementado, Validado, Testado, Documentado | `CURSOS_{PT,EN}.cpp`, `dicionario_de_cores.cpp`, `dicionario_disciplinas.cpp` | `test_03_DictionariesAndMappings` |
| **Listas Personalizadas (`Listas_Personalizadas`)** | 7 tabelas em `listas.db`, normalização `toCaseFolded()`, isolamento de idioma e remoção granular | Implementado, Integrado, Testado, Documentado | `Listas_Personalizadas.cpp` | `test_04_CustomListsDatabase` |
| **Integração de Dados (`Banco_Dados`)** | Mesclagem `_mesclar_listas`, resolução em cascata e mapeamento `obter_cor_por_nome` | Implementado, Integrado, Testado, Documentado | `Banco_Dados.cpp` | `test_05_BancoDadosIntegration` |
| **Gerenciador de Atividades (`GerenciamentoAtividades`)** | CRUD SQLite `atividades.db`, ordenação `dd/MM/yyyy`, HTML escaping, modo `coloridas`/`preto`, `config.json`, PDF (`QPdfWriter`) | Implementado, Integrado, Testado, Documentado | `Gerenciador_Atividades.cpp`, `gativ_01`..`gativ_21` | `test_06_ActivityManagerCRUD`, `test_08_GuiMainWindowAndWorkflows` |
| **Interface Gráfica (`InterfaceGerenciadorAtividades`)** | Janela principal Qt 6.12.0, menus, edição em lote/detalhes, exclusão, `SobreDialog` (6 abas) | Implementado, Integrado, Testado, Documentado | `Interface_Grafica.cpp`, `ginter_01`..`ginter_12` | `test_08_GuiMainWindowAndWorkflows` |
| **Internacionalização (`GerenciadorTraducao`)** | Carregamento `.qm`, persistência `language.json`, sinal `idioma_alterado`, retradução ao vivo | Implementado, Integrado, Testado, Documentado | `tr_01_gerenciadorTraducao.cpp`, `.ts`/`.qm` | `test_07_TranslationsAndAssets` |
| **Utilitários & Segurança (`utils/`)** | `LOCALAPPDATA`, descoberta de assets/ícones, `LogManager` (`QMutex`, máx. 10 logs), `TrialManager` | Implementado, Integrado, Testado, Documentado | `source/utils/*.cpp` | `test_02_UtilsAndPersistence` |
| **Build, Deploy Limpo & Ferramentas (`CMake`, `cmake/`, `mocks/`, `library/`)** | Presets C++23 / Qt 6.12.0 MSVC+MinGW, módulos `cmake/*.cmake`, `AgendaCore`, distribuições 29/27 arquivos sem `vc_redist`, `VersionEditor` | Implementado, Integrado, Testado, Documentado | `CMakeLists.txt`, `cmake/*.cmake`, `CMakePresets.json`, `mocks/`, `library/*.iss` | `test_01_FrameworkAndPlugins`, `test_09_MocksAndDeploymentIntegrity` |

---

## B. Decisões Arquiteturais (ADRs)

- **ADR-001 — Biblioteca Estática Compartilhada `AgendaCore`:** Extrair `${APP_SOURCES}` e `${APP_HEADERS}` para o target `AgendaCore` permite que `AgendaAvaliacoesAcademicas.exe` e os 9 executáveis de teste linkem exatamente o mesmo código compilado sem duplicar compilações nem modificar o código de produção.
- **ADR-002 — Separação da Suíte em 9 Executáveis CTest Isolados + `LOCALAPPDATA` Temporário:** A análise de código identificou variáveis estáticas de processo (`g_tabelasCriadas` em `Listas_Personalizadas.cpp`, `g_logFile` em `LogManager.cpp`, caches `g_lista*`/`g_mapeamentoCodigoCor` em `Banco_Dados.cpp` e `static const QString` em `ginter_02_OpcoesSobre.cpp`). Executar cada domínio em processo CTest próprio com `IsolatedLocalAppDataGuard` garante zero contaminação entre testes e zero alterações na máquina do usuário.
- **ADR-003 — Implantação Externa Ultra-Limpa App-Local (Sem `vc_redist.x64.exe`):** Exclusões cirúrgicas no `windeployqt` associadas às 5 DLLs app-local do CRT MSVC `14.52` (ou 3 DLLs do MinGW GCC `15.1.0`) reduzem a distribuição externa MSVC para 29 arquivos (`30,84 MB`) e o instalador Inno Setup para `11,08 MB`, evitando o embutimento redundante de `vc_redist.x64.exe`.
- **ADR-004 — Isolamento Estrito da Suíte de Testes (Sem Distribuição Externa):** Builds `suite-msvc` (`builds/build_suite_msvc`) e `suite-mingw` (`builds/build_suite_mingw`) configuram `AGENDA_BUILD_TESTS=ON` e dependem apenas de cópia local dos assets/traduções em `${CMAKE_CURRENT_BINARY_DIR}`, mantendo os diretórios externos de distribuição exclusivos do aplicativo final.
- **ADR-005 — Duplo Licenciamento Transparente:** Adoção de `GNU AGPLv3` em `LICENSE` para o repositório aberto combinada com autorização expressa para adaptação de `source/assets/` e `EULA Oficial + CLC` exclusivos ao pacote assinado na Microsoft Store.
- **ADR-006 — Modularização CMake e Orquestração Independente de PowerShell (`cmake/`):** Extração das rotinas de build, traduções, implantação, formatação (`AgendaFormatting.cmake`), limpeza (`AgendaClearBuilds.cmake`) e execução de tarefas (`AgendaTaskRunner.cmake`) para `cmake/`, realocação de `resources.rc` em `source/resources.rc`, organização de empacotamento em `library/` e unificação dos alvos de `mocks/` em `builds/build_MOCKS_{msvc,mingw}` com `.vscode/tasks.json` reduzido às 6 tarefas principais visíveis.

---

## C. Matriz de Rastreabilidade e Cobertura V&V

| Requisito / Recurso | Módulo Fonte | Suíte de Teste | Critério Verificado |
|---|---|---|---|
| Qt 6.12.0, C++23 e plugins (`qsqlite`, `qico`, `qwindows`, `qoffscreen`, `qmodernwindowsstyle`) | `CMakeLists.txt` | `test_01_FrameworkAndPlugins` | Versão em tempo de execução, presença física e funcionamento ao vivo dos plugins e PDF |
| Concorrência e rotação de logs (`<= 10` arquivos) + caminhos | `LogManager.cpp`, `utils/` | `test_02_UtilsAndPersistence` | 200 escritas concorrentes em 8 threads sem perda e poda determinística |
| Guard de registro `TrialManager` | `TrialManager.cpp` | `test_02_UtilsAndPersistence` | Restauração integral de `HKCU\...\FirstRunTimestamp` após testes de expiração |
| Catálogos e cores hex `#RRGGBB` | `DicionariosListasBancoDeDados/` | `test_03_DictionariesAndMappings` | Integridade PT/EN, mapeamento de códigos regex e traduções |
| SQLite `listas.db` (7 tabelas) e anti-SQLi | `Listas_Personalizadas.cpp` | `test_04_CustomListsDatabase` | Deduplicação case-insensitive, isolamento de idioma e payloads `'; DROP TABLE...` |
| Mesclagem hierárquica `Banco_Dados` | `Banco_Dados.cpp` | `test_05_BancoDadosIntegration` | Cascata `curso -> ementa -> semestre -> disciplina` e flags seletivos |
| CRUD `atividades.db`, ordenação, XSS/HTML e benchmark | `gativ_01`..`21` | `test_06_ActivityManagerCRUD` | Ordenação `dd/MM/yyyy`, escape `<script>`, modo `preto`/`coloridas` e carga de 250 itens |
| Tradução `.qm`, 6 abas Sobre e documentos | `tr_01_*`, `ginter_02_*`, `docs/` | `test_07_TranslationsAndAssets` | Sinal `idioma_alterado`, textos AGPLv3/EULA, ausência de documentação CLI |
| Fluxos end-to-end GUI, diálogos modais, `SobreDialog` e PDF | `Interface_Grafica.cpp`, `ginter_*` | `test_08_GuiMainWindowAndWorkflows` | Interação completa em `offscreen` sem travamentos modais + verificação `%PDF-` / `%%EOF` |
| Ferramenta `VersionEditor` (32 especificações), scripts CMake & `lrelease` | `mocks/find_VersionEditor.cpp`, `cmake/AgendaTaskRunner.cmake`, `cmake/AgendaClearBuilds.cmake` | `test_09_MocksAndDeploymentIntegrity` | Auditoria/atualização em sandbox (32 specs), compilação `.qm`, validação de presets e invariantes de visibilidade em `.vscode/tasks.json` |

---

## D. Achados Conhecidos da Auditoria (Documentados sem Alterar Código do Produto)

1. **`ginter_03_exibir_sobre.cpp:53` & `:73`:** O cabeçalho fixo traz `"2026.4.24.0"` hardcoded e a chave `"Notas de versão"` difere em caixa de `"Notas de Versão"` no `.ts`. Documentado para correção futura (Issue sugerida em `suggested-issues.md`).
2. **`tr_01_gerenciadorTraducao.cpp:117-123` (`traduzir_botoes_padrao`):** Utiliza contexto `"Dialog"` (ausente em `manager_compression_*.ts`), enquanto `ginter_12_excluir_item_lista_personalizada.cpp` emprega `"InterfaceGerenciadorAtividades"` corretamente traduzido. Documentado sem modificar o código-fonte atual e validado em `test_07_TranslationsAndAssets`.
3. **`Listas_Personalizadas.cpp:14` & `LogManager.cpp:15`:** Uso de flags estáticas em memória (`g_tabelasCriadas`, `g_logFile`) pressupõe diretório `LOCALAPPDATA` constante por processo; adequadamente isolado via múltiplos executáveis CTest.
4. **`TrialManager.hpp/.cpp`:** Mantém constantes legadas (`EisenhowerOrganizer` no registro e `LIBERAR_USO_DEFINITIVO = false`), coberto nos testes com guarda RAII para nunca alterar o estado do host nem encerrar o processo em execução.
5. **`LogManager.cpp:70`:** Prefixo histórico `"FileCompression:"` preservado nos logs atuais (`file_agenda_*.log`).
