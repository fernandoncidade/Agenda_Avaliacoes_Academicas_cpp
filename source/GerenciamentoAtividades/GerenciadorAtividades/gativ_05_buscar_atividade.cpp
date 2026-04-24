#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"

std::optional<Atividade> GerenciamentoAtividades::buscar_atividade(const Atividade &atividade) {
    const QString chave = chave_atividade(atividade);
    if (atividades.contains(chave)) {
        return atividades.value(chave);
    }
    return std::nullopt;
}
