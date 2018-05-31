# ELinker: An elf linker in C++

ELinker is a linker implementation which turns relocatable elf binaries into executable elf binaries.
It includes a 'somewhat' working AT&T linkscript syntax parser which is created in Bison and Flex. The linker supports the 3 major traditional segment types (.text, .data and .bss) . Some assumptions are made when parsing the linkscript:
 1. The code segment must have name '.text'.
 2. The data segment must have name '.data'.
 3. The bss segment must have name '.bss'.

Example link script:

```c++
ENTRY(_main)
SECTIONS
{
  . = 0x10000;
  .text : { *(.text)* }
  . = 0x8000000;
  .data : { *(.data) }
  .bss : { (.bss)* (my.bss)* }
}
```

# TODO!
  - Support static linking
  - Support more AT&T syntax

### Dependencies

To build ELinker following libraries are required:

* [Boost] - header files and compiled: unit_test_framework and date_time 
* [CMake] - build script
* [Flex] - Used to parse the linkscript
* [Bison] - Used to parse the linkscript

License
----

Boost


   [Boost]: <http://www.boost.org>
   [CMake]: <https://cmake.org/>
   [Bison]: <https://www.gnu.org/software/bison/>
   [Flex]: <https://www.gnu.org/software/flex/>
