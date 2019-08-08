//
// Created by XingfengYang on 2019-08-03.
//
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <bitset>

#include "../include/vm.h"
#include "../include/common.h"
#include "../include/instruction.h"

using namespace lc3;

typedef void (VM::*opHandler)(uint16_t);

typedef struct VM::OpINS OpINS;

typedef void (VM::*trapHandler)(uint16_t);

typedef struct TrapINS TrapINS;


VM::VM() {
    /** init memory,memory map
     *  0x00 - 0xFF : trap vector table
     *  0x100 - 0x1FF : interrupt vector table
     *  0x200 - 0x2FF : Operating system and supervisor stack
     *  0x300 - 0xFDFF : user program
     *  0xFE00 -  0xFFFF : device register address
     */
    this->memory = (uint16_t *) malloc(UINT16_MAX * sizeof(uint16_t));
    this->initHandlerTable();
}

void VM::Handle_BR(uint16_t instruction) {
    uint16_t pcOffset = signExtend(instruction & 0b111111111, 9);
    uint16_t condFlag = instruction & (0b111 << 9);
    if (this->registers[COND] & condFlag) {
        this->registers[PC] += pcOffset;
    }
} /* branch */

void VM::Handle_ADD(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t SR1 = (instruction >> 6) & 0b111;
    uint16_t immFlag = (instruction >> 5) & 0b1;
    if (immFlag) { /* immediate */
        uint16_t imm5 = signExtend(instruction & 0b11111, 5);
        this->registers[DR] = this->registers[SR1] + imm5;
    } else {
        uint16_t SR2 = instruction & (0b111);
        this->registers[DR] = this->registers[SR1] + this->registers[SR2];
    }
    this->updateFlag(DR); /* update cpu flag */
} /* add */

void VM::Handle_LD(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t pcOffset = signExtend(instruction & 0b111111111, 9);
    this->registers[DR] = this->readMemory(this->registers[PC] + pcOffset);
    this->updateFlag(DR);
} /* load */

void VM::Handle_ST(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t pcOffset = signExtend(instruction & 0b111111111, 9);
    this->writeMemory(this->registers[PC] + pcOffset, this->registers[DR]);
}/* store */

void VM::Handle_JSR(uint16_t instruction) {
    uint16_t flag = (instruction >> 11) & 0b1;
    this->registers[R7] = this->registers[PC];
    if (flag) {  /* JSR */
        this->registers[PC] += signExtend(instruction & (0b11111111111), 11);
    } else { /* JSRR */
        uint16_t BaseR = (instruction >> 6) & 0b111;
        this->registers[PC] = this->registers[BaseR];
    }
} /* jump register */

void VM::Handle_AND(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t SR1 = (instruction >> 6) & 0b111;
    uint16_t immFlag = (instruction >> 5) & 0b1;
    if (immFlag) {
        this->registers[DR] = this->registers[SR1] & signExtend(instruction & (0b11111), 5);
    } else {
        uint16_t SR2 = instruction & 0b111;
        this->registers[DR] = this->registers[SR1] & this->registers[SR2];
    }
    this->updateFlag(DR);
} /* and */

void VM::Handle_LDR(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t BaseR = (instruction >> 6) & 0b111;
    uint16_t offset = signExtend(instruction & 0b111111, 6);
    this->registers[DR] = this->readMemory(this->registers[BaseR] + offset);
    this->updateFlag(DR);
} /* load register */

void VM::Handle_STR(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t BaseR = (instruction >> 6) & 0b111;
    uint16_t offset = signExtend(instruction & 0b111111, 6);
    this->writeMemory(this->registers[BaseR] + offset, this->registers[DR]);
} /* store register */

void VM::Handle_RTI(uint16_t instruction) {

} /* not used */

void VM::Handle_NOT(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t SR1 = (instruction >> 6) & 0b111;
    this->registers[DR] = ~this->registers[SR1];
    this->updateFlag(DR);
} /* not */

void VM::Handle_LDI(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t offset = signExtend(instruction & 0b111111111, 9);
    this->registers[DR] = this->readMemory(this->readMemory(this->registers[PC] + offset));
    this->updateFlag(DR);
} /* load indirect */

void VM::Handle_STI(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t offset = signExtend(instruction & 0b111111111, 9);
    this->writeMemory(this->readMemory(this->registers[PC] + offset), this->registers[DR]);
} /* store indirect */

void VM::Handle_JMP(uint16_t instruction) {
    uint16_t BaseR = (instruction >> 6) & 0b111;
    this->registers[PC] = this->registers[BaseR];
} /* jump */

void VM::Handle_RES(uint16_t instruction) {
} /* reserved (unused) */

void VM::Handle_LEA(uint16_t instruction) {
    uint16_t DR = (instruction >> 9) & 0b111;
    uint16_t offset = signExtend(instruction & 0b111111111, 9);
    this->registers[DR] = this->registers[PC] + offset;
    this->updateFlag(DR);
} /* load effective address */

void VM::Handle_TRAP(uint16_t instruction) {
    uint16_t trapCode = instruction & 0b11111111;
    this->executeTrap(trapCode);
} /* trap */

void VM::Handle_TRAP_GETC(uint16_t op) {
    this->registers[R0] = (uint16_t) getchar();
} /* get char from keyboard but not echo onto terminal  */

void VM::Handle_TRAP_OUT(uint16_t op) {
    std::putc((char) this->registers[R0], stdout);
    std::fflush(stdout);
} /* echo a char onto terminal */

void VM::Handle_TRAP_PUTS(uint16_t op) {
    uint16_t offset = this->registers[R0];
    uint16_t c = this->readMemory(offset);
    while (c) {
        std::putc((char) c, stdout);
        ++offset;
        c = this->readMemory(offset);
    }
    std::fflush(stdout);
} /* echo a word string onto terminal */

void VM::Handle_TRAP_IN(uint16_t op) {
    char c = getchar();
    std::putc(c, stdout);
    this->registers[R0] = (uint16_t) c;
} /* get char from keyboard and echo onto terminal */

void VM::Handle_TRAP_PUTSP(uint16_t op) {
    uint16_t *c = this->memory + this->registers[R0];
    while (*c) {
        char c1 = *c & 0xFF;
        std::putc(c1, stdout);
        char c2 = *c >> 8;
        if (c2) {
            std::putc(c2, stdout);
        }
        ++c;
    }
    std::fflush(stdout);
} /* output a byte string */

void VM::Handle_TRAP_HALT(uint16_t op) {
    std::cout << std::endl;
    std::fflush(stdout);
    this->running = false;
} /* halt program */

void VM::initHandlerTable() {
    std::map<uint16_t, OpINS>::iterator it = this->opHandlerTable.begin();
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_BR, {OP_BR, 1, "BR", &VM::Handle_BR}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_ADD, {OP_ADD, 1, "ADD", &VM::Handle_ADD}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_LD, {OP_LD, 1, "LD", &VM::Handle_LD}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_ST, {OP_ST, 1, "ST", &VM::Handle_ST}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_JSR, {OP_JSR, 1, "JSR", &VM::Handle_JSR}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_AND, {OP_AND, 1, "AND", &VM::Handle_AND}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_LDR, {OP_LDR, 1, "LDR", &VM::Handle_LDR}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_STR, {OP_STR, 1, "STR", &VM::Handle_STR}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_RTI, {OP_RTI, 1, "RTI", &VM::Handle_RTI}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_NOT, {OP_NOT, 1, "NOT", &VM::Handle_NOT}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_LDI, {OP_LDI, 1, "LDI", &VM::Handle_LDI}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_STI, {OP_STI, 1, "STI", &VM::Handle_STI}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_JMP, {OP_JMP, 1, "JMP", &VM::Handle_JMP}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_RES, {OP_RES, 1, "RES", &VM::Handle_RES}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_LEA, {OP_LEA, 1, "LEA", &VM::Handle_LEA}));
    this->opHandlerTable.insert(it, std::pair<uint16_t, OpINS>(OP_TRAP, {OP_TRAP, 1, "TRAP", &VM::Handle_TRAP}));

    std::map<uint16_t, TrapINS>::iterator trapIt = this->trapHandlerTable.begin();

    this->trapHandlerTable.insert(trapIt, std::pair<uint16_t, TrapINS>(TRAP_GETC,
                                                                       {TRAP_GETC, "TRAP_GETC",
                                                                        &VM::Handle_TRAP_GETC}));
    this->trapHandlerTable.insert(trapIt, std::pair<uint16_t, TrapINS>(TRAP_OUT,
                                                                       {TRAP_OUT, "TRAP_OUT",
                                                                        &VM::Handle_TRAP_OUT}));
    this->trapHandlerTable.insert(trapIt, std::pair<uint16_t, TrapINS>(TRAP_PUTS,
                                                                       {TRAP_PUTS, "TRAP_PUTS",
                                                                        &VM::Handle_TRAP_PUTS}));
    this->trapHandlerTable.insert(trapIt, std::pair<uint16_t, TrapINS>(TRAP_IN,
                                                                       {TRAP_IN, "TRAP_IN",
                                                                        &VM::Handle_TRAP_IN}));
    this->trapHandlerTable.insert(trapIt, std::pair<uint16_t, TrapINS>(TRAP_PUTSP,
                                                                       {TRAP_PUTSP, "TRAP_PUTSP",
                                                                        &VM::Handle_TRAP_PUTSP}));
    this->trapHandlerTable.insert(trapIt, std::pair<uint16_t, TrapINS>(TRAP_HALT,
                                                                       {TRAP_HALT, "TRAP_HALT",
                                                                        &VM::Handle_TRAP_HALT}));
}

unsigned short VM::fetch() {
    return this->readMemory(this->registers[PC]++);
}

uint16_t VM::decode(uint16_t instruction) {
    uint16_t opCode = instruction >> 12;
    return opCode;
}

void VM::updateFlag(uint16_t reg) {
    if (this->registers[reg] == 0) {
        this->registers[COND] = FL_ZRO;
    } else if (this->registers[reg] >> 15) {
        this->registers[COND] = FL_NEG;
    } else {
        this->registers[COND] = FL_POS;
    }
}

void VM::execute(uint16_t opcode, uint16_t instruction) {
    OpINS opIns = this->opHandlerTable[opcode];
    void (VM::*opHdr)(uint16_t) = opIns.opHandler;
    (this->*opHdr)(instruction);

    if (this->registers[PC] >= UINT16_MAX) {
        this->running = false;
    }
}

void VM::printRegister() {
    std::cout << std::endl;
    std::cout << "REGISTERS: " << "R0:" << this->registers[R0] << " "
              << "R1:" << this->registers[R1] << " "
              << "R2:" << this->registers[R2] << " "
              << "R3:" << this->registers[R3] << " "
              << "R4:" << this->registers[R4] << " "
              << "R5:" << this->registers[R5] << " "
              << "R6:" << this->registers[R6] << " "
              << "R7:" << this->registers[R7] << " "
              << "PC:" << this->registers[PC] << " "
              << "COND:" << this->registers[COND] << std::endl;
}

void VM::executeTrap(uint16_t trapCode) {
    TrapINS trapIns = this->trapHandlerTable[trapCode];
    void (VM::*trapHandler)(uint16_t) = trapIns.trapHandler;
    (this->*trapHandler)(trapCode);
}

void VM::loadProgram(std::vector<uint16_t> prog) {
    for (int i = 0; i < prog.size(); i++) {
        this->writeMemory(0x3000 + i, prog.at(i));
    }
}

uint16_t VM::readMemory(uint16_t address) {
    return this->memory[address];
}

uint16_t VM::writeMemory(uint16_t address, uint16_t value) {
    this->memory[address] = value;
    return value;
}


void VM::run() {
    this->registers[PC] = 0x3000; /* pc start */

    this->running = true;
    while (this->running) {
        uint16_t instruction = this->fetch();
        uint16_t opcode = this->decode(instruction);
        this->execute(opcode, instruction);
//        this->printRegister();
    }
}

VM::~VM() {

}



