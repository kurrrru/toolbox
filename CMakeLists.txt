cmake_minimum_required(VERSION 3.20)

project(toolbox
    DESCRIPTION "Header-only collection of algorithms and data structures"
    LANGUAGES CXX
)

# ---- C++20 (拡張なし・必須) -------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ---- ヘッダオンリーライブラリ ----------------------------------------------
# インクルードルートは include/。すべてのヘッダは "toolbox/..." で解決する。
add_library(toolbox INTERFACE)
add_library(toolbox::toolbox ALIAS toolbox)
target_include_directories(toolbox INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(toolbox INTERFACE cxx_std_20)

# ---- テスト ----------------------------------------------------------------
option(TOOLBOX_BUILD_TESTS "Build the tests" ${PROJECT_IS_TOP_LEVEL})
if(TOOLBOX_BUILD_TESTS)
    enable_testing()

    # 共通の警告フラグ（DT-Patricia と揃える）。GCC / Clang 両対応。
    set(_toolbox_warn
        -Wall -Wextra -Wshadow -Wold-style-cast -Wnull-dereference
        -Wdouble-promotion -Wformat=2 -Werror -pedantic)

    # サニタイザ（ASan + UBSan）。ctest を確実に落とすため recover を無効化。
    option(TOOLBOX_SANITIZE
           "Build tests with AddressSanitizer + UndefinedBehaviorSanitizer" OFF)
    set(_toolbox_sanitize)
    if(TOOLBOX_SANITIZE)
        set(_toolbox_sanitize
            -fsanitize=address,undefined
            -fno-sanitize-recover=all
            -fno-omit-frame-pointer
            -g)
    endif()

    # 新しいテストの登録ヘルパ。
    #   toolbox_add_test(<name> <source.cpp>)
    function(toolbox_add_test name src)
        add_executable(${name} ${src})
        target_link_libraries(${name} PRIVATE toolbox::toolbox)
        # tests/ を include パスに追加（"test_util.hpp" を解決するため）
        target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/tests)
        target_compile_options(${name} PRIVATE ${_toolbox_warn} ${_toolbox_sanitize})
        target_link_options(${name}    PRIVATE ${_toolbox_sanitize})
        add_test(NAME ${name} COMMAND ${name})
    endfunction()

    # ---- テストの自動検出 --------------------------------------------------
    # tests/ 以下の任意の階層に置かれた main.cpp を自動でテストとして登録する。
    # 新しいアルゴリズムを追加するときは
    #     tests/<category>/<algorithm>/main.cpp
    # を作るだけでよい（CMakeLists.txt の編集は不要）。
    # テスト名は main.cpp を含むディレクトリ名 + "_test"。
    #   例) tests/datastructure/unionfind/main.cpp -> unionfind_test
    file(GLOB_RECURSE _toolbox_test_srcs CONFIGURE_DEPENDS
         ${PROJECT_SOURCE_DIR}/tests/*.cpp)
    foreach(_src ${_toolbox_test_srcs})
        get_filename_component(_base ${_src} NAME)
        if(NOT _base STREQUAL "main.cpp")
            continue()
        endif()
        get_filename_component(_dir ${_src} DIRECTORY)
        get_filename_component(_leaf ${_dir} NAME)
        toolbox_add_test(${_leaf}_test ${_src})
    endforeach()
endif()
