cmake_minimum_required(VERSION 2.8)
include(CheckSymbolExists)
include(CheckIncludeFile)
include(CheckLibraryExists)

set(CMAKE_MODULE_PATH "${CMAKE_ROOT}/cmake")

project(slash C)

# SAPIs that can be selected:

option(SL_SAPI_cli      "Build the command line interpreter"    ON)
option(SL_SAPI_apache2  "Build the apache2 module (mod_slash)"  OFF)
option(SL_SAPI_fcgi     "Build the FastCGI SAPI"                OFF)

# Extensions that can be selected

option(SL_EXT_base64    "Build the base64 extension"            ON)
option(SL_EXT_gcrypt    "Build the gcrypt extension"            ON)
option(SL_EXT_inflect   "Build the inflect extension"           ON)
option(SL_EXT_json      "Build the json extension"              ON)
option(SL_EXT_mysql     "Build the mysql extension"             ON)

include_directories(inc/)

# generate src/lib/error_page.c from src/lib/error_page.sl:
file(READ src/lib/error_page.sl ERROR_PAGE_SRC)
string(REPLACE "\\" "\\\\" ERROR_PAGE_SRC ${ERROR_PAGE_SRC})
string(REPLACE "\"" "\\\"" ERROR_PAGE_SRC ${ERROR_PAGE_SRC})
string(REPLACE "\n" "\\n\"\n\"" ERROR_PAGE_SRC ${ERROR_PAGE_SRC})
file(WRITE ${CMAKE_BINARY_DIR}/src/lib/error_page.c "char* sl__error_page_src = \"${ERROR_PAGE_SRC}\";")
# cool.

find_package(Flex REQUIRED)
flex_target(Scanner src/lex.yy ${CMAKE_BINARY_DIR}/src/lex.c)

check_include_file(gmp.h GMP_GMP_H)
check_symbol_exists(mpz_init gmp.h GMP_MPZ_INIT)

check_include_file(iconv.h ICONV_ICONV_H)
check_symbol_exists(iconv iconv.h ICONV_ICONV)

check_include_file(pcre.h ICONV_ICONV_H)
check_symbol_exists(iconv iconv.h ICONV_ICONV)

list(APPEND SOURCES       src/class.c
                          src/compile.c
                          src/error.c
                          src/eval.c
                          src/gc.c
                          ${CMAKE_BINARY_DIR}/src/init_exts.c
                          src/lex.c
                          ${CMAKE_BINARY_DIR}/src/lex_helper.c
                          src/method.c
                          src/object.c
                          src/parse.c
                          src/parse_helper.c
                          src/st.c
                          src/string.c
                          src/utf8.c
                          src/value.c
                          src/vm.c
                          src/vm_exec.c
                          
                          src/lib/array.c
                          src/lib/bignum.c
                          src/lib/comparable.c
                          src/lib/dict.c
                          src/lib/enumerable.c
                          ${CMAKE_BINARY_DIR}/src/lib/error_page.c
                          src/lib/false.c
                          src/lib/file.c
                          src/lib/float.c
                          src/lib/int.c
                          src/lib/lambda.c
                          src/lib/nil.c
                          src/lib/number.c
                          src/lib/rand.c
                          src/lib/range.c
                          src/lib/regexp.c
                          src/lib/request.c
                          src/lib/require.c
                          src/lib/response.c
                          src/lib/system.c
                          src/lib/true.c)

function(sl_static_initializer func_name)
    message(${func_name})
    #list(APPEND STATIC_INITIALIZERS ${func_name})
    
endfunction(sl_static_initializer)

add_library(slash STATIC ${SOURCES})

file(WRITE ${CMAKE_BINARY_DIR}/src/init_exts.c "
    #include <slash.h>
    
    void
    sl_static_init_exts()
    {
        /* */
    }
    
    void
    sl_init_exts(sl_vm_t* vm)
    {
        /* */
    }
")
