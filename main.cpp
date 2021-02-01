#include "ELF.hpp"
#include "compile.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
	if(argc != 2) {
		std::cerr << "Usage: file.\nOutput is in a.out.\n";
		return -1;
	}
	std::ifstream input(argv[1], std::ios::in);
	std::ostringstream ss;
	ss << input.rdbuf();
	const std::string& s = ss.str();
	std::vector<char> bfCode(s.begin(), s.end());

	const auto align = 0x200000;
	const auto bfDataStart = 0x200000;
	const auto bfDataLen = 0x200000;
	const auto textLoad = 0x600000 + sizeof(ELF::Elf64_Ehdr) + 2 * sizeof(ELF::Elf64_Phdr); // align
	const auto entryPoint = textLoad;

	std::vector<uint8_t> text = compile(bfCode, bfDataStart);

	std::ofstream output("./a.out", std::ios::binary);
	ELF::Elf64_Ehdr ehdr;
	memset(&ehdr, 0, sizeof(ehdr));
	ehdr.ident[0] = ELF::ELFMAG0;
	ehdr.ident[1] = ELF::ELFMAG1;
	ehdr.ident[2] = ELF::ELFMAG2;
	ehdr.ident[3] = ELF::ELFMAG3;
	ehdr.ident[4] = ELF::ELFCLASS64;
	ehdr.ident[5] = ELF::ELFDATA2LSB;
	ehdr.ident[6] = ELF::EV_CURRENT;
	ehdr.type = ELF::ET_EXEC;
	ehdr.machine = ELF::EM_X86_64;
	ehdr.version = ELF::EV_CURRENT;
	ehdr.entry = entryPoint;
	ehdr.phoff = sizeof(ehdr);
	ehdr.ehsize = sizeof(ehdr);
	ehdr.phentsize = sizeof(ELF::Elf64_Phdr);
	ehdr.phnum = 2;
	ehdr.writeLittleEndian(output);

	ELF::Elf64_Phdr load_RW; // .bss
	memset(&load_RW, 0, sizeof(load_RW));
	load_RW.type = ELF::PT_LOAD;
	load_RW.flags = ELF::PF_R | ELF::PF_W;
	load_RW.vaddr = bfDataStart;
	load_RW.paddr = bfDataStart;
	load_RW.filesz = 0;
	load_RW.memsz = bfDataLen;
	load_RW.align = align;
	load_RW.writeLittleEndian(output);

	ELF::Elf64_Phdr load_RX; // .text
	memset(&load_RX, 0, sizeof(load_RX));
	load_RX.type = ELF::PT_LOAD;
	load_RX.flags = ELF::PF_R | ELF::PF_X;
	load_RX.offset = sizeof(ehdr) + 2 * sizeof(ELF::Elf64_Phdr);
	load_RX.vaddr = textLoad;
	load_RX.paddr = textLoad;
	load_RX.filesz = text.size();
	load_RX.memsz = text.size();
	load_RX.align = align;
	load_RX.writeLittleEndian(output);

	for (auto b : text) {
		output << b;
	}
}
