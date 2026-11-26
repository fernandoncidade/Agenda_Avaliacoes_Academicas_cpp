#pragma once

#include <QHash>
#include <QObject>
#include <QString>
#include <QTranslator>
QString obter_diretorio_traducoes();

class GerenciadorTraducao : public QObject {
    Q_OBJECT
public:
    explicit GerenciadorTraducao(QObject *parent = nullptr);
    bool aplicar_traducao();
    bool definir_idioma(const QString &codigo_idioma);
    QString obter_idioma_atual() const;
    void traduzir_botoes_padrao(class QDialogButtonBox *dialogo);
    QHash<QString, QString> idiomas_disponiveis;
signals:
    void idioma_alterado(const QString &codigo_idioma);

private:
    void carregar_configuracao_idioma();
    void salvar_configuracao_idioma() const;
    QString obter_caminho_configuracao() const;
    void _remover_tradutor_instalado();
    QTranslator *tradutor = nullptr;
    QString idioma_atual = "pt_BR";
    QString idioma_padrao = "pt_BR";
    QString dir_traducoes;
};
