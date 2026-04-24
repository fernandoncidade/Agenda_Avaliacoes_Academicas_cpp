#include "find_VersionEditor.hpp"

#include <QApplication>
#include <QComboBox>
#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QSpinBox>
#include <QStringDecoder>
#include <QTextEdit>
#include <QVBoxLayout>

#ifndef AGENDA_SOURCE_DIR
#define AGENDA_SOURCE_DIR ""
#endif

namespace {
    struct FileSpec {
        QString key;
        QString relative_path;
        QStringList patterns;
    };

    struct CheckInfo {
        QString key;
        bool exists = false;
        QStringList found;
        QStringList missing;
    };

    struct UpdateResult {
        QString key;
        bool ok = false;
        QString message;
    };

    QStringList pt_months() {
        return {
            QStringLiteral("Janeiro"), QStringLiteral("Fevereiro"), QStringLiteral("Março"),
            QStringLiteral("Abril"),   QStringLiteral("Maio"),      QStringLiteral("Junho"),
            QStringLiteral("Julho"),   QStringLiteral("Agosto"),    QStringLiteral("Setembro"),
            QStringLiteral("Outubro"), QStringLiteral("Novembro"),  QStringLiteral("Dezembro"),
        };
    }

    QStringList pt_months_lower() {
        return {
            QStringLiteral("janeiro"), QStringLiteral("fevereiro"), QStringLiteral("março"),
            QStringLiteral("abril"),   QStringLiteral("maio"),      QStringLiteral("junho"),
            QStringLiteral("julho"),   QStringLiteral("agosto"),    QStringLiteral("setembro"),
            QStringLiteral("outubro"), QStringLiteral("novembro"),  QStringLiteral("dezembro"),
        };
    }

    QStringList en_months() {
        return {
            QStringLiteral("January"), QStringLiteral("February"), QStringLiteral("March"),
            QStringLiteral("April"),   QStringLiteral("May"),      QStringLiteral("June"),
            QStringLiteral("July"),    QStringLiteral("August"),   QStringLiteral("September"),
            QStringLiteral("October"), QStringLiteral("November"), QStringLiteral("December"),
        };
    }

    QString version_from_date(int year, int month_index, int day) {
        return QStringLiteral("%1.%2.%3.0")
            .arg(QString::number(year), QString::number(month_index + 1), QString::number(day));
    }

    QList<FileSpec> file_specs() {
        return {
            {QStringLiteral("privacy_pt"),
             QStringLiteral("source/assets/PRIVACY_POLICY/Privacy_Policy_pt_BR.txt"),
             {QStringLiteral(R"(^(Versão:\s*.*)$)"), QStringLiteral(R"(^(Última atualização:\s*.*)$)")}},
            {QStringLiteral("privacy_en"),
             QStringLiteral("source/assets/PRIVACY_POLICY/Privacy_Policy_en_US.txt"),
             {QStringLiteral(R"(^(Version:\s*.*)$)"), QStringLiteral(R"(^(Last updated:\s*.*)$)")}},
            {QStringLiteral("notice_pt"),
             QStringLiteral("source/assets/NOTICES/NOTICE_pt_BR.txt"),
             {QStringLiteral(R"(^(Versão.*)$)")}},
            {QStringLiteral("notice_en"),
             QStringLiteral("source/assets/NOTICES/NOTICE_en_US.txt"),
             {QStringLiteral(R"(^(Version.*)$)")}},
            {QStringLiteral("eula_pt"),
             QStringLiteral("source/assets/EULA/EULA_pt_BR.txt"),
             {QStringLiteral(R"(^(Versão.*)$)")}},
            {QStringLiteral("eula_en"),
             QStringLiteral("source/assets/EULA/EULA_en_US.txt"),
             {QStringLiteral(R"(^(Version.*)$)")}},
            {QStringLiteral("copyright_pt"),
             QStringLiteral("source/assets/COPYRIGHT/AVISO DE COPYRIGHT E MARCA REGISTRA_pt_BR.txt"),
             {QStringLiteral(R"(^(Versão.*)$)"), QStringLiteral(R"(^(Copyright.*)$)")}},
            {QStringLiteral("copyright_en"),
             QStringLiteral("source/assets/COPYRIGHT/COPYRIGHT AND TRADEMARK NOTICE_en_US.txt"),
             {QStringLiteral(R"(^(Version.*)$)"), QStringLiteral(R"(^(Copyright.*)$)")}},
            {QStringLiteral("clc_pt"),
             QStringLiteral("source/assets/CLC/CLC_pt_BR.txt"),
             {QStringLiteral(R"(^(Versão:\s*.*)$)"), QStringLiteral(R"(^(Data:\s*.*)$)")}},
            {QStringLiteral("clc_en"),
             QStringLiteral("source/assets/CLC/CLC_en_US.txt"),
             {QStringLiteral(R"(^(Version:\s*.*)$)"), QStringLiteral(R"(^(Date:\s*.*)$)")}},
            {QStringLiteral("about_pt"),
             QStringLiteral("source/assets/ABOUT/ABOUT_pt_BR.txt"),
             {QStringLiteral(R"(^(Versão:\s*.*)$)")}},
            {QStringLiteral("about_en"),
             QStringLiteral("source/assets/ABOUT/ABOUT_en_US.txt"),
             {QStringLiteral(R"(^(Version:\s*.*)$)")}},
            {QStringLiteral("readme_pt"),
             QStringLiteral("README.md"),
             {QStringLiteral(R"(^(Versão:\s*v[\d\.]+\s*)$)"),
              QStringLiteral(R"(^(Pequena nota:.*\(v[\d\.]+\).*)$)")}},
            {QStringLiteral("readme_en"),
             QStringLiteral("README.md"),
             {QStringLiteral(R"(^(Version:\s*v[\d\.]+\s*)$)"),
              QStringLiteral(R"(^(Small note:.*\(v[\d\.]+\).*)$)")}},
        };
    }

    QString clean_absolute_path(const QString &path) {
        return QFileInfo(QDir::cleanPath(path)).absoluteFilePath();
    }

    QString default_base_dir() {
        const QString configured_source_dir = QString::fromUtf8(AGENDA_SOURCE_DIR);
        if (!configured_source_dir.isEmpty() && QDir(configured_source_dir).exists()) {
            return clean_absolute_path(configured_source_dir);
        }

        QFileInfo source_file(QString::fromUtf8(__FILE__));
        QDir source_dir = source_file.absoluteDir();
        if (source_dir.dirName() == QStringLiteral("mocks") && source_dir.cdUp()) {
            if (QDir(source_dir.filePath(QStringLiteral("source/assets"))).exists()) {
                return source_dir.absolutePath();
            }
        }

        const QString current_path = QDir::currentPath();
        if (QDir(QDir(current_path).filePath(QStringLiteral("source/assets"))).exists()) {
            return clean_absolute_path(current_path);
        }

        return clean_absolute_path(QCoreApplication::applicationDirPath());
    }

    QString resolve_file_path(const QString &base_dir, const QString &relative_path) {
        QDir base(base_dir);
        const QString candidate = clean_absolute_path(base.filePath(relative_path));
        if (QFileInfo::exists(candidate)) {
            return candidate;
        }

        const QString source_prefix = QStringLiteral("source/");
        if (relative_path.startsWith(source_prefix)) {
            const QString without_source = relative_path.mid(source_prefix.size());
            const QString alternative = clean_absolute_path(base.filePath(without_source));
            if (QFileInfo::exists(alternative)) {
                return alternative;
            }
        }

        return candidate;
    }

    bool read_file(const QString &path, QString *text) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }

        const QByteArray data = file.readAll();
        QStringDecoder decoder(QStringDecoder::Utf8);
        QString decoded = decoder.decode(data);
        if (decoder.hasError()) {
            decoded = QString::fromLatin1(data);
        }

        *text = decoded;
        return true;
    }

    bool write_file(const QString &path, const QString &text, QString *error_message) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            *error_message = file.errorString();
            return false;
        }

        const qint64 written = file.write(text.toUtf8());
        if (written < 0) {
            *error_message = file.errorString();
            return false;
        }

        return true;
    }

    QList<CheckInfo> check_expected_lines(const QString &base_dir) {
        QList<CheckInfo> status;
        for (const FileSpec &spec : file_specs()) {
            CheckInfo info;
            info.key = spec.key;

            const QString path = resolve_file_path(base_dir, spec.relative_path);
            QString text;
            if (!read_file(path, &text)) {
                status.append(info);
                continue;
            }

            info.exists = true;
            for (const QString &pattern : spec.patterns) {
                const QRegularExpression regex(pattern, QRegularExpression::MultilineOption);
                const QRegularExpressionMatch match = regex.match(text);
                if (match.hasMatch()) {
                    const QString captured =
                        match.lastCapturedIndex() >= 1 ? match.captured(1).trimmed() : match.captured(0).trimmed();
                    info.found.append(captured);
                } else {
                    info.missing.append(pattern);
                }
            }

            status.append(info);
        }

        return status;
    }

    bool replace_with_prefix(QString *text, const QString &pattern, const QString &value) {
        const QRegularExpression regex(pattern, QRegularExpression::MultilineOption);
        const QRegularExpressionMatch match = regex.match(*text);
        if (!match.hasMatch()) {
            return false;
        }

        text->replace(match.capturedStart(0), match.capturedLength(0), match.captured(1) + value);
        return true;
    }

    bool replace_whole_line(QString *text, const QString &pattern, const QString &value) {
        const QRegularExpression regex(pattern, QRegularExpression::MultilineOption);
        const QRegularExpressionMatch match = regex.match(*text);
        if (!match.hasMatch()) {
            return false;
        }

        text->replace(match.capturedStart(0), match.capturedLength(0), value);
        return true;
    }

    bool replace_capture(QString *text, const QString &pattern, int capture_index, const QString &value) {
        const QRegularExpression regex(pattern, QRegularExpression::MultilineOption);
        const QRegularExpressionMatch match = regex.match(*text);
        if (!match.hasMatch()) {
            return false;
        }

        const int start = match.capturedStart(capture_index);
        const int length = match.capturedLength(capture_index);
        if (start < 0 || length < 0) {
            return false;
        }

        text->replace(start, length, value);
        return true;
    }

    QList<UpdateResult> apply_updates(const QString &pt_version, int pt_day, int pt_month_index, int pt_year,
                                      const QString &en_version, int en_day, int en_month_index, int en_year,
                                      const QString &base_dir) {
        QList<UpdateResult> results;

        const QString pt_month_name = pt_months_lower().value(pt_month_index);
        const QString pt_date =
            QStringLiteral("%1 de %2 de %3").arg(QString::number(pt_day), pt_month_name, QString::number(pt_year));
        const QString en_month_name = en_months().value(en_month_index);
        const QString en_date =
            QStringLiteral("%1 %2, %3").arg(en_month_name, QString::number(en_day), QString::number(en_year));

        for (const FileSpec &spec : file_specs()) {
            UpdateResult result;
            result.key = spec.key;

            const QString path = resolve_file_path(base_dir, spec.relative_path);
            QString text;
            if (!read_file(path, &text)) {
                result.message = QStringLiteral("arquivo inexistente");
                results.append(result);
                continue;
            }

            const QString original = text;
            bool matched = false;

            if (spec.key == QStringLiteral("privacy_pt")) {
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Versão:\s*).*$)"), pt_version);
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Última atualização:\s*).*$)"), pt_date);
            } else if (spec.key == QStringLiteral("privacy_en")) {
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Version:\s*).*$)"), en_version);
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Last updated:\s*).*$)"), en_date);
            } else if (spec.key == QStringLiteral("notice_pt") || spec.key == QStringLiteral("eula_pt") ||
                       spec.key == QStringLiteral("copyright_pt")) {
                matched |= replace_whole_line(&text, QStringLiteral(R"(^(Versão\s*)([\d\.]+)\s*,\s*.*$)"),
                                              QStringLiteral("Versão %1, %2").arg(pt_version, pt_date));
            } else if (spec.key == QStringLiteral("notice_en") || spec.key == QStringLiteral("eula_en") ||
                       spec.key == QStringLiteral("copyright_en")) {
                matched |= replace_whole_line(&text, QStringLiteral(R"(^(Version\s*)([\d\.]+)\s*,\s*.*$)"),
                                              QStringLiteral("Version %1, %2").arg(en_version, en_date));
            } else if (spec.key == QStringLiteral("clc_pt")) {
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Versão:\s*).*$)"), pt_version);
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Data:\s*).*$)"), pt_date);
            } else if (spec.key == QStringLiteral("clc_en")) {
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Version:\s*).*$)"), en_version);
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Date:\s*).*$)"), en_date);
            } else if (spec.key == QStringLiteral("about_pt")) {
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Versão:\s*).*$)"), pt_version);
            } else if (spec.key == QStringLiteral("about_en")) {
                matched |= replace_with_prefix(&text, QStringLiteral(R"(^(Version:\s*).*$)"), en_version);
            } else if (spec.key == QStringLiteral("readme_pt")) {
                matched |= replace_capture(&text, QStringLiteral(R"(^(Versão:\s*v)([\d\.]+)(\s*)$)"), 2, pt_version);
                matched |= replace_capture(
                    &text, QStringLiteral(R"(^(Pequena nota:.*\(v)([\d\.]+)(\).*)$)"), 2, pt_version);
            } else if (spec.key == QStringLiteral("readme_en")) {
                matched |= replace_capture(&text, QStringLiteral(R"(^(Version:\s*v)([\d\.]+)(\s*)$)"), 2, en_version);
                matched |= replace_capture(
                    &text, QStringLiteral(R"(^(Small note:.*\(v)([\d\.]+)(\).*)$)"), 2, en_version);
            }

            if (!matched || text == original) {
                result.message = QStringLiteral("padrões não encontrados / sem alteração");
                results.append(result);
                continue;
            }

            QString error_message;
            if (write_file(path, text, &error_message)) {
                result.ok = true;
                result.message = QStringLiteral("atualizado");
            } else {
                result.message = QStringLiteral("erro escrita: %1").arg(error_message);
            }

            results.append(result);
        }

        return results;
    }
} // namespace

VersionEditor::VersionEditor(QWidget *parent) : QWidget(parent), selected_base_dir(default_base_dir()) {
    setWindowTitle(QStringLiteral("Editor de Versões / Datas - Agenda (PT/EN)"));
    resize(820, 520);

    const QDate current_date = QDate::currentDate();
    const int current_day = current_date.day();
    const int current_month_index = current_date.month() - 1;
    const int current_year = current_date.year();
    const QString current_version = version_from_date(current_year, current_month_index, current_day);

    const QString icon_path =
        QDir(selected_base_dir).filePath(QStringLiteral("source/assets/icones/ReviewsManager.ico"));
    if (QFileInfo::exists(icon_path)) {
        setWindowIcon(QIcon(icon_path));
    }

    auto *layout = new QVBoxLayout(this);

    auto *folder_layout = new QHBoxLayout();
    folder_layout->addWidget(new QLabel(QStringLiteral("Pasta dos arquivos:"), this));

    folder_path = new QLineEdit(selected_base_dir, this);
    folder_path->setReadOnly(true);
    folder_layout->addWidget(folder_path);

    auto *select_folder_btn = new QPushButton(QStringLiteral("Selecionar..."), this);
    connect(select_folder_btn, &QPushButton::clicked, this, &VersionEditor::on_select_folder);
    folder_layout->addWidget(select_folder_btn);
    layout->addLayout(folder_layout);

    auto *pt_group = new QGroupBox(QStringLiteral("Português (pt-BR)"), this);
    auto *pt_layout = new QHBoxLayout(pt_group);
    pt_layout->addWidget(new QLabel(QStringLiteral("Versão:"), pt_group));
    pt_version = new QLineEdit(current_version, pt_group);
    pt_layout->addWidget(pt_version);
    pt_layout->addWidget(new QLabel(QStringLiteral("Dia:"), pt_group));
    pt_day = new QSpinBox(pt_group);
    pt_day->setRange(1, 31);
    pt_day->setValue(current_day);
    pt_layout->addWidget(pt_day);
    pt_layout->addWidget(new QLabel(QStringLiteral("Mês:"), pt_group));
    pt_month = new QComboBox(pt_group);
    pt_month->addItems(pt_months());
    pt_month->setCurrentIndex(current_month_index);
    pt_layout->addWidget(pt_month);
    pt_layout->addWidget(new QLabel(QStringLiteral("Ano:"), pt_group));
    pt_year = new QSpinBox(pt_group);
    pt_year->setRange(1900, 3000);
    pt_year->setValue(current_year);
    pt_layout->addWidget(pt_year);
    layout->addWidget(pt_group);

    auto *en_group = new QGroupBox(QStringLiteral("English (en-US)"), this);
    auto *en_layout = new QHBoxLayout(en_group);
    en_layout->addWidget(new QLabel(QStringLiteral("Version:"), en_group));
    en_version = new QLineEdit(current_version, en_group);
    en_layout->addWidget(en_version);
    en_layout->addWidget(new QLabel(QStringLiteral("Day:"), en_group));
    en_day = new QSpinBox(en_group);
    en_day->setRange(1, 31);
    en_day->setValue(current_day);
    en_layout->addWidget(en_day);
    en_layout->addWidget(new QLabel(QStringLiteral("Month:"), en_group));
    en_month = new QComboBox(en_group);
    en_month->addItems(en_months());
    en_month->setCurrentIndex(current_month_index);
    en_layout->addWidget(en_month);
    en_layout->addWidget(new QLabel(QStringLiteral("Year:"), en_group));
    en_year = new QSpinBox(en_group);
    en_year->setRange(1900, 3000);
    en_year->setValue(current_year);
    en_layout->addWidget(en_year);
    layout->addWidget(en_group);

    auto *button_layout = new QHBoxLayout();
    auto *check_btn = new QPushButton(QStringLiteral("Verificar arquivos"), this);
    connect(check_btn, &QPushButton::clicked, this, &VersionEditor::on_check);
    button_layout->addWidget(check_btn);

    auto *save_btn = new QPushButton(QStringLiteral("Salvar alterações"), this);
    connect(save_btn, &QPushButton::clicked, this, &VersionEditor::on_save);
    button_layout->addWidget(save_btn);

    auto *refresh_btn = new QPushButton(QStringLiteral("Recarregar verificação"), this);
    connect(refresh_btn, &QPushButton::clicked, this, &VersionEditor::on_check);
    button_layout->addWidget(refresh_btn);
    layout->addLayout(button_layout);

    status = new QTextEdit(this);
    status->setReadOnly(true);
    layout->addWidget(status);

    on_check();
}

void VersionEditor::on_select_folder() {
    const QString dir_path = QFileDialog::getExistingDirectory(
        this, QStringLiteral("Selecione a pasta onde os arquivos estão"), selected_base_dir);
    if (dir_path.isEmpty()) {
        return;
    }

    selected_base_dir = clean_absolute_path(dir_path);
    folder_path->setText(selected_base_dir);
    on_check();
}

void VersionEditor::on_check() {
    QStringList lines;
    lines.append(QStringLiteral("Base: %1").arg(selected_base_dir));

    for (const CheckInfo &info : check_expected_lines(selected_base_dir)) {
        lines.append(QStringLiteral("%1: arquivo %2")
                         .arg(info.key, info.exists ? QStringLiteral("existe") : QStringLiteral("ausente")));
        if (info.exists) {
            if (!info.found.isEmpty()) {
                lines.append(QStringLiteral("  encontrado:"));
                for (const QString &found : info.found) {
                    lines.append(QStringLiteral("    - %1").arg(found));
                }
            }
            if (!info.missing.isEmpty()) {
                lines.append(QStringLiteral("  ausente:"));
                for (const QString &missing : info.missing) {
                    lines.append(QStringLiteral("    - %1").arg(missing));
                }
            }
        }
        lines.append(QString());
    }

    status->setPlainText(lines.join(QChar::LineFeed));
}

void VersionEditor::on_save() {
    const QList<UpdateResult> results = apply_updates(
        pt_version->text().trimmed(), pt_day->value(), pt_month->currentIndex(), pt_year->value(),
        en_version->text().trimmed(), en_day->value(), en_month->currentIndex(), en_year->value(), selected_base_dir);

    QStringList lines;
    lines.append(QStringLiteral("Resultados:"));
    for (const UpdateResult &result : results) {
        lines.append(QStringLiteral("%1: %2 - %3")
                         .arg(result.key, result.ok ? QStringLiteral("OK") : QStringLiteral("FALHA"), result.message));
    }

    status->setPlainText(lines.join(QChar::LineFeed));
}

#ifdef AGENDA_BUILD_VERSION_EDITOR_STANDALONE
int main(int argc, char **argv) {
    QApplication app(argc, argv);
    VersionEditor editor;
    editor.show();
    return app.exec();
}
#endif
