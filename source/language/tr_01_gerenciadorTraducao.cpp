#include "tr_01_gerenciadorTraducao.hpp"

#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>

QString obter_diretorio_traducoes() {
    const QString appDir = QCoreApplication::applicationDirPath();
    const QString current = QDir::currentPath();
    const QStringList candidatos = {appDir + "/source/language/translations",
                                    appDir + "/language/translations",
                                    appDir + "/_internal/source/language/translations",
                                    appDir + "/_internal/language/translations",
                                    current + "/source/language/translations",
                                    QDir(current).absoluteFilePath("../source/language/translations")};
    for (const QString &caminho : candidatos) {
        if (QDir(caminho).exists()) {
            return QDir(caminho).absolutePath();
        }
    }
    return QDir(candidatos.first()).absolutePath();
}

GerenciadorTraducao::GerenciadorTraducao(QObject *parent) : QObject(parent) {
    idiomas_disponiveis.insert("pt_BR", "Português (Brasil)");
    idiomas_disponiveis.insert("en_US", "English (United States)");
    dir_traducoes = obter_diretorio_traducoes();
    QDir().mkpath(dir_traducoes);
    carregar_configuracao_idioma();
}

void GerenciadorTraducao::carregar_configuracao_idioma() {
    QFile file(obter_caminho_configuracao());
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    const QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
    const QString idioma = obj.value("idioma").toString();
    if (idiomas_disponiveis.contains(idioma)) {
        idioma_atual = idioma;
    }
}

void GerenciadorTraducao::salvar_configuracao_idioma() const {
    QFile file(obter_caminho_configuracao());
    QDir().mkpath(QFileInfo(file).absolutePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        LogManager::error("Erro ao salvar configuracao de idioma");
        return;
    }
    QJsonObject obj;
    obj.insert("idioma", idioma_atual);
    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
}

QString GerenciadorTraducao::obter_caminho_configuracao() const {
    return QDir(obter_caminho_persistente()).filePath("language.json");
}

void GerenciadorTraducao::_remover_tradutor_instalado() {
    QCoreApplication *app = QCoreApplication::instance();
    if (app && tradutor) {
        app->removeTranslator(tradutor);
    }
    delete tradutor;
    tradutor = nullptr;
}

bool GerenciadorTraducao::aplicar_traducao() {
    QCoreApplication *app = QCoreApplication::instance();
    if (!app) {
        return false;
    }
    _remover_tradutor_instalado();
    if (idioma_atual == idioma_padrao) {
        return true;
    }
    tradutor = new QTranslator(this);
    const QString arquivo = QString("manager_compression_%1.qm").arg(idioma_atual);
    const QString caminho = QDir(dir_traducoes).filePath(arquivo);
    if (QFile::exists(caminho) && tradutor->load(caminho)) {
        app->installTranslator(tradutor);
        return true;
    }
    LogManager::warning(QString("Arquivo de traducao nao encontrado ou invalido: %1").arg(caminho));
    _remover_tradutor_instalado();
    return false;
}

bool GerenciadorTraducao::definir_idioma(const QString &codigo_idioma) {
    if (!idiomas_disponiveis.contains(codigo_idioma)) {
        return false;
    }
    idioma_atual = codigo_idioma;
    salvar_configuracao_idioma();
    const bool resultado = aplicar_traducao();
    emit idioma_alterado(codigo_idioma);
    return resultado;
}

QString GerenciadorTraducao::obter_idioma_atual() const {
    return idioma_atual;
}

void GerenciadorTraducao::traduzir_botoes_padrao(QDialogButtonBox *dialogo) {
    if (!dialogo) {
        return;
    }
    if (auto *botao = dialogo->button(QDialogButtonBox::Ok))
        botao->setText(QCoreApplication::translate("Dialog", "OK"));
    if (auto *botao = dialogo->button(QDialogButtonBox::Cancel))
        botao->setText(QCoreApplication::translate("Dialog", "Cancelar"));
    if (auto *botao = dialogo->button(QDialogButtonBox::Yes))
        botao->setText(QCoreApplication::translate("Dialog", "Sim"));
    if (auto *botao = dialogo->button(QDialogButtonBox::No))
        botao->setText(QCoreApplication::translate("Dialog", "Não"));
}
