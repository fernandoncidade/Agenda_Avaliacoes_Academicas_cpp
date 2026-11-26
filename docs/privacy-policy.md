# Política de Privacidade / Privacy Policy — AGENDA DE AVALIAÇÕES ACADÊMICAS

<p align="center">
  <b>Selecione o idioma / Select language:</b><br>
  <a href="#ptbr">🇧🇷 Português (BR)</a> | <a href="#enus">🇺🇸 English (US)</a>
</p>

---

## <a id="ptbr"></a>🇧🇷 Português (BR)

<details>
<summary>Clique para expandir / recolher a Política de Privacidade em português</summary><br>

# Política de Privacidade — AGENDA DE AVALIAÇÕES ACADÊMICAS

**Última atualização:** `22 de novembro de 2026`  
**Versão:** `2026.11.22.0`  
**Autor:** Fernando Nillsson Cidade  

> [!IMPORTANT]
> **Independência Frente à Microsoft:** A **Microsoft NÃO fornece uma política de privacidade padrão para a AGENDA DE AVALIAÇÕES ACADÊMICAS**, e este software **NÃO é coberto pela [Política de Privacidade da Microsoft](https://www.microsoft.com/pt-br/privacy/privacystatement)**. Telemetria ou dados de loja coletados pela plataforma Microsoft Store seguem a política própria da Microsoft; todo o processamento realizado pelo aplicativo em execução local é regido exclusivamente por esta Política de Privacidade do desenvolvedor (versão embarcada em [`source/assets/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt`](../source/assets/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt)).

## 1. Operação 100% Local-First e Offline

Todos os dados manipulados permanecem integralmente na máquina do usuário:
- **Zero servidores remotos, zero contas, zero telemetria e zero SDKs analíticos ou de publicidade.**

## 2. Arquivos Armazenados Localmente

Sob o diretório persistente local `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\` (ou contêiner isolado MSIX equivalente):
- `atividades.db` (SQLite): data, tipo, sequência, disciplina e turma de cada avaliação cadastrada;
- `listas.db` (SQLite): itens personalizados digitados pelo usuário separados por idioma (`pt_BR`/`en_US`);
- `config.json`: preferência do modo cromático (`"coloridas"` ou `"preto"`);
- `language.json`: idioma selecionado (`"pt_BR"` ou `"en_US"`);
- `logs\file_agenda_*.log`: logs locais limitados automaticamente a no máximo **10 arquivos** recentes;
- **Exportações PDF:** gravadas exclusivamente na pasta e nome de arquivo selecionados pelo usuário no diálogo padrão de salvamento do Windows.

## 3. Direitos do Titular (LGPD / GDPR) e Desinstalação Limpa

O usuário pode consultar, editar, exportar ou excluir seus registros a qualquer momento dentro do aplicativo ou diretamente no diretório local `%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\`. A desinstalação pelas **Configurações do Windows → Aplicativos** remove o aplicativo de forma limpa sem deixar processos em segundo plano.

## 4. Contato

Fernando Nillsson Cidade — `linceu_lighthouse@outlook.com` — [`https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp`](https://github.com/fernandoncidade/Agenda_Avaliacoes_Academicas_cpp) *(Microsoft não presta suporte ao usuário).*

</details>

---

## <a id="enus"></a>🇺🇸 English (US)

<details>
<summary>Click to expand / collapse the Privacy Policy in English</summary><br>

# Privacy Policy — ACADEMIC EVALUATION SCHEDULE

**Last updated:** `November 22, 2026`  
**Version:** `2026.11.22.0`  
**Author:** Fernando Nillsson Cidade  

> [!IMPORTANT]
> **Independence from Microsoft:** Microsoft does **NOT** provide a default privacy policy for **ACADEMIC EVALUATION SCHEDULE**, and this Software is **NOT covered by Microsoft's Privacy Statement** ([microsoft.com/en-us/privacy/privacystatement](https://www.microsoft.com/en-us/privacy/privacystatement)). Embedded copy shipped inside [`source/assets/PRIVACY_POLICY/Privacy_Policy_en_US.txt`](../source/assets/PRIVACY_POLICY/Privacy_Policy_en_US.txt).

## 1. 100% Local-First & Offline Architecture

Zero remote telemetry, zero network requests, zero user accounts, and zero third-party analytics SDKs.

## 2. Local Persistence Layout (`%LOCALAPPDATA%\AgendaAvaliacoesAcademicas\`)

- `atividades.db`: scheduled assessments (`data`, `tipo`, `sequencia`, `nome`, `turma`);
- `listas.db`: user custom cascading lists per language (`pt_BR`/`en_US`);
- `config.json`: active color mode (`"coloridas"` / `"preto"`);
- `language.json`: selected UI language (`"pt_BR"` / `"en_US"`);
- `logs\file_agenda_*.log`: bounded local logs capped at `10` files;
- Exported PDFs are written only to paths explicitly chosen by the user.

## 3. User Control, Clean Uninstall & Contact

Full local data ownership (LGPD Art. 7 / GDPR), clean uninstallation via Windows Settings, and voluntary developer privacy contact at `linceu_lighthouse@outlook.com` (Microsoft provides no support).

</details>
