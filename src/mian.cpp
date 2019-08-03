//
// Created by XingfengYang on 2019-08-03.
//

#include <iostream>
#include "../include/vm.h"


int main(int argc, char *argv[]){

    std::cout<<"=========lc3=========="<<std::endl;
    lc3::VM vm;
    std::vector<uint16_t> prog;
    prog.push_back(0b0001000000100000); /* ADD R0 R0 0 */
    prog.push_back(0b0001001001101111); /* ADD R1 R1 15 */
    prog.push_back(0b0001000001000001); /* ADD R0 R1 R1 */
    prog.push_back(0b0001000000100010); /* ADD R0 R0 2 */
    prog.push_back(0b1111000000100001); /* trap out R0 */
    for(int i = 0;i<94;i++) {
        prog.push_back(0b0001000000100001); /* ADD R0 R0 1 */
        prog.push_back(0b1111000000100001); /* trap out R0 */
    }
    prog.push_back(0b1111000000100101); /* trap halt */
    vm.loadProgram(prog);
    vm.run();

    return 0;
}
