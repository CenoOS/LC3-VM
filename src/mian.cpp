//
// Created by XingfengYang on 2019-08-03.
//

#include <iostream>
#include "../include/vm.h"


int main(int argc, char *argv[]){


    /**
     * ADD R0,R0,0
     * ADD R1,R1,3
     * ADD R0,R1,R1
     */
    std::vector<uint16_t> prog;

    prog.push_back(0b0001000000100000); /* ADD R0 R0 0 */
    prog.push_back(0b0001001001101111); /* ADD R1 R1 15 */
    prog.push_back(0b0001000001000001); /* ADD R0 R1 R0 */
    prog.push_back(0b1111000000100001); /* trap out R0 */
    prog.push_back(0b0001000000100001); /* ADD R0 R0 1 */
    prog.push_back(0b1111000000100001); /* trap out R0 */
    prog.push_back(0b1111000000100101); /* trap halt */

    std::cout<<"=========lc3========"<<std::endl;
    lc3::VM vm;
    vm.loadProgram(prog);
    vm.run();


    return 0;
}
