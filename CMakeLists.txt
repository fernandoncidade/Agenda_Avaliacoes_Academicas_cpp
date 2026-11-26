cmake_minimum_required(VERSION 3.28)

project(Agenda_Avaliacoes_Academicas_cpp
    VERSION 2026.11.22.0
    DESCRIPTION "Agenda de Avaliacoes Academicas em C++/Qt"
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(AgendaToolchain)
include(AgendaTranslations)
include(AgendaDeployment)
include(AgendaFormatting)

agenda_configure_project_defaults()

find_package(Qt6 6.12 REQUIRED COMPONENTS Core Gui Widgets Sql PrintSupport LinguistTools)
qt_standard_project_setup()

option(AGENDA_BUILD_TESTS "Compilar e registrar a suite completa de testes automatizados" OFF)

agenda_configure_deploy_dir()
agenda_resolve_qt_and_compiler_runtime()
agenda_configure_translations("${CMAKE_CURRENT_SOURCE_DIR}/source/language/translations")

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
)
file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp"
)

add_library(AgendaCore STATIC
    ${APP_SOURCES}
    ${APP_HEADERS}
)

target_include_directories(AgendaCore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/source"
)

target_link_libraries(AgendaCore PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::PrintSupport
)

add_dependencies(AgendaCore AgendaTranslations)

qt_add_executable(AgendaAvaliacoesAcademicas WIN32
    main.cpp
    source/resources.rc
)

target_link_libraries(AgendaAvaliacoesAcademicas PRIVATE
    AgendaCore
)

add_dependencies(AgendaAvaliacoesAcademicas AgendaTranslations)

agenda_define_runtime_assets("${CMAKE_CURRENT_SOURCE_DIR}")
agenda_configure_app_deployment(AgendaAvaliacoesAcademicas "${CMAKE_CURRENT_SOURCE_DIR}")
agenda_configure_code_formatting("${CMAKE_CURRENT_SOURCE_DIR}")

if(AGENDA_BUILD_TESTS)
    enable_testing()
    find_package(Qt6 6.12 REQUIRED COMPONENTS Test)
    add_subdirectory(tests)
endif()
