# ELinker: An elf linker in C++

ELinker is a linker implementation which turns relocatable elf binaries into executable elf binaries.
It includes a 'somewhat' working AT&T linkscript syntax parser which is created in Bison and Flex. The linker supports the 3 major traditional segment types (.text, .data and .bss) . Some assumptions are made when parsing the linkscript:
 1. The code segment must have name '.text'.
 2. The data segment must have name '.data'.
 3. The bss segment must have name '.bss'.
 
 By default the alignment for all segments is set to the page size of the device which is used to build the executable elf file.
 When building an executable elf for another platform it may be necessary to override this default value. For that purpose the ALIGN property can be added to a segment definition. In the example below this is used to align the .bss segment.
 
Example link script:

```c++
ENTRY(_main)
SECTIONS
{
  . = 0x10000;
  .text :  { *(.text)* }
  . = 0x8000000;
  .data : { *(.data) }
  .bss : ALIGN(0x20000) { (.bss)* (my.bss)* }
}
```

# TODO!
  - Support static linking
  - Support more AT&T syntax

### Dependencies

To build ELinker following libraries are required:

* [Boost] - Header files and compiled: unit_test_framework and date_time.
* [CMake] - Build script.
* [Flex] - Used to generate code which is used when parsing the linkscript.
* [Bison] - Used to generate code which is used when parsing the linkscript.

License
----

Boost


   [Boost]: <http://www.boost.org>
   [CMake]: <https://cmake.org/>
   [Bison]: <https://www.gnu.org/software/bison/>
   [Flex]: <https://www.gnu.org/software/flex/>
