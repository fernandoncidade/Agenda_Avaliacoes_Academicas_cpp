#include "ginter_01_SobreDialog.hpp"

#include "source/utils/LogManager.hpp"

#include <QDesktopServices>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRegularExpression>
#include <QTabWidget>
#include <QTextBrowser>
#include <QUrl>
#include <QVBoxLayout>

SobreDialog::SobreDialog(const SobreDialogTextos &texts, QWidget *parent) : QDialog(parent) {
    _configure_window_flags();
    auto *mainLayout = new QVBoxLayout(this);
    fixed_label = new QLabel(this);
    fixed_label->setWordWrap(true);
    fixed_label->setOpenExternalLinks(true);
    mainLayout->addWidget(fixed_label);
    tabs = new QTabWidget(this);
    _history_browser = new QTextBrowser(this);
    _detalhes_browser = new QTextBrowser(this);
    _licencas_browser = new QTextBrowser(this);
    _avisos_browser = new QTextBrowser(this);
    _privacidade_browser = new QTextBrowser(this);
    _release_notes_browser = new QTextBrowser(this);
    for (QTextBrowser *browser : {_history_browser, _detalhes_browser, _licencas_browser, _avisos_browser,
                                  _privacidade_browser, _release_notes_browser}) {
        browser->setOpenExternalLinks(true);
    }
    tabs->addTab(_history_browser, QString());
    tabs->addTab(_detalhes_browser, QString());
    tabs->addTab(_licencas_browser, QString());
    tabs->addTab(_avisos_browser, QString());
    tabs->addTab(_privacidade_browser, QString());
    tabs->addTab(_release_notes_browser, QString());
    connect(tabs, &QTabWidget::currentChanged, this, &SobreDialog::_on_tab_changed);
    mainLayout->addWidget(tabs);
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    ok_button = new QPushButton(this);
    connect(ok_button, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(ok_button);
    mainLayout->addLayout(buttonLayout);
    resize(800, 650);
    setMinimumSize(400, 200);
    retranslate_ui(texts);
}

void SobreDialog::_configure_window_flags() {
    setWindowFlag(Qt::WindowMinimizeButtonHint, true);
    setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

QString SobreDialog::_plain_to_html(const QString &text) const {
    if (text.isEmpty())
        return QString();
    QString escaped = text.toHtmlEscaped();
    escaped.replace(QRegularExpression("(https?://[^\\s<]+)"), "<a href=\"\\1\">\\1</a>");
    const QStringList lines = escaped.split('\n');
    QString html;
    bool inList = false;
    QStringList paragraph;
    auto flushParagraph = [&]() {
        if (!paragraph.isEmpty()) {
            html += "<p>" + paragraph.join("<br>") + "</p>";
            paragraph.clear();
        }
    };
    for (const QString &line : lines) {
        const QString stripped = line.trimmed();
        if (stripped.isEmpty()) {
            if (inList) {
                html += "</ul>";
                inList = false;
            }
            flushParagraph();
            continue;
        }
        if (stripped.startsWith("- ") || stripped.startsWith("* ")) {
            flushParagraph();
            if (!inList) {
                html += "<ul>";
                inList = true;
            }
            html += "<li>" + stripped.mid(2) + "</li>";
        } else {
            if (inList) {
                html += "</ul>";
                inList = false;
            }
            paragraph << line;
        }
    }
    if (inList)
        html += "</ul>";
    flushParagraph();
    return html.isEmpty() ? "<p></p>" : html;
}

void SobreDialog::_set_browser_content(QTextBrowser *browser, const QString &content, const QString &info_text) {
    if (!browser)
        return;
    if (content.trimmed().isEmpty()) {
        browser->setHtml(QString("<p>%1.</p>").arg(info_text.toHtmlEscaped()));
        return;
    }
    if (content.contains(QRegularExpression("<\\s*(a|p|ul|ol|li|br|b|i|strong|em|div|h[1-6])\\b",
                                            QRegularExpression::CaseInsensitiveOption))) {
        browser->setHtml(content);
    } else {
        browser->setHtml(_plain_to_html(content));
    }
}

void SobreDialog::_on_tab_changed(int index) {
    _update_tab_labels(index);
}

void SobreDialog::_update_tab_labels(int current_index) {
    for (int i = 0; i < tabs->count(); ++i) {
        const QString label = (i == current_index && i < _hide_labels.size() && !_hide_labels[i].isEmpty())
                                  ? _hide_labels[i]
                                  : (i < _show_labels.size() ? _show_labels[i] : tabs->tabText(i));
        tabs->setTabText(i, label);
    }
}

void SobreDialog::retranslate_ui(const SobreDialogTextos &texts) {
    info_not_available_text = texts.info_not_available_text.isEmpty() ? QStringLiteral("Informação não disponível")
                                                                      : texts.info_not_available_text;
    setWindowTitle(texts.titulo);
    fixed_label->setText(QString("<div style='margin:0;padding:0;'>%1</div>").arg(texts.cabecalho_fixo));
    _set_browser_content(_history_browser, texts.texto_history, info_not_available_text);
    _set_browser_content(_detalhes_browser, texts.detalhes, info_not_available_text);
    if (!texts.licencas.isEmpty()) {
        QString licencasHtml = _plain_to_html(texts.licencas);
        licencasHtml += QString("<br><br><h3>%1</h3><ul>").arg(texts.site_oficial_text.toHtmlEscaped());
        for (const QString &site : texts.sites_licencas.split('\n')) {
            const QString s = site.trimmed();
            if (!s.isEmpty())
                licencasHtml += QString("<li><a href=\"%1\">%1</a></li>").arg(s.toHtmlEscaped());
        }
        licencasHtml += "</ul>";
        _licencas_browser->setHtml(licencasHtml);
    } else {
        _licencas_browser->setHtml(QString("<p>%1.</p>").arg(info_not_available_text.toHtmlEscaped()));
    }
    _set_browser_content(_avisos_browser, texts.avisos, info_not_available_text);
    _set_browser_content(_privacidade_browser, texts.privacy_policy, info_not_available_text);
    _set_browser_content(_release_notes_browser, texts.release_notes, info_not_available_text);
    ok_button->setText(texts.ok_text);
    _show_labels = {texts.show_history_text, texts.show_details_text,        texts.show_licenses_text,
                    texts.show_notices_text, texts.show_privacy_policy_text, texts.show_release_notes_text};
    _hide_labels = {texts.hide_history_text, texts.hide_details_text,        texts.hide_licenses_text,
                    texts.hide_notices_text, texts.hide_privacy_policy_text, texts.hide_release_notes_text};
    _update_tab_labels(tabs->currentIndex());
}
