//
// Created by XingfengYang on 2019-08-03.
//
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <bitset>
#include "../include/vm.h"


using namespace lc3;

VM::VM() {
    /** init memory,memory map
     *  0x00 - 0xFF : trap vector table
     *  0x100 - 0x1FF : interrupt vector table
     *  0x200 - 0x2FF : Operating system and supervisor stack
     *  0x300 - 0xFDFF : user program
     *  0xFE00 -  0xFFFF : device register address
     */
    this->memory = (uint16_t *) malloc(UINT16_MAX * sizeof(uint16_t));
}

unsigned short VM::fetch() {
    return this->readMemory(this->registers[PC]++);
}

uint16_t VM::decode(uint16_t instruction) {
    uint16_t opCode = instruction >> 12;
    return opCode;
}


uint16_t VM::signExtend(uint16_t source, uint16_t numberOfDigits) {
    if ((source >> (numberOfDigits - 1)) & 1) {
        source |= (0xFFFF << numberOfDigits);
    }
    return source;
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

    // todo : refactor
    switch (opcode) {
        case OP_BR: {
            uint16_t pcOffset = this->signExtend(instruction & 0b111111111, 9);
            uint16_t condFlag = instruction & (0b111 << 9);
            if (this->registers[COND] & condFlag) {
                this->registers[PC] += pcOffset;
            }
            break;
        }
        case OP_ADD: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t SR1 = (instruction >> 6) & 0b111;
            uint16_t immFlag = (instruction >> 5) & 0b1;
            if (immFlag) { /* immediate */
                uint16_t imm5 = this->signExtend(instruction & 0b11111, 5);
                this->registers[DR] = this->registers[SR1] + imm5;
            } else {
                uint16_t SR2 = instruction & (0b111);
                this->registers[DR] = this->registers[SR1] + this->registers[SR2];
            }
            this->updateFlag(DR); /* update cpu flag */

            break;
        }
        case OP_LD: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t pcOffset = this->signExtend(instruction & 0b111111111, 9);
            this->registers[DR] = this->readMemory(this->registers[PC] + pcOffset);
            this->updateFlag(DR);
            break;
        }
        case OP_ST: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t pcOffset = this->signExtend(instruction & 0b111111111, 9);
            this->writeMemory(this->registers[PC] + pcOffset, this->registers[DR]);
            break;
        }
        case OP_JSR: {
            uint16_t flag = (instruction >> 11) & 0b1;
            this->registers[R7] = this->registers[PC];
            if (flag) {  /* JSR */
                this->registers[PC] += this->signExtend(instruction & (0b11111111111), 11);
            } else { /* JSRR */
                uint16_t BaseR = (instruction >> 6) & 0b111;
                this->registers[PC] = this->registers[BaseR];
            }
            break;
        }
        case OP_AND: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t SR1 = (instruction >> 6) & 0b111;
            uint16_t immFlag = (instruction >> 5) & 0b1;
            if (immFlag) {
                this->registers[DR] = this->registers[SR1] & this->signExtend(instruction & (0b11111), 5);
            } else {
                uint16_t SR2 = instruction & 0b111;
                this->registers[DR] = this->registers[SR1] & this->registers[SR2];
            }
            this->updateFlag(DR);
            break;
        }
        case OP_LDR: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t BaseR = (instruction >> 6) & 0b111;
            uint16_t offset = this->signExtend(instruction & 0b111111, 6);
            this->registers[DR] = this->readMemory(this->registers[BaseR] + offset);
            this->updateFlag(DR);
            break;
        }
        case OP_STR: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t BaseR = (instruction >> 6) & 0b111;
            uint16_t offset = this->signExtend(instruction & 0b111111, 6);
            this->writeMemory(this->registers[BaseR] + offset, this->registers[DR]);
            break;
        }
        case OP_RTI: {
            /* not used now */
            break;
        }
        case OP_NOT: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t SR1 = (instruction >> 6) & 0b111;
            this->registers[DR] = ~this->registers[SR1];
            this->updateFlag(DR);
            break;
        }
        case OP_LDI: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t offset = this->signExtend(instruction & 0b111111111, 9);
            this->registers[DR] = this->readMemory(this->readMemory(this->registers[PC] + offset));
            this->updateFlag(DR);
            break;
        }
        case OP_STI: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t offset = this->signExtend(instruction & 0b111111111, 9);
            this->writeMemory(this->readMemory(this->registers[PC] + offset), this->registers[DR]);
            break;
        }
        case OP_JMP: {
            uint16_t BaseR = (instruction >> 6) & 0b111;
            this->registers[PC] = this->registers[BaseR];
            break;
        }
        case OP_RES: {
            /* not used now */
            /* initiate an illegal opcode exception */

            break;
        }
        case OP_LEA: {
            uint16_t DR = (instruction >> 9) & 0b111;
            uint16_t offset = this->signExtend(instruction & 0b111111111, 9);
            this->registers[DR] = this->registers[PC] + offset;
            this->updateFlag(DR);
            break;
        }
        case OP_TRAP: { /* some I/O */
            uint16_t trapCode = instruction & 0b11111111;
            this->executeTrap(trapCode);
        }
    }

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
    switch (trapCode) {
        case TRAP_GETC: {
            this->registers[R0] = (uint16_t) getchar();
            break;
        }
        case TRAP_OUT: {
            std::putc((char) this->registers[R0], stdout);
            std::fflush(stdout);
            break;
        }
        case TRAP_PUTS: {
            uint16_t offset = this->registers[R0];
            uint16_t c = this->readMemory(offset);
            while (c) {
                std::putc((char) c, stdout);
                ++offset;
                c = this->readMemory(offset);
            }
            std::fflush(stdout);
            break;
        }
        case TRAP_IN: {
            char c = getchar();
            std::putc(c, stdout);
            this->registers[R0] = (uint16_t) c;
            break;
        }
        case TRAP_PUTSP: {
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
            break;
        }
        case TRAP_HALT: {
            std::cout << std::endl;
            std::fflush(stdout);
            this->running = false;
            break;
        }
    }
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



