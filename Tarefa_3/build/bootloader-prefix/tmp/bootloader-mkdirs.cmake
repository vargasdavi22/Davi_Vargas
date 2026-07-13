# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/usr/lib/sdk/esp-idf/idf/components/bootloader/subproject"
  "/var/home/aluno/workspace/blink/build/bootloader"
  "/var/home/aluno/workspace/blink/build/bootloader-prefix"
  "/var/home/aluno/workspace/blink/build/bootloader-prefix/tmp"
  "/var/home/aluno/workspace/blink/build/bootloader-prefix/src/bootloader-stamp"
  "/var/home/aluno/workspace/blink/build/bootloader-prefix/src"
  "/var/home/aluno/workspace/blink/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/var/home/aluno/workspace/blink/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/var/home/aluno/workspace/blink/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
