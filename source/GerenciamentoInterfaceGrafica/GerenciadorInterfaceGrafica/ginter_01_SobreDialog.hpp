#pragma once

#include <QDialog>
#include <QString>
#include <QStringList>
class QLabel;
class QPushButton;
class QTabWidget;
class QTextBrowser;

struct SobreDialogTextos {
    QString titulo;
    QString cabecalho_fixo;
    QString texto_history;
    QString detalhes;
    QString licencas;
    QString sites_licencas;
    QString avisos;
    QString privacy_policy;
    QString release_notes;
    QString show_history_text;
    QString hide_history_text;
    QString show_details_text;
    QString hide_details_text;
    QString show_licenses_text;
    QString hide_licenses_text;
    QString show_notices_text;
    QString hide_notices_text;
    QString show_privacy_policy_text;
    QString hide_privacy_policy_text;
    QString show_release_notes_text;
    QString hide_release_notes_text;
    QString ok_text;
    QString site_oficial_text;
    QString info_not_available_text;
};

class SobreDialog : public QDialog {
    Q_OBJECT
public:
    explicit SobreDialog(const SobreDialogTextos &texts, QWidget *parent = nullptr);
    void retranslate_ui(const SobreDialogTextos &texts);
private slots:
    void _on_tab_changed(int index);

private:
    void _configure_window_flags();
    QString _plain_to_html(const QString &text) const;
    void _set_browser_content(QTextBrowser *browser, const QString &content, const QString &info_text);
    void _update_tab_labels(int current_index);
    QLabel *fixed_label = nullptr;
    QTabWidget *tabs = nullptr;
    QTextBrowser *_history_browser = nullptr;
    QTextBrowser *_detalhes_browser = nullptr;
    QTextBrowser *_licencas_browser = nullptr;
    QTextBrowser *_avisos_browser = nullptr;
    QTextBrowser *_privacidade_browser = nullptr;
    QTextBrowser *_release_notes_browser = nullptr;
    QPushButton *ok_button = nullptr;
    QStringList _show_labels;
    QStringList _hide_labels;
    QString info_not_available_text;
};
