# Licenciamento e Esclarecimento Comercial / Licensing & Commercial Model — AGENDA DE AVALIAÇÕES ACADÊMICAS

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher o documento de licenciamento em português</summary><br>

# AGENDA DE AVALIAÇÕES ACADÊMICAS — Duplo Licenciamento e Esclarecimento Comercial

**Versão:** `2026.11.22.0`  
**Data:** `22 de novembro de 2026`  
**Licença do Código-Fonte:** GNU Affero General Public License v3 (GNU AGPLv3)  
**Autor e Detentor dos Direitos Autorais:** Fernando Nillsson Cidade  

---

## 1. Visão Geral do Modelo de Duplo Licenciamento

A **AGENDA DE AVALIAÇÕES ACADÊMICAS** adota um modelo transparente de duplo licenciamento que concilia as liberdades do software livre copyleft forte com a distribuição oficial assinada na Microsoft Store:

| Componente | Licença | Usos Permitidos |
|---|---|---|
| **Código-Fonte (Repositório Completo)** | [GNU AGPLv3](../LICENSE) | 100% Livre: uso, estudo, compilação, modificação, criação de forks e distribuição sob os termos copyleft da GNU AGPLv3 (`AGPL-3.0-only`). |
| **Documentação em `source/assets/`** | Autoria do Projeto (Uso Livre para Desenvolvedores) | Elaborada para atender exigências de conformidade da Microsoft Store e instaladores; **desenvolvedores e forks têm liberdade total** para alterar, excluir, adicionar e adaptar em suas próprias distribuições e plataformas. |
| **Binários Compilados por Terceiros (via Código)** | GNU AGPLv3 | Livres para compilar, executar, empacotar e redistribuir cumprindo integralmente os termos da GNU AGPLv3 (incluindo oferta do código-fonte correspondente). |
| **Binário Oficial na Microsoft Store** | [EULA Oficial do Autor](../source/assets/EULA/EULA_pt_BR.txt) & [CLC](../source/assets/CLC/CLC_pt_BR.txt) | Distribuição oficial assinada pelo autor em harmonia com os Termos de Licença Padrão para Aplicativos da Microsoft Store. Proibida revenda comercial não autorizada do pacote oficial ou apropriação indevida da marca. |
| **Bibliotecas de Terceiros (`Qt 6.12.0`, `SQLite`)** | GNU LGPLv3 / Domínio Público | Atribuições completas em [`source/assets/NOTICES/NOTICE_pt_BR.txt`](../source/assets/NOTICES/NOTICE_pt_BR.txt) e [`source/assets/LICENSES/`](../source/assets/LICENSES). |

---

## 2. Direitos de Código Aberto (GNU AGPLv3)

Todo o código-fonte em C++23 / Qt 6.12.0 deste repositório (`source/`, `mocks/`, `tests/`, `CMakeLists.txt`, `CMakePresets.json`) está licenciado sob a **GNU Affero General Public License v3 (GNU AGPLv3)**:

```text
AGENDA DE AVALIAÇÕES ACADÊMICAS (Academic Evaluation Schedule)
Copyright (c) 2025-2026 Fernando Nillsson Cidade

Este programa é um software livre: você pode redistribuí-lo e/ou modificá-lo sob os termos da GNU Affero General Public License conforme publicada pela Free Software Foundation, seja a versão 3 da Licença, ou (a seu critério) qualquer versão posterior.

Este programa é distribuído na esperança de que seja útil, mas SEM QUALQUER GARANTIA; sem mesmo a garantia implícita de COMERCIALIZAÇÃO ou ADEQUAÇÃO A UM DETERMINADO FIM. Veja a GNU Affero General Public License para mais detalhes.

Você deve ter recebido uma cópia da GNU Affero General Public License junto com este programa. Se não, veja em: <https://www.gnu.org/licenses/>.
```

---

## 3. Documentação em `source/assets/` e Liberdade Total para Desenvolvedores

Os arquivos institucionais e regulatórios mantidos em `source/assets/` (`ABOUT`, `CLC`, `COPYRIGHT`, `EULA`, `LICENSES`, `MANUAL`, `NOTICES`, `PRIVACY_POLICY` e `RELEASE`) possuem finalidade e permissão expressas:
1. **Finalidade Original:** Atender às exigências de publicação, EULA, avisos de terceiros, privacidade e suporte da **Microsoft Store** e instaladores Windows.
2. **Autorização Expressa para Desenvolvedores:** O autor **concede autorização irrestrita** para que desenvolvedores, contribuidores e mantenedores de forks modifiquem, removam ou adaptem esses textos como julgarem adequado em suas próprias distribuições, observada a legislação de cada jurisdição e as diretrizes da plataforma de destino.

---

## 4. Diferenciação do Binário Oficial Assinado (Microsoft Store)

- **Compilações Próprias:** Qualquer usuário ou instituição pode compilar o projeto localmente com MSVC x64 ou MinGW-w64 x64 (ou redistribuir suas compilações próprias sob GNU AGPLv3).
- **Pacote Oficial da Microsoft Store:** Regido pelo [EULA Oficial](../source/assets/EULA/EULA_pt_BR.txt) e pelo [CLC](../source/assets/CLC/CLC_pt_BR.txt), preservando instalação e desinstalação limpa (Política 10.2.7), independência total frente à Microsoft (sem cobertura pela Microsoft Privacy Statement e sem suporte ao cliente pela Microsoft) e vedação de revenda comercial não autorizada do pacote oficial assinado.

---

## 5. Checklist de Submissão e Conformidade Microsoft Store (Partner Center / Políticas v7.20)

| Item do Partner Center | Referência no Repositório (`pt-BR`) | Observação |
|---|---|---|
| **Categoria (Obrigatório)** | *Educação* / *Produtividade* | Configurar nas propriedades da listagem |
| **Política de Privacidade (Obrigatório)** | [`docs/privacy-policy.md`](privacy-policy.md#ptbr) · [`Privacy_Policy_pt_BR.txt`](../source/assets/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt) | App Win32/`runFullTrust` 100% offline. **Microsoft não fornece política padrão nem cobre este app.** |
| **Site Oficial** | `https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp` | Aponta para o repositório oficial do desenvolvedor |
| **Suporte ao Cliente** | `linceu_lighthouse@outlook.com` / GitHub Issues | Suporte voluntário best-effort pelo autor; **a Microsoft não presta suporte** |
| **Desinstalação Limpa (Política 10.2.7)** | Declarada em `README.md`, `privacy-policy.md` e `EULA` | Sem serviços residentes em segundo plano |

### Textos Prontos para Partner Center
- **Justificativa (`runFullTrust`):**
  > *Academic Evaluation Schedule (Agenda de Avaliações Acadêmicas) is a native Win32 C++23 / Qt 6.12.0 desktop utility storing academic schedules locally in SQLite (%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\) and saving user-requested PDF summaries via standard Windows file dialogs. It operates 100% offline with zero background services, zero drivers, and zero telemetry.*
- **Copyright (`pt-BR`, <= 200 caracteres):**
  > `© 2025-2026 Fernando Nillsson Cidade. Código aberto sob GNU AGPLv3; pacote oficial Store sob EULA do autor. Qt (LGPLv3), SQLite (Public Domain).`

---

## 6. Escopo Auxiliar e Isenção Legal ("AS IS")

Este software gratuito destina-se exclusivamente ao uso auxiliar de organização pessoal e acadêmica de datas avaliativas, sendo fornecido **"NO ESTADO EM QUE SE ENCONTRA" ("AS IS")**, sem garantias expressas ou implícitas ([EULA Oficial](../source/assets/EULA/EULA_pt_BR.txt), Lei nº 9.609/1998 e GNU AGPLv3 Seções 15–16). O usuário deve sempre confirmar prazos oficiais junto à sua instituição de ensino.

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse licensing in English</summary><br>

# ACADEMIC EVALUATION SCHEDULE — Dual Licensing & Commercial Clarification

**Version:** `2026.11.22.0`  
**Date:** `November 22, 2026`  
**Source License:** GNU Affero General Public License v3 (GNU AGPLv3)  
**Author & Copyright Holder:** Fernando Nillsson Cidade  

---

## 1. Dual-Licensing Architecture

| Asset | License | Rights & Boundaries |
|---|---|---|
| **Full Repository Source Code** | [GNU AGPLv3](../LICENSE) | 100% Free Open Source (`AGPL-3.0-only`): inspect, compile, modify, fork, and distribute under strong copyleft terms. |
| **Institutional Docs in `source/assets/`** | Author Grant (Free for Developers) | Written for Microsoft Store / installer compliance; **developers and forks are explicitly granted complete freedom** to edit, remove, or adapt them for their own distributions. |
| **Official Signed Microsoft Store Package** | [Author's Official EULA](../source/assets/EULA/EULA_en_US.txt) & [CLC](../source/assets/CLC/CLC_en_US.txt) | Official Store package governed by the Author's EULA alongside Microsoft Store Standard Application License Terms; third-party paid resale or brand misappropriation of the official binary package is prohibited. |
| **Third-Party Libraries (`Qt 6.12.0`, `SQLite`)** | GNU LGPLv3 / Public Domain | Full notices in [`source/assets/NOTICES/NOTICE_en_US.txt`](../source/assets/NOTICES/NOTICE_en_US.txt) and [`source/assets/LICENSES/`](../source/assets/LICENSES). |

---

## 2. Microsoft Store Compliance & Legal Disclaimer ("AS IS")

Microsoft provides **neither customer support nor privacy statement coverage** for this application. Voluntary author support operates strictly on a best-effort basis via `linceu_lighthouse@outlook.com` and GitHub Issues. Provided **"AS IS"** without warranties (Official EULA, Brazilian Law No. 9.609/1998, and GNU AGPLv3 Sections 15–16).

</details>
