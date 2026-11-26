cmake_minimum_required(VERSION 3.28)

project(AgendaTools
    VERSION 2026.4.23.0
    DESCRIPTION "Ferramentas auxiliares do Agenda"
    LANGUAGES CXX
)

get_filename_component(AGENDA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
set(AGENDA_LANGUAGE_DIR "${AGENDA_SOURCE_DIR}/source/language")

list(APPEND CMAKE_MODULE_PATH "${AGENDA_SOURCE_DIR}/cmake")

include(AgendaToolchain)
include(AgendaDeployment)

agenda_configure_project_defaults()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 6.12 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
qt_standard_project_setup()

agenda_resolve_qt_and_compiler_runtime()

qt_add_executable(VersionEditor WIN32
    find_VersionEditor.cpp
    find_VersionEditor.hpp
)

target_compile_definitions(VersionEditor PRIVATE
    AGENDA_BUILD_VERSION_EDITOR_STANDALONE
    AGENDA_SOURCE_DIR="${AGENDA_SOURCE_DIR}"
)

target_include_directories(VersionEditor PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    target_compile_options(VersionEditor PRIVATE /utf-8)
endif()

target_link_libraries(VersionEditor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

agenda_deploy_mock_qt_runtime(VersionEditor)

qt_add_executable(CompileTranslations WIN32
    compileTranslations.cpp
    compileTranslations.hpp
)

target_compile_definitions(CompileTranslations PRIVATE
    AGENDA_BUILD_COMPILE_TRANSLATIONS_STANDALONE
    AGENDA_LANGUAGE_DIR="${AGENDA_LANGUAGE_DIR}"
    AGENDA_LRELEASE_EXECUTABLE="${AGENDA_LRELEASE_EXECUTABLE}"
)

target_include_directories(CompileTranslations PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    target_compile_options(CompileTranslations PRIVATE /utf-8)
endif()

target_link_libraries(CompileTranslations PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

agenda_deploy_mock_qt_runtime(CompileTranslations)
