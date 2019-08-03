//
// Created by XingfengYang on 2019-08-03.
//

#ifndef LC3_VM_H
#define LC3_VM_H


#include <vector>

namespace lc3 {

    enum REG {
        R0,
        R1,
        R2,
        R3,
        R4,
        R5,
        R6,
        R7,
        PC, /* program counter */
        COND, /* condition flag */
        REG_COUNT
    };

    enum OP_CODE {
        OP_BR = 0b0000, /* branch */
        OP_ADD = 0b0001, /* add */
        OP_LD = 0b0010, /* load */
        OP_ST = 0b0011, /* store */
        OP_JSR = 0b0100, /* jump register */
        OP_AND = 0b0101, /* and */
        OP_LDR = 0b0110, /* load register */
        OP_STR = 0b0111, /* store register */
        OP_RTI = 0b1000, /* not used */
        OP_NOT = 0b1001, /* not */
        OP_LDI = 0b1010, /* load indirect */
        OP_STI = 0b1011, /* store indirect */
        OP_JMP = 0b1100, /* jump */
        OP_RES = 0b1101, /* reserved (unused) */
        OP_LEA = 0b1110, /* load effective address */
        OP_TRAP = 0b1111, /* execute trap */
    };

    enum COND_FLAG {
        FL_POS = 1 << 0, /* positive */
        FL_ZRO = 1 << 1, /* zero */
        FL_NEG = 1 << 2,  /* negative */
    };

    enum Trap{
        TRAP_GETC = 0x20, /* get char from keyboard but not echo onto terminal  */
        TRAP_OUT = 0x21, /* echo a char onto terminal */
        TRAP_PUTS = 0x22, /* echo a word string onto terminal */
        TRAP_IN = 0x23, /* get char from keyboard and echo onto terminal */
        TRAP_PUTSP = 0x24, /* output a byte string */
        TRAP_HALT = 0x25, /* halt program */
    };

    class VM {
    private:
        bool running = false;

        uint16_t *memory; /* memory */

        uint16_t registers[REG_COUNT];

    private:

        /* fetch instruction from memory */
        uint16_t fetch();

        /* signed extend */
        uint16_t signExtend(uint16_t source,uint16_t numberOfDigits);

        void updateFlag(uint16_t reg);

        /* decode instruction into opcode and args */
        uint16_t decode(uint16_t instruction);

        /* execute opcode */
        void execute(uint16_t opcode, uint16_t instruction);

        void executeTrap(uint16_t trapCode);

        /* load from memory */
        uint16_t readMemory(uint16_t address);

        /* write into memory */
        uint16_t writeMemory(uint16_t address, uint16_t value);

    public:
        VM();

        void loadProgram(std::vector<uint16_t> prog);

        void run();

        ~VM();

    };

};


#endif //LC3_VM_H
