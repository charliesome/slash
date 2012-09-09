cmake_minimum_required(VERSION 2.8)

project(slash C)

include_directories(inc/)

# generate src/lib/error_page.c from src/lib/error_page.sl:
file(READ src/lib/error_page.sl ERROR_PAGE_SRC)
string(REPLACE "\\" "\\\\" ERROR_PAGE_SRC ${ERROR_PAGE_SRC})
string(REPLACE "\"" "\\\"" ERROR_PAGE_SRC ${ERROR_PAGE_SRC})
string(REPLACE "\n" "\\n\"\n\"" ERROR_PAGE_SRC ${ERROR_PAGE_SRC})
file(WRITE ${CMAKE_BINARY_DIR}/src/lib/error_page.c "char* sl__error_page_src = \"${ERROR_PAGE_SRC}\";")
# cool.

find_package(flex REQUIRED)
flex_target(Scanner src/lex.yy ${CMAKE_BINARY_DIR}/src/lex.c)

add_library(slash STATIC src/class.c
                         src/compile.c
                         src/error.c
                         src/eval.c
                         src/gc.c
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
