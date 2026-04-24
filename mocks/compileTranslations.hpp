#pragma once

#include <QStringList>
#include <QWidget>

class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QTextEdit;

class CompileTranslationsWindow : public QWidget {
public:
    explicit CompileTranslationsWindow(QWidget *parent = nullptr);

private:
    void selectTranslationsDirectory();
    void refreshFiles();
    void compileCheckedFiles();
    void compileAllFiles();
    void compileFiles(const QStringList &translationFiles);
    void appendLog(const QString &message);
    void setControlsEnabled(bool enabled);
    QStringList checkedTranslationFiles() const;

    QLineEdit *directoryEdit = nullptr;
    QListWidget *fileList = nullptr;
    QTextEdit *logOutput = nullptr;
    QLabel *statusLabel = nullptr;
    QPushButton *selectDirectoryButton = nullptr;
    QPushButton *refreshButton = nullptr;
    QPushButton *compileCheckedButton = nullptr;
    QPushButton *compileAllButton = nullptr;
};

void compilar_traducoes();
