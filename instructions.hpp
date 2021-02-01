#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <stdexcept>

// functions for writing single instructions.

// data pointer rsi

// init: set rsi to cell 0, rdx=1
// +: inc byte ptr [rsi];
// -: dec byte ptr [rsi];
// >: inc rsi;
// <: dec rsi;
// .: mov rax, 1; mov rdi, rax; syscall;
// ,: xor rax, rax; mov rdi, rax; syscall;
// [: cmp byte ptr [rsi], 0; je _____;
// ]: cmp byte ptr [rsi], 0; jne _____;

uint8_t initCode[] {
	0x48, 0xbe, // mov rsi,
	0, 0, 0, 0, 0, 0, 0, 0, // dataStart goes here
	0xba, 0x01, 0, 0, 0 // mov edx, 1. also zeroes high half.
};

uint8_t exitCode[] {
	0x48, 0x31, 0xff, // xor rdi, rdi
	0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00, // mov rax, 0x3c
	0x0f, 0x05 // syscall
};

uint8_t incCode[] = {
	0xfe, 0x06 // inc byte ptr [rsi]
};

uint8_t decCode[] = {
	0xfe, 0x0e // dec byte ptr [rsi]
};

uint8_t rightCode[] = {
	0x48, 0xff, 0xc6 // inc rsi
};

uint8_t leftCode[] = {
	0x48, 0xff, 0xce // dec rsi
};

uint8_t outputCode[] = {
	0x48, 0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00, // mov rax, 1
	0x48, 0x89, 0xc7, // mov rdi, rax
	0x0f, 0x05 // syscall
};

uint8_t inputCode[] = {
	0x48, 0x31, 0xc0, // xor rax, rax
	0x48, 0x89, 0xc7, // mov rdi, rax
	0x0f, 0x05, // syscall
};

uint8_t bracketOpenCode[] {
	0x80, 0x3e, 0x00, // cmp byte ptr [rsi], 0
	0x74, // je
	0 // jmp offset goes here
};

uint8_t bracketCloseCode[] {
	0x80, 0x3e, 0x00, // cmp byte ptr [rsi], 0
	0x75, // jne
	0 // jmp offset goes here
};

void writeInit(std::vector<uint8_t>& out, uint64_t dataStart) {
	for(int i = 0; i < 8; ++i) {
		initCode[2 + i] = (uint8_t)dataStart;
		dataStart >>= 8;
	}
	for(uint8_t b : initCode) {
		out.push_back(b);
	}
}

void writeExit(std::vector<uint8_t>& out) {
	for(uint8_t b : exitCode) {
		out.push_back(b);
	}
}

void writeInc(std::vector<uint8_t>& out) {
	for(uint8_t b : incCode) {
		out.push_back(b);
	}
}

void writeDec(std::vector<uint8_t>& out) {
	for(uint8_t b : decCode) {
		out.push_back(b);
	}	
}

void writeRight(std::vector<uint8_t>& out) {
	for(uint8_t b : rightCode) {
		out.push_back(b);
	}
}

void writeLeft(std::vector<uint8_t>& out) {
	for(uint8_t b : leftCode) {
		out.push_back(b);
	}
}

void writeOutput(std::vector<uint8_t>& out) {
	for(uint8_t b : outputCode) {
		out.push_back(b);
	}
}

void writeInput(std::vector<uint8_t>& out) {
	for(uint8_t b : inputCode) {
		out.push_back(b);
	}
}

void writeBracketOpen(std::vector<uint8_t>& out, int offset) {
	if(offset > 127) {
		throw std::runtime_error("long jump unimplemented.");
	}
	bracketOpenCode[4] = (uint8_t)(offset & 0xff);
	for(uint8_t b : bracketOpenCode) {
		out.push_back(b);
	}
}

void writeBracketClose(std::vector<uint8_t>& out, int offset) {
	if(offset < -128) {
		throw std::runtime_error("short jump unimplemented.");
	}
	bracketCloseCode[4] = (uint8_t)(offset & 0xff);
	for(uint8_t b : bracketCloseCode) {
		out.push_back(b);
	}
}

int sizeOfInst(char inst) {
	switch(inst) {
		case '+': return sizeof(incCode);
		case '-': return sizeof(decCode);
		case '>': return sizeof(rightCode);
		case '<': return sizeof(leftCode);
		case '.': return sizeof(outputCode);
		case ',': return sizeof(inputCode);
		case '[': return sizeof(bracketOpenCode);
		case ']': return sizeof(bracketCloseCode);
		default: return 0;
	}
}

#endif
