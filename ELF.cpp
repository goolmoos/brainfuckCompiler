#include "ELF.hpp"

template <typename T>
void writeNumLittleEndian(std::ostream& o, T& val) {
	for(int i = 0; i < sizeof(val); ++i) {
		o << (unsigned char)val;
		val >>= 8;
	}
}

void ELF::Elf64_Ehdr::writeLittleEndian(std::ostream& out) {
	for(int i = 0; i < sizeof(ident); ++i) {
		out << ident[i];
	}
	writeNumLittleEndian(out, type);
	writeNumLittleEndian(out, machine);
	writeNumLittleEndian(out, version);
	writeNumLittleEndian(out, entry);
	writeNumLittleEndian(out, phoff);
	writeNumLittleEndian(out, shoff);
	writeNumLittleEndian(out, flags);
	writeNumLittleEndian(out, ehsize);
	writeNumLittleEndian(out, phentsize);
	writeNumLittleEndian(out, phnum);
	writeNumLittleEndian(out, shentsize);
	writeNumLittleEndian(out, shnum);
	writeNumLittleEndian(out, shstrndx);
}

void ELF::Elf64_Phdr::writeLittleEndian(std::ostream& out) {
	writeNumLittleEndian(out, type);
	writeNumLittleEndian(out, flags);
	writeNumLittleEndian(out, offset);
	writeNumLittleEndian(out, vaddr);
	writeNumLittleEndian(out, paddr);
	writeNumLittleEndian(out, filesz);
	writeNumLittleEndian(out, memsz);
	writeNumLittleEndian(out, align);
}
