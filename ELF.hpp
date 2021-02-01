#ifndef ELF_HPP
#define ELF_HPP

#include <cstdint>
#include <ostream>

namespace ELF {

	using Elf64_Xword = uint64_t;
	using Elf64_Sxword = int64_t;
	using Elf64_Addr = uint64_t;
	using Elf64_Off = uint64_t;
	using Elf64_Half = uint16_t;
	using Elf64_Word = uint32_t;
	using Elf64_Sword = int32_t;

	const int ET_EXEC = 2;
	const int EM_X86_64 = 62;
	const int EV_CURRENT = 1;
	const int ELFMAG0 = 0x7f;
	const int ELFMAG1 = 'E';
	const int ELFMAG2 = 'L';
	const int ELFMAG3 = 'F';
	const int ELFCLASS64 = 2;
	const int ELFDATA2LSB = 1;
	const int PT_LOAD = 1;
	const int PF_X = 1;
	const int PF_W = 2;
	const int PF_R = 4;

	struct Elf64_Ehdr
	{
		unsigned char ident[16];
		Elf64_Half type;
		Elf64_Half machine;
		Elf64_Word version;
		Elf64_Addr entry;
		Elf64_Off phoff;
		Elf64_Off shoff;
		Elf64_Word flags;
		Elf64_Half ehsize;
		Elf64_Half phentsize;
		Elf64_Half phnum;
		Elf64_Half shentsize;
		Elf64_Half shnum;
		Elf64_Half shstrndx;

		void writeLittleEndian(std::ostream& out);
	};

	struct Elf64_Phdr {
		Elf64_Word type;
		Elf64_Word flags;
		Elf64_Off offset;
		Elf64_Addr vaddr;
		Elf64_Addr paddr;
		Elf64_Xword filesz;
		Elf64_Xword memsz;
		Elf64_Xword align;

		void writeLittleEndian(std::ostream& out);
	};
}

#endif
