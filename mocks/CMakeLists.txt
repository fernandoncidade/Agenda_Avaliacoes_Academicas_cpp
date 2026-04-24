cmake_minimum_required(VERSION 3.21)

project(AgendaTools
    VERSION 2026.4.23.0
    DESCRIPTION "Ferramentas auxiliares do Agenda"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 6.11 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
qt_standard_project_setup()

if(MSVC)
    set(CMAKE_CXX_CL_SHOWINCLUDES_PREFIX "Observação: incluindo arquivo: ")
    set(CMAKE_CL_SHOWINCLUDES_PREFIX "${CMAKE_CXX_CL_SHOWINCLUDES_PREFIX}")
    add_compile_options(/wd4335)
endif()

get_filename_component(AGENDA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
set(AGENDA_LANGUAGE_DIR "${AGENDA_SOURCE_DIR}/source/language")

if(TARGET Qt6::lrelease)
    get_target_property(AGENDA_LRELEASE_EXECUTABLE Qt6::lrelease IMPORTED_LOCATION)
endif()
if(NOT AGENDA_LRELEASE_EXECUTABLE)
    find_program(AGENDA_LRELEASE_EXECUTABLE
        NAMES lrelease lrelease.exe
        HINTS "${CMAKE_PREFIX_PATH}/bin"
    )
endif()
if(NOT AGENDA_LRELEASE_EXECUTABLE)
    message(FATAL_ERROR "lrelease nao encontrado. Verifique a instalacao do Qt LinguistTools.")
endif()

set(AGENDA_QT_BIN_DIR "" CACHE PATH "Diretorio bin da instalacao Qt usada para windeployqt")

if(NOT AGENDA_QT_BIN_DIR AND TARGET Qt6::qmake)
    get_target_property(AGENDA_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    if(AGENDA_QMAKE_EXECUTABLE)
        get_filename_component(AGENDA_QT_BIN_DIR "${AGENDA_QMAKE_EXECUTABLE}" DIRECTORY)
    endif()
endif()

find_program(AGENDA_WINDEPLOYQT_EXECUTABLE
    NAMES windeployqt windeployqt.exe
    HINTS "${AGENDA_QT_BIN_DIR}" "${CMAKE_PREFIX_PATH}/bin"
)
if(NOT AGENDA_WINDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR "windeployqt nao encontrado. Verifique AGENDA_QT_BIN_DIR ou CMAKE_PREFIX_PATH.")
endif()

function(agenda_deploy_qt_runtime target_name)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND "${AGENDA_WINDEPLOYQT_EXECUTABLE}"
                --release
                --force
                --dir "$<TARGET_FILE_DIR:${target_name}>"
                "$<TARGET_FILE:${target_name}>"
        COMMENT "Copiando dependencias Qt para $<TARGET_FILE_DIR:${target_name}>"
        VERBATIM
    )
endfunction()

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

target_link_libraries(VersionEditor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

agenda_deploy_qt_runtime(VersionEditor)

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

target_link_libraries(CompileTranslations PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

agenda_deploy_qt_runtime(CompileTranslations)
