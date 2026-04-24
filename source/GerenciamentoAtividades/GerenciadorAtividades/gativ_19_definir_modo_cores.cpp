#include "source/GerenciamentoAtividades/Gerenciador_Atividades.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

bool GerenciamentoAtividades::definir_modo_cores(const QString &modo) {
    if (modo != "preto" && modo != "coloridas") {
        return false;
    }
    modo_cores = modo;
    QFile file(QDir(obter_caminho_persistente()).filePath("config.json"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonObject obj;
        obj.insert("modo_cores", modo_cores);
        file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    }
    return true;
}
