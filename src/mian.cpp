//
// Created by XingfengYang on 2019-08-03.
//

#include <iostream>
#include "../include/vm.h"


int main(int argc, char *argv[]){
    lc3::VM vm;
    std::vector<uint16_t> prog;
    prog.push_back(0b0001000000100000); /* ADD R0 R0 0 */
    prog.push_back(0b0001011011101111); /* ADD R3 R3 15 */
    prog.push_back(0b0001011011000011); /* ADD R3 R3 R3 */
    prog.push_back(0b0001011011000011); /* ADD R3 R3 R3 */
    prog.push_back(0b0001011011100101); /* ADD R3 R3 4 */
    prog.push_back(0b0001011011100111); /* ADD R3 R3 7 */
    prog.push_back(0b0111011000000000); /* STR R3 R0 0 (h 72)*/
    prog.push_back(0b0001011011111101); /* ADD R3 R3 -3 */
    prog.push_back(0b0111011000000001); /* STR R3 R0 1 (e 69)*/
    prog.push_back(0b0001011011100111); /* ADD R3 R3 7 */
    prog.push_back(0b0111011000000010); /* STR R3 R0 2 (l 76)*/
    prog.push_back(0b0111011000000011); /* STR R3 R0 3 (l 76)*/
    prog.push_back(0b0001011011100011); /* ADD R3 R3 3 */
    prog.push_back(0b0111011000000100); /* STR R3 R0 4 (o 79)*/
    prog.push_back(0b0001011011110000); /* ADD R3 R3 -16 */
    prog.push_back(0b0001011011110000); /* ADD R3 R3 -16 */
    prog.push_back(0b0001011011111101); /* ADD R3 R3 -3 */
    prog.push_back(0b0111011000000101); /* STR R3 R0 5 (, 44)*/
    prog.push_back(0b0001011011101111); /* ADD R3 R3 15 */
    prog.push_back(0b0001011011101111); /* ADD R3 R3 15 */
    prog.push_back(0b0001011011101101); /* ADD R3 R3 13 */
    prog.push_back(0b0111011000000110); /* STR R3 R0 6 (w 87)*/
    prog.push_back(0b0001011011111000); /* ADD R3 R3 -8 */
    prog.push_back(0b0111011000000111); /* STR R3 R0 7 (o 79)*/
    prog.push_back(0b0001011011100011); /* ADD R3 R3 3 */
    prog.push_back(0b0111011000001000); /* STR R3 R0 8 (r 82)*/
    prog.push_back(0b0001011011111010); /* ADD R3 R3 -6 */
    prog.push_back(0b0111011000001001); /* STR R3 R0 9 (l 76)*/
    prog.push_back(0b0001011011111000); /* ADD R3 R3 -8 */
    prog.push_back(0b0111011000001010); /* STR R3 R0 10 (d 68)*/
    prog.push_back(0b0001011011110000); /* ADD R3 R3 -16 */
    prog.push_back(0b0001011011110000); /* ADD R3 R3 -16 */
    prog.push_back(0b0001011011111101); /* ADD R3 R3 -3 */
    prog.push_back(0b0111011000001011); /* STR R3 R0 11 (! 33)*/
    prog.push_back(0b0001011011110000); /* ADD R3 R3 -16 */
    prog.push_back(0b0001011011111011); /* ADD R3 R3 -5 */
    prog.push_back(0b0111011000001100); /* STR R3 R0 12 (enter 10)*/
    prog.push_back(0b0001001001100000); /* ADD R1 R1 0 */
    prog.push_back(0b0111011000101101); /* STR R3 R1 13 */
    prog.push_back(0b1111000000100010); /* trap puts R0 */
    prog.push_back(0b0001000000100000); /* ADD R0 R0 0 */
    prog.push_back(0b0001001001101111); /* ADD R1 R1 15 */
    prog.push_back(0b0001000001000001); /* ADD R0 R1 R1 */
    prog.push_back(0b0001000000100010); /* ADD R0 R0 2 */
    prog.push_back(0b1111000000100001); /* trap out R0 */
    for(int i = 0;i<94;i++) {
        prog.push_back(0b0001000000100001); /* ADD R0 R0 1 */
        prog.push_back(0b1111000000100001); /* trap out R0 */
    }
    prog.push_back(0b1111000000100011); /* trap in */
    prog.push_back(0b1111000000100011); /* trap in */
    prog.push_back(0b1111000000100011); /* trap in */
    prog.push_back(0b1111000000100011); /* trap in */
    prog.push_back(0b1111000000100011); /* trap in */
    prog.push_back(0b1111000000100101); /* trap halt */
    vm.loadProgram(prog);
    vm.run();

    return 0;
}
