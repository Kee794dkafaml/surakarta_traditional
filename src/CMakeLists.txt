SET(CORE_SOURCES
    surakarta_bitboard.cpp
    surakarta_reason.cpp
    surakarta_game.cpp
    surakarta_rule_manager_bitboard.cpp
    surakarta_rule_manager.cpp
    surakarta_rule_manager_impl.cpp
    surakarta_search.cpp
    surakarta_dev_session.cpp
    global_random_generator.cpp
    surakarta_utils.cpp
    surakarta_agent_random.cpp
    surakarta_agent_mine.cpp
    surakarta_daemon.cpp
    surakarta_agent_interactive.cpp
    surakarta_animation.cpp
    surakarta_logger.cpp
)

if(NOT TARGET surakarta)
    if(WIN32)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif()
    add_library(surakarta SHARED ${CORE_SOURCES})
    set_target_properties(surakarta PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_include_directories(surakarta PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    install(TARGETS surakarta)
endif()

if(NOT TARGET surakarta-benchmark)
    add_executable(surakarta-benchmark main.cpp)
    target_link_libraries(surakarta-benchmark surakarta)
    install(TARGETS surakarta-benchmark)
endif()

if(NOT TARGET surakarta-bitboard-selftest)
    add_executable(surakarta-bitboard-selftest surakarta_bitboard_selftest.cpp)
    target_link_libraries(surakarta-bitboard-selftest surakarta)
    get_filename_component(BITBOARD_TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../test/test_data" ABSOLUTE)
    target_compile_definitions(surakarta-bitboard-selftest PRIVATE BITBOARD_TEST_DATA_DIR="${BITBOARD_TEST_DATA_DIR}")
    add_test(NAME surakarta-bitboard-selftest COMMAND surakarta-bitboard-selftest)
    install(TARGETS surakarta-bitboard-selftest)
endif()

if(NOT TARGET surakarta-dev-session-selftest)
    add_executable(surakarta-dev-session-selftest surakarta_dev_session_selftest.cpp)
    target_link_libraries(surakarta-dev-session-selftest surakarta)
    get_filename_component(BITBOARD_TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../test/test_data" ABSOLUTE)
    target_compile_definitions(surakarta-dev-session-selftest PRIVATE BITBOARD_TEST_DATA_DIR="${BITBOARD_TEST_DATA_DIR}")
    add_test(NAME surakarta-dev-session-selftest COMMAND surakarta-dev-session-selftest)
    install(TARGETS surakarta-dev-session-selftest)
endif()

if(NOT TARGET surakarta-mask-export)
    add_executable(surakarta-mask-export
        surakarta_mask_export.cpp
        surakarta_bitboard.cpp
    )
    target_compile_definitions(surakarta-mask-export PRIVATE SURAKARTA_DYNAMIC_MASKS=1)
    target_include_directories(surakarta-mask-export PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    install(TARGETS surakarta-mask-export)
endif()

if(SURAKARTA_ENABLE_GUI AND NOT TARGET surakarta-gui)
    include(FetchContent)

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip
    )
    FetchContent_MakeAvailable(glfw)

    FetchContent_Declare(
        imgui
        URL https://github.com/ocornut/imgui/archive/refs/tags/v1.92.7.zip
    )
    FetchContent_GetProperties(imgui)
    if(NOT imgui_POPULATED)
        FetchContent_Populate(imgui)
        add_library(imgui STATIC
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        )
        target_include_directories(imgui PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends
        )
        target_link_libraries(imgui PUBLIC glfw)
    endif()

    find_package(OpenGL REQUIRED)

    add_executable(surakarta-gui surakarta_gui_main.cpp)
    target_link_libraries(surakarta-gui PRIVATE surakarta imgui OpenGL::GL glfw)
    target_include_directories(surakarta-gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    get_filename_component(BITBOARD_GUI_TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../test/test_data" ABSOLUTE)
    add_test(NAME surakarta-gui-smoke-session
        COMMAND surakarta-gui --smoke-test --file "${BITBOARD_GUI_TEST_DATA_DIR}/game1.txt")
    add_test(NAME surakarta-gui-smoke-init
        COMMAND surakarta-gui --smoke-test-init)
    add_test(NAME surakarta-gui-smoke-geometry
        COMMAND surakarta-gui --smoke-test-geometry)
    set_tests_properties(surakarta-gui-smoke-session PROPERTIES TIMEOUT 10)
    set_tests_properties(surakarta-gui-smoke-init PROPERTIES TIMEOUT 10)
    set_tests_properties(surakarta-gui-smoke-geometry PROPERTIES TIMEOUT 10)
    install(TARGETS surakarta-gui)
endif()
