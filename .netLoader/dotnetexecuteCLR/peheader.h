#pragma once
#pragma once
#include <windows.h>




typedef struct _widecharbuffer
{
    wchar_t* Buffer;
    unsigned long Length;
} widecharbuffer;



typedef struct _asciicharbuffer
{
    unsigned char* Buffer;
    unsigned long Length;
} ascicharbuffer;


typedef struct _DIMAGE_DOS_HEADER {      // DOS .EXE header
    unsigned short   e_magic;                     // Magic number
    unsigned short   e_cblp;                      // Bytes on last page of file
    unsigned short   e_cp;                        // Pages in file
    unsigned short   e_crlc;                      // Relocations
    unsigned short   e_cparhdr;                   // Size of header in paragraphs
    unsigned short   e_minalloc;                  // Minimum extra paragraphs needed
    unsigned short   e_maxalloc;                  // Maximum extra paragraphs needed
    unsigned short   e_ss;                        // Initial (relative) SS value
    unsigned short   e_sp;                        // Initial SP value
    unsigned short   e_csum;                      // Checksum
    unsigned short   e_ip;                        // Initial IP value
    unsigned short   e_cs;                        // Initial (relative) CS value
    unsigned short   e_lfarlc;                    // File address of relocation table
    unsigned short   e_ovno;                      // Overlay number
    unsigned short   e_res[4];                    // Reserved unsigned shorts
    unsigned short   e_oemid;                     // OEM identifier (for e_oeminfo)
    unsigned short   e_oeminfo;                   // OEM information; e_oemid specific
    unsigned short   e_res2[10];                  // Reserved unsigned shorts
    long   e_lfanew;                    // File address of new exe header
} DIMAGE_DOS_HEADER, * PDIMAGE_DOS_HEADER;




#define IMAGE_SIZEOF_SHORT_NAME              8

typedef struct _DIMAGE_SECTION_HEADER {
    unsigned char    Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
        unsigned long   PhysicalAddress;
        unsigned long   VirtualSize;
    } Misc;
    unsigned long   VirtualAddress;
    unsigned long   SizeOfRawData;
    unsigned long   PointerToRawData;
    unsigned long   PointerToRelocations;
    unsigned long   PointerToLinenumbers;
    unsigned short    NumberOfRelocations;
    unsigned short    NumberOfLinenumbers;
    unsigned long   Characteristics;
} DIMAGE_SECTION_HEADER, * PDIMAGE_SECTION_HEADER;






typedef struct _DIMAGE_FILE_HEADER {
    unsigned short    Machine;
    unsigned short    NumberOfSections;
    unsigned long   TimeDateStamp;
    unsigned long   PointerToSymbolTable;
    unsigned long   NumberOfSymbols;
    unsigned short    SizeOfOptionalHeader;
    unsigned short    Characteristics;
} DIMAGE_FILE_HEADER, * PDIMAGE_FILE_HEADER;



typedef struct _DIMAGE_DATA_DIRECTORY {
    unsigned long   VirtualAddress;
    unsigned long   Size;
} DIMAGE_DATA_DIRECTORY, * PDIMAGE_DATA_DIRECTORY;




typedef struct _DIMAGE_OPTIONAL_HEADER {
    //
    // Standard fields.
    //

    unsigned short    Magic;
    unsigned char    MajorLinkerVersion;
    unsigned char    MinorLinkerVersion;
    unsigned long   SizeOfCode;
    unsigned long   SizeOfInitializedData;
    unsigned long   SizeOfUninitializedData;
    unsigned long   AddressOfEntryPoint;
    unsigned long   BaseOfCode;
    unsigned long   BaseOfData;

    //
    // NT additional fields.
    //

    unsigned long   ImageBase;
    unsigned long   SectionAlignment;
    unsigned long   FileAlignment;
    unsigned short    MajorOperatingSystemVersion;
    unsigned short    MinorOperatingSystemVersion;
    unsigned short    MajorImageVersion;
    unsigned short    MinorImageVersion;
    unsigned short    MajorSubsystemVersion;
    unsigned short    MinorSubsystemVersion;
    unsigned long   Win32VersionValue;
    unsigned long   SizeOfImage;
    unsigned long   SizeOfHeaders;
    unsigned long   CheckSum;
    unsigned short    Subsystem;
    unsigned short    DllCharacteristics;
    unsigned long   SizeOfStackReserve;
    unsigned long   SizeOfStackCommit;
    unsigned long   SizeOfHeapReserve;
    unsigned long   SizeOfHeapCommit;
    unsigned long   LoaderFlags;
    unsigned long   NumberOfRvaAndSizes;
    DIMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} DIMAGE_OPTIONAL_HEADER32, * PDIMAGE_OPTIONAL_HEADER32;





typedef struct _DIMAGE_OPTIONAL_HEADER64 {
    unsigned short        Magic;
    unsigned char        MajorLinkerVersion;
    unsigned char        MinorLinkerVersion;
    unsigned long       SizeOfCode;
    unsigned long       SizeOfInitializedData;
    unsigned long       SizeOfUninitializedData;
    unsigned long       AddressOfEntryPoint;
    unsigned long       BaseOfCode;
    unsigned long long   ImageBase;
    unsigned long       SectionAlignment;
    unsigned long       FileAlignment;
    unsigned short        MajorOperatingSystemVersion;
    unsigned short        MinorOperatingSystemVersion;
    unsigned short        MajorImageVersion;
    unsigned short        MinorImageVersion;
    unsigned short        MajorSubsystemVersion;
    unsigned short        MinorSubsystemVersion;
    unsigned long       Win32VersionValue;
    unsigned long       SizeOfImage;
    unsigned long       SizeOfHeaders;
    unsigned long       CheckSum;
    unsigned short        Subsystem;
    unsigned short        DllCharacteristics;
    unsigned long long   SizeOfStackReserve;
    unsigned long long   SizeOfStackCommit;
    unsigned long long   SizeOfHeapReserve;
    unsigned long long   SizeOfHeapCommit;
    unsigned long       LoaderFlags;
    unsigned long       NumberOfRvaAndSizes;
    DIMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} DIMAGE_OPTIONAL_HEADER64, * PDIMAGE_OPTIONAL_HEADER64;




typedef struct _DIMAGE_NT_HEADERS64 {
    unsigned long Signature;
    DIMAGE_FILE_HEADER FileHeader;
    DIMAGE_OPTIONAL_HEADER64 OptionalHeader;
} DIMAGE_NT_HEADERS64, * PDIMAGE_NT_HEADERS64;

typedef struct _DIMAGE_NT_HEADERS {
    unsigned long Signature;
    DIMAGE_FILE_HEADER FileHeader;
    DIMAGE_OPTIONAL_HEADER32 OptionalHeader;
} DIMAGE_NT_HEADERS32, * PDIMAGE_NT_HEADERS32;




typedef struct
{
    unsigned long Signature;
    unsigned short MajorVersion;
    unsigned short MinorVersion;
    unsigned long Reserved;
    unsigned long VersionLength;
} METADATA_HEADER;
#pragma once
