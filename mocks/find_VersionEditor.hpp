#pragma once

#include <QString>
#include <QWidget>

class QComboBox;
class QLineEdit;
class QSpinBox;
class QTextEdit;

class VersionEditor : public QWidget {
    Q_OBJECT

public:
    explicit VersionEditor(QWidget *parent = nullptr);

private slots:
    void on_select_folder();
    void on_check();
    void on_save();

private:
    QString selected_base_dir;

    QLineEdit *folder_path = nullptr;
    QLineEdit *pt_version = nullptr;
    QSpinBox *pt_day = nullptr;
    QComboBox *pt_month = nullptr;
    QSpinBox *pt_year = nullptr;
    QLineEdit *en_version = nullptr;
    QSpinBox *en_day = nullptr;
    QComboBox *en_month = nullptr;
    QSpinBox *en_year = nullptr;
    QTextEdit *status = nullptr;
};
