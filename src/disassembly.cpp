//
// Created by XingfengYang on 2019-08-05.
//

#include <iostream>
#include "../include/disassembly.h"
#include "../include/common.h"
#include "../include/instruction.h"


using namespace lc3;

DisAssembly::DisAssembly() {

}


void DisAssembly::loadProgram(std::vector<uint16_t> prog) {
    this->prog = prog;
}

void DisAssembly::disAssembly() {
    std::cout << "SIZE:" << prog.size() << std::endl;
    for (int i = 0; i < this->prog.size(); i++) {
        uint16_t instruction = swapEdition(prog.at(i));
        std::cout << std::hex << instruction << std::endl;
//        uint16_t opCode = (instruction >> 12 & 0b1111);
//        switch (opCode) {
//            case OP_BR: {
//                uint16_t pcOffset = signExtend(instruction & 0b111111111, 9);
////                std::cout << "BR " << pcOffset << std::endl;
//                break;
//            }
//            case OP_ADD: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t SR1 = (instruction >> 6) & 0b111;
//                uint16_t immFlag = (instruction >> 5) & 0b1;
//                if (immFlag) { /* immediate */
////                    std::cout << "ADD R" << DR << " R" << SR1 << " " << signExtend((instruction & 0x11111), 5)
////                              << std::endl;
//                } else {
//                    uint16_t SR2 = instruction & (0b111);
////                    std::cout << "ADD R" << DR << " R" << SR1 << " R" << SR2 << std::endl;
//                }
//                break;
//            }
//            case OP_LD: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t pcOffset = signExtend(instruction & 0b111111111, 9);
//
//                break;
//            }
//            case OP_ST: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t pcOffset = signExtend(instruction & 0b111111111, 9);
//
//                break;
//            }
//            case OP_JSR: {
//                uint16_t flag = (instruction >> 11) & 0b1;
//                if (flag) {  /* JSR */
//
//                } else { /* JSRR */
//                    uint16_t BaseR = (instruction >> 6) & 0b111;
//
//                }
//                break;
//            }
//            case OP_AND: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t SR1 = (instruction >> 6) & 0b111;
//                uint16_t immFlag = (instruction >> 5) & 0b1;
//                if (immFlag) {
//
//                } else {
//                    uint16_t SR2 = instruction & 0b111;
//
//                }
//                break;
//            }
//            case OP_LDR: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t BaseR = (instruction >> 6) & 0b111;
//                uint16_t offset = signExtend(instruction & 0b111111, 6);
//
//                break;
//            }
//            case OP_STR: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t BaseR = (instruction >> 6) & 0b111;
//                uint16_t offset = signExtend(instruction & 0b111111, 6);
//
//                break;
//            }
//            case OP_RTI: {
//                /* not used now */
//                break;
//            }
//            case OP_NOT: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t SR1 = (instruction >> 6) & 0b111;
//
//                break;
//            }
//            case OP_LDI: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t offset = signExtend(instruction & 0b111111111, 9);
//
//                break;
//            }
//            case OP_STI: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t offset = signExtend(instruction & 0b111111111, 9);
//
//                break;
//            }
//            case OP_JMP: {
//                uint16_t BaseR = (instruction >> 6) & 0b111;
//
//                break;
//            }
//            case OP_RES: {
//                /* not used now */
//                /* initiate an illegal opcode exception */
//
//                break;
//            }
//            case OP_LEA: {
//                uint16_t DR = (instruction >> 9) & 0b111;
//                uint16_t offset = signExtend(instruction & 0b111111111, 9);
//
//                break;
//            }
//            case OP_TRAP: { /* some I/O */
//                uint16_t trapCode = instruction & 0b11111111;
//            }
//        }
    }
}

void DisAssembly::writeToFile(const char *outputFileName) {

}


DisAssembly::~DisAssembly() {
}


