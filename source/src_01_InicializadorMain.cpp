#include "src_01_InicializadorMain.hpp"

#include "source/GerenciamentoInterfaceGrafica/Interface_Grafica.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"
#include "source/utils/LogManager.hpp"

#include <QApplication>

#include <chrono>
#include <csignal>

namespace {
    QApplication *g_app = nullptr;
    auto g_ultimoSigint = std::chrono::steady_clock::time_point{};

    void tratar_sigint(int) {
        const auto agora = std::chrono::steady_clock::now();
        if (g_app && g_ultimoSigint.time_since_epoch().count() != 0 &&
            std::chrono::duration_cast<std::chrono::seconds>(agora - g_ultimoSigint).count() <= 2) {
            LogManager::debug("Aplicacao recebeu SIGINT e sera encerrada.");
            g_app->quit();
            return;
        }
        g_ultimoSigint = agora;
    }
} // namespace

int iniciar_aplicacao(int argc, char *argv[]) {
    auto sinal_sigint_original = std::signal(SIGINT, tratar_sigint);
    try {
        QApplication app(argc, argv);
        g_app = &app;
        GerenciadorTraducao gerenciador_traducao;
        gerenciador_traducao.aplicar_traducao();
        InterfaceGerenciadorAtividades window(&gerenciador_traducao);
        window.show();
        const int exit_code = app.exec();
        LogManager::debug(QString("Aplicacao encerrada com codigo de saida: %1").arg(exit_code));
        std::signal(SIGINT, sinal_sigint_original);
        return exit_code;
    } catch (const std::exception &e) {
        LogManager::critical(QString("Erro fatal ao iniciar aplicacao: %1").arg(e.what()));
    } catch (...) {
        LogManager::critical("Erro fatal ao iniciar aplicacao.");
    }
    std::signal(SIGINT, sinal_sigint_original);
    return 1;
}
