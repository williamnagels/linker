# ELinker

ELinker is a linker implementation targetting ELF binaries. Primarly developped in c++
it exists of a core library which can be linked against so that it may be reused easily in other projects.
The front end is a CLI which is a facade and only feeds input to the core lib.

Compiles on MVSC. CMake will run unit tests when the build has been completed.

# Core Features!

  - Memory map elf files and build a model of the elf that can be hooked into.
  - Edit elf files using a straightforward API using elf nomenclature: sections, segments, symtabs... .

# TODO!
  - Actual linking

### Dependencies

To build ELinker following libraries are required:

* [Boost] - header files and compiled: unit_test_framework and date_time 
* [CMake] - build script

License
----

Boost


   [Boost]: <http://www.boost.org>
   [CMake]: <https://cmake.org/>
