# ELinker: An elf linker in C++

ELinker is a linker implementation which turns relocatable elf binaries into executable elf binaries.
It includes a 'somewhat' working AT&T linkscript syntax parser which is created in Bison and Flex. The linker supports the 3 major traditional segment types (.text, .data and .bss) . Some assumptions are made when parsing the linkscript:
 1. The code segment must have name '.text'.
 2. The data segment must have name '.data'.
 3. The bss segment must have name '.bss'.
 
 ## ALIGN property of segments
 By default the alignment for each segment will be set to the page size of the device which is used to build the executable elf file.
 When building an executable elf for another platform it may be necessary to override this default value. The ALIGN property can be added to a segment definition to achieve this. In the example linkerscript below the ALIGN property is used to align the .bss segment. It may be required to add this property if the target platform has a pagesize which is not a multiple of the default alignment.
Following statement is always enforced by the linker: address % alignment == offset % alignment. 
1. 'Offset' is the position of the first byte of the segment in the executable elf file. 
2. 'Address' is the (virtual/physical)address where the first byte of the segment will be memory mapped.

Only if it is not possible to solve this equation by decreasing the offset will the virtual address of the segment be increased. Decreasing the offset is essentially making the segment bigger; it expresses: "this segment actually starts at another offset in the file now". Using this technique leads to wasted address space since some data will be mapped twice into memory. The pros outweigh the cons. If this equation is not satisfied it is possible that the elf cannot be memory mapped and the executable elf file is not runnable.

**TL;DR Segments should align to the target device's page size. Use the ALIGN(...) property.**
 
## Example linkerscript

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
