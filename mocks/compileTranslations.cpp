#include "compileTranslations.hpp"

#include <QApplication>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QStandardPaths>
#include <QTextStream>
#include <QTextEdit>
#include <QVBoxLayout>

namespace {
struct CompilationResult {
    bool success = false;
    int exitCode = -1;
    QString output;
};

QString normalized_path(const QString &path) {
    return QDir::fromNativeSeparators(path);
}

QString configured_language_dir() {
#ifdef AGENDA_LANGUAGE_DIR
    return normalized_path(QString::fromUtf8(AGENDA_LANGUAGE_DIR));
#else
    return {};
#endif
}

QString configured_lrelease_path() {
#ifdef AGENDA_LRELEASE_EXECUTABLE
    return normalized_path(QString::fromUtf8(AGENDA_LRELEASE_EXECUTABLE));
#else
    return {};
#endif
}

QString default_language_dir() {
    const QString configured = configured_language_dir();
    if (!configured.isEmpty() && QDir(configured).exists()) {
        return configured;
    }

    const QDir appDir(QCoreApplication::applicationDirPath());
    const QString deployedLanguageDir = appDir.filePath("source/language");
    if (QDir(deployedLanguageDir).exists()) {
        return deployedLanguageDir;
    }

    const QDir currentDir(QDir::currentPath());
    if (currentDir.exists("translations")) {
        return currentDir.absolutePath();
    }

    return appDir.absolutePath();
}

QString default_translations_dir() {
    return QDir(default_language_dir()).filePath("translations");
}

QString find_lrelease() {
    const QString configured = configured_lrelease_path();
    if (!configured.isEmpty() && QFileInfo::exists(configured)) {
        return configured;
    }

    QString executable = QStandardPaths::findExecutable("lrelease");
    if (executable.isEmpty()) {
        executable = QStandardPaths::findExecutable("lrelease.exe");
    }

    return normalized_path(executable);
}

QString qm_path_for_ts(const QString &tsFile) {
    const QFileInfo tsInfo(tsFile);
    return tsInfo.dir().filePath(tsInfo.completeBaseName() + ".qm");
}

CompilationResult compile_translation_file(const QString &lreleasePath, const QString &tsFile) {
    CompilationResult result;
    const QString qmFile = qm_path_for_ts(tsFile);

    QProcess process;
    process.setProgram(lreleasePath);
    process.setArguments({tsFile, "-qm", qmFile});
    process.start();

    if (!process.waitForStarted(10000)) {
        result.output = "Nao foi possivel iniciar lrelease: " + process.errorString();
        return result;
    }

    process.waitForFinished(-1);
    result.exitCode = process.exitCode();
    result.output = QString::fromLocal8Bit(process.readAllStandardOutput());
    const QString errorOutput = QString::fromLocal8Bit(process.readAllStandardError());
    if (!errorOutput.isEmpty()) {
        if (!result.output.isEmpty()) {
            result.output += '\n';
        }
        result.output += errorOutput;
    }

    result.success = process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0;
    return result;
}
} // namespace

CompileTranslationsWindow::CompileTranslationsWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("CompileTranslations");
    setMinimumSize(760, 520);

    auto *mainLayout = new QVBoxLayout(this);

    auto *directoryLayout = new QHBoxLayout();
    auto *directoryLabel = new QLabel("Pasta:", this);
    directoryEdit = new QLineEdit(default_translations_dir(), this);
    selectDirectoryButton = new QPushButton("Selecionar", this);
    refreshButton = new QPushButton("Atualizar", this);

    directoryLayout->addWidget(directoryLabel);
    directoryLayout->addWidget(directoryEdit, 1);
    directoryLayout->addWidget(selectDirectoryButton);
    directoryLayout->addWidget(refreshButton);
    mainLayout->addLayout(directoryLayout);

    fileList = new QListWidget(this);
    fileList->setAlternatingRowColors(true);
    mainLayout->addWidget(fileList, 1);

    auto *buttonLayout = new QHBoxLayout();
    compileCheckedButton = new QPushButton("Compilar marcados", this);
    compileAllButton = new QPushButton("Compilar todos", this);
    statusLabel = new QLabel(this);

    buttonLayout->addWidget(compileCheckedButton);
    buttonLayout->addWidget(compileAllButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);

    logOutput = new QTextEdit(this);
    logOutput->setReadOnly(true);
    logOutput->setMinimumHeight(170);
    mainLayout->addWidget(logOutput);

    connect(selectDirectoryButton, &QPushButton::clicked, this, [this]() {
        selectTranslationsDirectory();
    });
    connect(refreshButton, &QPushButton::clicked, this, [this]() {
        refreshFiles();
    });
    connect(compileCheckedButton, &QPushButton::clicked, this, [this]() {
        compileCheckedFiles();
    });
    connect(compileAllButton, &QPushButton::clicked, this, [this]() {
        compileAllFiles();
    });

    refreshFiles();
}

void CompileTranslationsWindow::selectTranslationsDirectory() {
    const QString selectedDir = QFileDialog::getExistingDirectory(
        this,
        "Selecionar pasta de traducoes",
        directoryEdit->text()
    );

    if (selectedDir.isEmpty()) {
        return;
    }

    directoryEdit->setText(normalized_path(selectedDir));
    refreshFiles();
}

void CompileTranslationsWindow::refreshFiles() {
    fileList->clear();

    const QDir translationsDir(directoryEdit->text());
    if (!translationsDir.exists()) {
        statusLabel->setText("Pasta inexistente");
        appendLog("Pasta inexistente: " + translationsDir.absolutePath());
        return;
    }

    const QStringList files = translationsDir.entryList({"*.ts"}, QDir::Files, QDir::Name);
    for (const QString &fileName : files) {
        auto *item = new QListWidgetItem(fileName, fileList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        item->setData(Qt::UserRole, translationsDir.filePath(fileName));
    }

    statusLabel->setText(QString::number(files.size()) + " arquivo(s) .ts");
    appendLog("Lista atualizada: " + translationsDir.absolutePath());
}

void CompileTranslationsWindow::compileCheckedFiles() {
    compileFiles(checkedTranslationFiles());
}

void CompileTranslationsWindow::compileAllFiles() {
    QStringList files;
    files.reserve(fileList->count());

    for (int i = 0; i < fileList->count(); ++i) {
        files.append(fileList->item(i)->data(Qt::UserRole).toString());
    }

    compileFiles(files);
}

void CompileTranslationsWindow::compileFiles(const QStringList &translationFiles) {
    if (translationFiles.isEmpty()) {
        QMessageBox::information(this, "CompileTranslations", "Nenhum arquivo .ts selecionado.");
        return;
    }

    const QString lreleasePath = find_lrelease();
    if (lreleasePath.isEmpty()) {
        QMessageBox::critical(this, "CompileTranslations", "lrelease nao foi encontrado no Qt ou no PATH.");
        return;
    }

    setControlsEnabled(false);
    appendLog("lrelease: " + lreleasePath);

    int successCount = 0;
    for (const QString &tsFile : translationFiles) {
        const QString qmFile = qm_path_for_ts(tsFile);
        appendLog("Compilando: " + tsFile);
        appendLog("Destino: " + qmFile);

        const CompilationResult result = compile_translation_file(lreleasePath, tsFile);
        if (!result.output.trimmed().isEmpty()) {
            appendLog(result.output.trimmed());
        }

        if (result.success) {
            ++successCount;
            appendLog("OK: " + QFileInfo(qmFile).fileName());
        } else {
            appendLog("Falha: codigo " + QString::number(result.exitCode));
        }

        QCoreApplication::processEvents();
    }

    setControlsEnabled(true);
    statusLabel->setText(QString::number(successCount) + "/" + QString::number(translationFiles.size()) + " compilado(s)");
}

void CompileTranslationsWindow::appendLog(const QString &message) {
    const QString time = QDateTime::currentDateTime().toString("HH:mm:ss");
    logOutput->append("[" + time + "] " + message);
}

void CompileTranslationsWindow::setControlsEnabled(bool enabled) {
    directoryEdit->setEnabled(enabled);
    selectDirectoryButton->setEnabled(enabled);
    refreshButton->setEnabled(enabled);
    compileCheckedButton->setEnabled(enabled);
    compileAllButton->setEnabled(enabled);
    fileList->setEnabled(enabled);
}

QStringList CompileTranslationsWindow::checkedTranslationFiles() const {
    QStringList files;

    for (int i = 0; i < fileList->count(); ++i) {
        const QListWidgetItem *item = fileList->item(i);
        if (item->checkState() == Qt::Checked) {
            files.append(item->data(Qt::UserRole).toString());
        }
    }

    return files;
}

void compilar_traducoes() {
    QTextStream out(stdout);
    const QDir dir(default_translations_dir());
    const QString lreleasePath = find_lrelease();

    if (!dir.exists()) {
        out << "Pasta de traducoes nao encontrada: " << dir.absolutePath() << Qt::endl;
        return;
    }

    if (lreleasePath.isEmpty()) {
        out << "lrelease nao foi encontrado." << Qt::endl;
        return;
    }

    const QStringList files = dir.entryList({"*.ts"}, QDir::Files, QDir::Name);
    for (const QString &fileName : files) {
        const QString tsFile = dir.filePath(fileName);
        const CompilationResult result = compile_translation_file(lreleasePath, tsFile);
        out << "Compilando: " << fileName << Qt::endl;
        if (!result.output.trimmed().isEmpty()) {
            out << result.output.trimmed() << Qt::endl;
        }
        out << (result.success ? "OK" : "Falha") << Qt::endl;
    }
}

#ifdef AGENDA_BUILD_COMPILE_TRANSLATIONS_STANDALONE
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("CompileTranslations");
    QApplication::setOrganizationName("AgendaAvaliacoesAcademicas");

    CompileTranslationsWindow window;
    window.show();

    return app.exec();
}
#endif
