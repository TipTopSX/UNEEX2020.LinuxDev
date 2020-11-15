cmake_minimum_required(VERSION 3.16)
project(oracle C)

set(CMAKE_C_STANDARD 99)

add_executable(oracle oracle.c oracle_ru.po oracle.pot)

find_package(Gettext REQUIRED)
find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)

add_custom_command(
        OUTPUT ru/LC_MESSAGES/oracle.mo
        DEPENDS ${CMAKE_SOURCE_DIR}/oracle_ru.po
        COMMAND mkdir -p ru/LC_MESSAGES
        COMMAND msgfmt ${CMAKE_SOURCE_DIR}/oracle_ru.po -o ru/LC_MESSAGES/oracle.mo
)

add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/oracle_ru.po
        DEPENDS ${CMAKE_SOURCE_DIR}/oracle.pot
        COMMAND msgmerge -U ${CMAKE_SOURCE_DIR}/oracle_ru.po ${CMAKE_SOURCE_DIR}/oracle.pot
)

add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/oracle.pot
        DEPENDS ${CMAKE_SOURCE_DIR}/oracle.c
        COMMAND xgettext -k_ -c ${CMAKE_SOURCE_DIR}/oracle.c -o new.pot
        COMMAND msgmerge -U ${CMAKE_SOURCE_DIR}/oracle.pot new.pot
        COMMAND rm -f new.pot
)

add_custom_target(translation
        DEPENDS ru/LC_MESSAGES/oracle.mo
        )