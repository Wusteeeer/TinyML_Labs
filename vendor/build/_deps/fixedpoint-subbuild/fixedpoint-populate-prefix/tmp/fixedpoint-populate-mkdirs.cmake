# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-src")
  file(MAKE_DIRECTORY "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-src")
endif()
file(MAKE_DIRECTORY
  "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-build"
  "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-subbuild/fixedpoint-populate-prefix"
  "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-subbuild/fixedpoint-populate-prefix/tmp"
  "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-subbuild/fixedpoint-populate-prefix/src/fixedpoint-populate-stamp"
  "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-subbuild/fixedpoint-populate-prefix/src"
  "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-subbuild/fixedpoint-populate-prefix/src/fixedpoint-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-subbuild/fixedpoint-populate-prefix/src/fixedpoint-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/wuster/projects/python/TinyML_Labs/vendor/build/_deps/fixedpoint-subbuild/fixedpoint-populate-prefix/src/fixedpoint-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
