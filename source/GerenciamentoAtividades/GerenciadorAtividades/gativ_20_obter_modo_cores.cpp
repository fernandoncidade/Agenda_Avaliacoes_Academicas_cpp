#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"

QString GerenciamentoAtividades::obter_modo_cores() const {
    return modo_cores.isEmpty() ? QString("coloridas") : modo_cores;
}
