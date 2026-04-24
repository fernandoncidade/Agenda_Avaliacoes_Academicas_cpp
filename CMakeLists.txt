
cmake_minimum_required(VERSION 3.21)

project(Agenda_Avaliacoes_Academicas_cpp
    VERSION 2026.4.23.0
    DESCRIPTION "Agenda de Avaliacoes Academicas em C++/Qt"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.11 REQUIRED COMPONENTS Core Gui Widgets Sql PrintSupport LinguistTools)
qt_standard_project_setup()

if(MSVC)
    set(CMAKE_CXX_CL_SHOWINCLUDES_PREFIX "Observação: incluindo arquivo: ")
    set(CMAKE_CL_SHOWINCLUDES_PREFIX "${CMAKE_CXX_CL_SHOWINCLUDES_PREFIX}")
    add_compile_options(/wd4335)
endif()

set(AGENDA_DEPLOY_DIR_DEFAULT "D:/MISCELANEAS/VSCode/AGENDA/AgendaAvaliacoesAcademicas")
if(MSVC)
    set(AGENDA_DEPLOY_DIR_DEFAULT "D:/MISCELANEAS/VSCode/AGENDA/AgendaAvaliacoesAcademicas_msvc")
elseif(MINGW)
    set(AGENDA_DEPLOY_DIR_DEFAULT "D:/MISCELANEAS/VSCode/AGENDA/AgendaAvaliacoesAcademicas_mingw")
endif()

set(AGENDA_DEPLOY_DIR
    "${AGENDA_DEPLOY_DIR_DEFAULT}"
    CACHE PATH "Diretorio externo de distribuicao do Agenda"
)
if(AGENDA_DEPLOY_DIR STREQUAL "" OR AGENDA_DEPLOY_DIR STREQUAL "/" OR AGENDA_DEPLOY_DIR MATCHES "^[A-Za-z]:[/\\]?$")
    message(FATAL_ERROR "AGENDA_DEPLOY_DIR aponta para um caminho inseguro: '${AGENDA_DEPLOY_DIR}'")
endif()

set(AGENDA_QT_BIN_DIR "" CACHE PATH "Diretorio bin da instalacao Qt usada para windeployqt")

if(NOT AGENDA_QT_BIN_DIR AND TARGET Qt6::qmake)
    get_target_property(AGENDA_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    if(AGENDA_QMAKE_EXECUTABLE)
        get_filename_component(AGENDA_QT_BIN_DIR "${AGENDA_QMAKE_EXECUTABLE}" DIRECTORY)
    endif()
endif()

if(TARGET Qt6::lrelease)
    get_target_property(AGENDA_LRELEASE_EXECUTABLE Qt6::lrelease IMPORTED_LOCATION)
endif()
if(NOT AGENDA_LRELEASE_EXECUTABLE)
    find_program(AGENDA_LRELEASE_EXECUTABLE
        NAMES lrelease lrelease.exe
        HINTS "${AGENDA_QT_BIN_DIR}" "${CMAKE_PREFIX_PATH}/bin"
    )
endif()
if(NOT AGENDA_LRELEASE_EXECUTABLE)
    message(FATAL_ERROR "lrelease nao encontrado. Verifique a instalacao do Qt LinguistTools.")
endif()

find_program(AGENDA_WINDEPLOYQT_EXECUTABLE
    NAMES windeployqt windeployqt.exe
    HINTS "${AGENDA_QT_BIN_DIR}" "${CMAKE_PREFIX_PATH}/bin"
)
if(NOT AGENDA_WINDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR "windeployqt nao encontrado. Verifique AGENDA_QT_BIN_DIR ou CMAKE_PREFIX_PATH.")
endif()

file(GLOB AGENDA_TRANSLATION_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/language/translations/*.ts"
)
set(AGENDA_TRANSLATION_OUTPUTS)
foreach(AGENDA_TS_FILE IN LISTS AGENDA_TRANSLATION_SOURCES)
    get_filename_component(AGENDA_TS_DIR "${AGENDA_TS_FILE}" DIRECTORY)
    get_filename_component(AGENDA_TS_NAME "${AGENDA_TS_FILE}" NAME_WE)
    set(AGENDA_QM_FILE "${AGENDA_TS_DIR}/${AGENDA_TS_NAME}.qm")
    add_custom_command(
        OUTPUT "${AGENDA_QM_FILE}"
        COMMAND "${AGENDA_LRELEASE_EXECUTABLE}" "${AGENDA_TS_FILE}" -qm "${AGENDA_QM_FILE}"
        DEPENDS "${AGENDA_TS_FILE}"
        COMMENT "Compilando traducao ${AGENDA_TS_NAME}.ts"
        VERBATIM
    )
    list(APPEND AGENDA_TRANSLATION_OUTPUTS "${AGENDA_QM_FILE}")
endforeach()

add_custom_target(AgendaTranslations
    DEPENDS ${AGENDA_TRANSLATION_OUTPUTS}
)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
)
file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp"
)

qt_add_executable(AgendaAvaliacoesAcademicas WIN32
    main.cpp
    ${APP_SOURCES}
    ${APP_HEADERS}
    resources.rc
)

target_include_directories(AgendaAvaliacoesAcademicas PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/source"
)

target_link_libraries(AgendaAvaliacoesAcademicas PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::PrintSupport
)

add_dependencies(AgendaAvaliacoesAcademicas AgendaTranslations)

add_custom_command(TARGET AgendaAvaliacoesAcademicas POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/source/assets"
            "$<TARGET_FILE_DIR:AgendaAvaliacoesAcademicas>/source/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/source/language/translations"
            "$<TARGET_FILE_DIR:AgendaAvaliacoesAcademicas>/source/language/translations"
    COMMAND "${AGENDA_WINDEPLOYQT_EXECUTABLE}"
            --release
            --force
            --dir "$<TARGET_FILE_DIR:AgendaAvaliacoesAcademicas>"
            "$<TARGET_FILE:AgendaAvaliacoesAcademicas>"
    COMMENT "Copiando assets, traducoes e dependencias Qt para $<TARGET_FILE_DIR:AgendaAvaliacoesAcademicas>"
)

add_custom_target(DeployAgenda
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${AGENDA_DEPLOY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${AGENDA_DEPLOY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:AgendaAvaliacoesAcademicas>"
            "${AGENDA_DEPLOY_DIR}/$<TARGET_FILE_NAME:AgendaAvaliacoesAcademicas>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/source/assets"
            "${AGENDA_DEPLOY_DIR}/source/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/source/language/translations"
            "${AGENDA_DEPLOY_DIR}/source/language/translations"
    COMMAND "${AGENDA_WINDEPLOYQT_EXECUTABLE}"
            --release
            --dir "${AGENDA_DEPLOY_DIR}"
            "${AGENDA_DEPLOY_DIR}/$<TARGET_FILE_NAME:AgendaAvaliacoesAcademicas>"
    DEPENDS AgendaAvaliacoesAcademicas AgendaTranslations
    COMMENT "Gerando distribuicao em ${AGENDA_DEPLOY_DIR}"
    VERBATIM
)
