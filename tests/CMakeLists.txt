cmake_minimum_required(VERSION 3.28)

set(AGENDA_TEST_ASSETS_COMMANDS
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${CMAKE_BINARY_DIR}/source"
)

set(_AGENDA_TEST_ASSET_DIRS "${CMAKE_BINARY_DIR}/source/language/translations")
foreach(_asset_rel IN LISTS AGENDA_RUNTIME_ASSETS)
    get_filename_component(_asset_subdir "${_asset_rel}" DIRECTORY)
    if(_asset_subdir)
        list(APPEND _AGENDA_TEST_ASSET_DIRS "${CMAKE_BINARY_DIR}/source/assets/${_asset_subdir}")
    endif()
endforeach()
list(REMOVE_DUPLICATES _AGENDA_TEST_ASSET_DIRS)

list(APPEND AGENDA_TEST_ASSETS_COMMANDS
    COMMAND "${CMAKE_COMMAND}" -E make_directory ${_AGENDA_TEST_ASSET_DIRS}
)

foreach(_asset_rel IN LISTS AGENDA_RUNTIME_ASSETS)
    list(APPEND AGENDA_TEST_ASSETS_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${CMAKE_SOURCE_DIR}/source/assets/${_asset_rel}"
            "${CMAKE_BINARY_DIR}/source/assets/${_asset_rel}"
    )
endforeach()

foreach(_qm_file IN LISTS AGENDA_TRANSLATION_OUTPUTS)
    get_filename_component(_qm_name "${_qm_file}" NAME)
    list(APPEND AGENDA_TEST_ASSETS_COMMANDS
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${_qm_file}"
            "${CMAKE_BINARY_DIR}/source/language/translations/${_qm_name}"
    )
endforeach()

add_custom_target(AgendaTestRuntimeAssets
    ${AGENDA_TEST_ASSETS_COMMANDS}
    DEPENDS AgendaTranslations
    COMMENT "Copiando assets e traducoes localmente no diretorio de build da suite (sem distribuicao externa)"
    VERBATIM
)

set(AGENDA_TEST_TARGETS
    test_01_FrameworkAndPlugins
    test_02_UtilsAndPersistence
    test_03_DictionariesAndMappings
    test_04_CustomListsDatabase
    test_05_BancoDadosIntegration
    test_06_ActivityManagerCRUD
    test_07_TranslationsAndAssets
    test_08_GuiMainWindowAndWorkflows
    test_09_MocksAndDeploymentIntegrity
)

set(_AGENDA_QT_PLUGINS_DIR "${AGENDA_QT_BIN_DIR}/../plugins")

foreach(T_NAME IN LISTS AGENDA_TEST_TARGETS)
    add_executable(${T_NAME} ${T_NAME}.cpp)

    target_include_directories(${T_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/source"
    )

    if(T_NAME STREQUAL "test_09_MocksAndDeploymentIntegrity")
        target_sources(${T_NAME} PRIVATE
            "${CMAKE_SOURCE_DIR}/mocks/find_VersionEditor.cpp"
            "${CMAKE_SOURCE_DIR}/mocks/find_VersionEditor.hpp"
            "${CMAKE_SOURCE_DIR}/mocks/compileTranslations.cpp"
            "${CMAKE_SOURCE_DIR}/mocks/compileTranslations.hpp"
        )
        set_source_files_properties(
            "${CMAKE_SOURCE_DIR}/mocks/find_VersionEditor.cpp"
            PROPERTIES COMPILE_DEFINITIONS
            "AGENDA_SOURCE_DIR=\"${CMAKE_CURRENT_BINARY_DIR}/mock_version_sandbox\""
        )
        set_source_files_properties(
            "${CMAKE_SOURCE_DIR}/mocks/compileTranslations.cpp"
            PROPERTIES COMPILE_DEFINITIONS
            "AGENDA_LANGUAGE_DIR=\"${CMAKE_CURRENT_BINARY_DIR}/mock_translations_sandbox\""
        )
        target_include_directories(${T_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/mocks")
    endif()

    target_link_libraries(${T_NAME} PRIVATE
        AgendaCore
        Qt6::Test
    )

    target_compile_definitions(${T_NAME} PRIVATE
        AGENDA_QT_PLUGINS_DIR="${_AGENDA_QT_PLUGINS_DIR}"
        AGENDA_LRELEASE_PATH="${AGENDA_LRELEASE_EXECUTABLE}"
        AGENDA_LRELEASE_EXECUTABLE="${AGENDA_LRELEASE_EXECUTABLE}"
        AGENDA_PROJECT_VERSION="${PROJECT_VERSION}"
        AGENDA_DEPLOY_DIR_CONFIGURED="${AGENDA_DEPLOY_DIR}"
    )
    if(T_NAME STREQUAL "test_09_MocksAndDeploymentIntegrity")
        target_compile_definitions(${T_NAME} PRIVATE
            AGENDA_REAL_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
            AGENDA_VERSION_SANDBOX_DIR="${CMAKE_CURRENT_BINARY_DIR}/mock_version_sandbox"
            AGENDA_TRANSLATIONS_SANDBOX_DIR="${CMAKE_CURRENT_BINARY_DIR}/mock_translations_sandbox"
        )
    else()
        target_compile_definitions(${T_NAME} PRIVATE
            AGENDA_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
            AGENDA_LANGUAGE_DIR="${CMAKE_SOURCE_DIR}/source/language"
        )
    endif()

    if(MSVC)
        target_compile_definitions(${T_NAME} PRIVATE AGENDA_TOOLCHAIN_MSVC=1)
        target_compile_options(${T_NAME} PRIVATE /W4 /utf-8 /permissive- /wd4335)
    else()
        target_compile_definitions(${T_NAME} PRIVATE AGENDA_TOOLCHAIN_MINGW=1)
        target_compile_options(${T_NAME} PRIVATE -Wall -Wextra -Wno-unused-parameter)
    endif()

    add_dependencies(${T_NAME} AgendaTestRuntimeAssets AgendaTranslations)

    add_test(NAME ${T_NAME} COMMAND $<TARGET_FILE:${T_NAME}>)

    if(WIN32)
        set(_AGENDA_TEST_ENV
            "QT_PLUGIN_PATH=${_AGENDA_QT_PLUGINS_DIR}"
            "QT_QPA_PLATFORM=offscreen"
            "QT_ASSUME_STDERR_HAS_CONSOLE=1"
        )
        set(_AGENDA_TEST_ENV_MODS
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:Qt6::Core>"
        )
        if(MINGW AND DEFINED AGENDA_MINGW_BIN_DIR AND NOT "${AGENDA_MINGW_BIN_DIR}" STREQUAL "")
            list(APPEND _AGENDA_TEST_ENV_MODS "PATH=path_list_prepend:${AGENDA_MINGW_BIN_DIR}")
        endif()
        set_tests_properties(${T_NAME} PROPERTIES
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            ENVIRONMENT "${_AGENDA_TEST_ENV}"
            ENVIRONMENT_MODIFICATION "${_AGENDA_TEST_ENV_MODS}"
        )
    endif()
endforeach()

add_custom_target(AgendaTestSuite
    DEPENDS AgendaTestRuntimeAssets ${AGENDA_TEST_TARGETS}
)
