//
// Created by XingfengYang on 2019-08-03.
//

#include <iostream>
#include "../include/vm.h"
#include "../include/bin_loader.h"
#include "../include/disassembly.h"

int main(int argc, char *argv[]) {


    lc3::BinLoader binLoader;
    if (argc <= 1) {
        std::cout << "Error: No file input." << std::endl;
        exit(0);
    }

    binLoader.loadBinFile(argv[1]);
    std::vector<uint16_t> prog = binLoader.getBinaryBuffer();

    lc3::VM vm;
//    lc3::DisAssembly disAssembly;
//    disAssembly.loadProgram(prog);
//    disAssembly.disAssembly();

    vm.loadProgram(prog);
    vm.run();


    return 0;
}
