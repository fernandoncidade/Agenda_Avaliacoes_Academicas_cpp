#include "source/BancoDeDados/Banco_Dados.hpp"
#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"
#include "source/utils/ComboBoxUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QHBoxLayout>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>

void InterfaceGerenciadorAtividades::create_widgets() {
    auto *layout_horizontal_1 = new QHBoxLayout();
    auto *layout_vertical_1 = new QVBoxLayout();
    label_curso = new QLabel(this);
    combo_curso = new QComboBox(this);
    configurar_combo_texto_livre(combo_curso);
    combo_curso->setMinimumWidth(300);
    combo_curso->setMaximumWidth(300);
    combo_curso->addItems(obter_cursos());
    connect(combo_curso, &QComboBox::currentTextChanged, this, &InterfaceGerenciadorAtividades::update_ementa);
    layout_vertical_1->addWidget(label_curso);
    layout_vertical_1->addWidget(combo_curso);
    label_ementa = new QLabel(this);
    entry_ementa = new QComboBox(this);
    configurar_combo_texto_livre(entry_ementa);
    entry_ementa->setMinimumWidth(300);
    entry_ementa->setMaximumWidth(300);
    connect(entry_ementa, &QComboBox::currentTextChanged, this, &InterfaceGerenciadorAtividades::update_semestre);
    layout_vertical_1->addWidget(label_ementa);
    layout_vertical_1->addWidget(entry_ementa);
    label_semestre = new QLabel(this);
    entry_semestre = new QComboBox(this);
    configurar_combo_texto_livre(entry_semestre);
    entry_semestre->setMinimumWidth(300);
    entry_semestre->setMaximumWidth(300);
    connect(entry_semestre, &QComboBox::currentTextChanged, this, &InterfaceGerenciadorAtividades::update_disciplinas);
    layout_vertical_1->addWidget(label_semestre);
    layout_vertical_1->addWidget(entry_semestre);
    label_disciplina = new QLabel(this);
    entry_disciplina = new QComboBox(this);
    configurar_combo_texto_livre(entry_disciplina);
    entry_disciplina->setMinimumWidth(300);
    entry_disciplina->setMaximumWidth(300);
    layout_vertical_1->addWidget(label_disciplina);
    layout_vertical_1->addWidget(entry_disciplina);
    label_codigo = new QLabel(this);
    entry_codigo = new QComboBox(this);
    configurar_combo_texto_livre(entry_codigo);
    entry_codigo->setMinimumWidth(300);
    entry_codigo->setMaximumWidth(300);
    entry_codigo->addItems(obter_turmas());
    layout_vertical_1->addWidget(label_codigo);
    layout_vertical_1->addWidget(entry_codigo);
    label_tipo = new QLabel(this);
    combo_tipo = new QComboBox(this);
    configurar_combo_texto_livre(combo_tipo);
    combo_tipo->setMinimumWidth(300);
    combo_tipo->setMaximumWidth(300);
    combo_tipo->addItems(obter_avaliacoes());
    layout_vertical_1->addWidget(label_tipo);
    layout_vertical_1->addWidget(combo_tipo);
    label_sequencia = new QLabel(this);
    combo_sequencia = new QComboBox(this);
    configurar_combo_texto_livre(combo_sequencia);
    combo_sequencia->setMinimumWidth(300);
    combo_sequencia->setMaximumWidth(300);
    combo_sequencia->addItems(obter_sequencias());
    layout_vertical_1->addWidget(label_sequencia);
    layout_vertical_1->addWidget(combo_sequencia);
    label_data = new QLabel(this);
    calendar = new QCalendarWidget(this);
    calendar->setMinimumWidth(300);
    calendar->setMaximumWidth(300);
    calendar->setMaximumHeight(200);
    layout_vertical_1->addWidget(label_data);
    layout_vertical_1->addWidget(calendar);
    auto *layout_botoes = new QHBoxLayout();
    auto *layout_sub_vertical_1 = new QVBoxLayout();
    auto *layout_sub_vertical_2 = new QVBoxLayout();
    button_clear_item = create_button();
    connect(button_clear_item, &QPushButton::clicked, this, &InterfaceGerenciadorAtividades::excluir_item);
    button_clear_ultima = create_button();
    connect(button_clear_ultima, &QPushButton::clicked, this, &InterfaceGerenciadorAtividades::limpar_ultima_entrada);
    button_clear = create_button();
    connect(button_clear, &QPushButton::clicked, this, &InterfaceGerenciadorAtividades::limpar_entradas);
    button_editar_item = create_button();
    connect(button_editar_item, &QPushButton::clicked, this, &InterfaceGerenciadorAtividades::editar_item);
    button_submiter = create_button();
    connect(button_submiter, &QPushButton::clicked, this, &InterfaceGerenciadorAtividades::submiter);
    button_export = create_button();
    connect(button_export, &QPushButton::clicked, this, &InterfaceGerenciadorAtividades::exportar_para_pdf);
    layout_sub_vertical_1->addWidget(button_clear_item);
    layout_sub_vertical_1->addWidget(button_clear_ultima);
    layout_sub_vertical_1->addWidget(button_clear);
    layout_sub_vertical_2->addWidget(button_editar_item);
    layout_sub_vertical_2->addWidget(button_submiter);
    layout_sub_vertical_2->addWidget(button_export);
    layout_botoes->addLayout(layout_sub_vertical_1);
    layout_botoes->addLayout(layout_sub_vertical_2);
    layout_vertical_1->addLayout(layout_botoes);
    layout_vertical_1->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout_horizontal_1->addLayout(layout_vertical_1);
    auto *layout_vertical_2 = new QVBoxLayout();
    label_banco_dados = new QLabel(this);
    textbox = new QTextBrowser(this);
    textbox->setMinimumWidth(600);
    layout_vertical_2->addWidget(label_banco_dados);
    layout_vertical_2->addWidget(textbox);
    layout_horizontal_1->addLayout(layout_vertical_2);
    auto *main_layout = new QVBoxLayout();
    main_layout->addLayout(layout_horizontal_1);
    auto *central = new QWidget(this);
    central->setLayout(main_layout);
    setCentralWidget(central);
    gerenciamento_atividades.textbox = textbox;
    gerenciamento_atividades.combo_curso = combo_curso;
    gerenciamento_atividades.entry_codigo = entry_codigo;
    gerenciamento_atividades.combo_tipo = combo_tipo;
    gerenciamento_atividades.combo_sequencia = combo_sequencia;
    gerenciamento_atividades.calendar = calendar;
    gerenciamento_atividades.entry_disciplina = entry_disciplina;
    gerenciamento_atividades.entry_ementa = entry_ementa;
    gerenciamento_atividades.entry_semestre = entry_semestre;
}
