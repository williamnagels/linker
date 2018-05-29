# ELinker: An elf linker in C++

ELinker is a linker implementation which turns relocatable elf binaries into executable elf binaries.
It includes a 'somewhat' working AT&T linkscript syntax parser which is created in Bison and Flex. The linker supports the 3 major traditional types (.text, .data and .bss) of segments and when parsing the linkscript the following is assumed:
 1. The code segment will have '.text' as name.
 2. The data segment will have '.data' as name.
 3. The bss segment: must have '.bss' as name

Example link script:
```
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
