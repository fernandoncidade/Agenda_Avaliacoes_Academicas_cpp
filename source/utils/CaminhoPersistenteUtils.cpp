#include "CaminhoPersistenteUtils.hpp"

#include "LogManager.hpp"

#include <QDir>

QString obter_caminho_persistente() {
    QString base = qEnvironmentVariable("LOCALAPPDATA");
    if (base.trimmed().isEmpty()) {
        base = QDir::homePath() + "/AppData/Local";
    }
    QDir dir(base + "/AgendaAvaliacoesAcademicas");
    if (!dir.exists() && !dir.mkpath(".")) {
        LogManager::error(QString("Erro ao criar diretorio de configuracao: %1").arg(dir.absolutePath()));
    }
    return dir.absolutePath();
}
