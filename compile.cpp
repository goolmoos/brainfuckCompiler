#include "compile.hpp"
#include "instructions.hpp"

#include <vector>
#include <stack>

using namespace std;

vector<int> calcBracketJumpAddr(vector<char>& bfCode);

vector<uint8_t> compile(vector<char>& bfCode, uint64_t dataStart) {
	vector<uint8_t> out;
	writeInit(out, dataStart);
	vector<int> bracketJumpAddr = calcBracketJumpAddr(bfCode); // index i contains jump addr for ith bracket
	int bracketNum = -1;
	int curr_addr = 0;
	
	int from;
	int to;

	for(char c : bfCode) {
		switch(c) {
			case '+':
				writeInc(out);
				break;
			case '-':
				writeDec(out);
				break;
			case '>':
				writeRight(out);
				break;
			case '<':
				writeLeft(out);
				break;
			case '.':
				writeOutput(out);
				break;
			case ',':
				writeInput(out);
				break;
			case '[':
				to = bracketJumpAddr[++bracketNum];
				from = curr_addr + sizeOfInst('[');
				writeBracketOpen(out, to - from);
				break;
			case ']':
				to = bracketJumpAddr[++bracketNum];
				from = curr_addr + sizeOfInst(']');
				writeBracketClose(out, to - from);
				break;

		}
		curr_addr += sizeOfInst(c);
	}
	writeExit(out);
	return out;
}

vector<int> calcBracketJumpAddr(vector<char>& bfCode) {
	// return vector containing jump addr for each bracket.
	vector<int> bracketJumpAddr; // index i contains pair of 
	stack<pair<int, int>> openBrackets; // contains the open brackets. pair of bracket index and addr.
	int addr = 0; // relative to first brainfuck inst
	for(char c : bfCode) {
		if(c == '[') {
			int bracketNum = bracketJumpAddr.size();
			bracketJumpAddr.push_back(-1); // dummy val, filled by ]
			openBrackets.push(pair<int, int>(bracketNum, addr));
		} else if(c == ']') {
			if(openBrackets.empty()) {
				throw runtime_error("unexpected ]");
			}
			pair<int, int> match = openBrackets.top(); // num, addr
			openBrackets.pop();
			bracketJumpAddr[match.first] = addr + sizeOfInst(']'); // [ jumps to after this
			bracketJumpAddr.push_back(match.second + sizeOfInst('[')); // this jumps to after [
		}
		addr += sizeOfInst(c);
	}
	if(!openBrackets.empty()) {
		throw runtime_error("expected ]");
	}
	return bracketJumpAddr;
}
