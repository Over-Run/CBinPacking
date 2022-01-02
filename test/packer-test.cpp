#define BINPACKING_IMPLEMENTATION
#include "bin-packing/packer.h"
#include <iostream>

using std::cout;
using std::endl;
using BinPacking::Block;
using BinPacking::Packer;
using BinPacking::GrowingPacker;

void packer() {
    Block* blocks[] = { new Block(16, 16), new Block(8, 8), new Block(4, 4) };
    Packer packer(24, 16);
    packer.fit(blocks);
    for (int i = 0; i < 3; ++i) {
        Block* block = blocks[i];
        if (block->fit) {
            cout << "Num: " << i
                << ", Fit: (" << block->fit->x << ", " << block->fit->y
                << "), Size: (" << block->w << ", " << block->h << ")"
                << endl;
        }
    }
    for (int i = 0; i < 3; ++i) {
        delete blocks[i];
        blocks[i] = nullptr;
    }
}

void growingPacker() {
    Block* blocks[] = { new Block(100, 100), new Block(80, 80), new Block(46, 46), new Block(20, 30) };
    GrowingPacker packer;
    packer.fit(blocks);
    for (int i = 0; i < 4; ++i) {
        Block* block = blocks[i];
        if (block->fit) {
            cout << "Num: " << i
                << ", Fit: (" << block->fit->x << ", " << block->fit->y
                << "), Size: (" << block->w << ", " << block->h << ")"
                << endl;
        }
    }
    for (int i = 0; i < 4; ++i) {
        delete blocks[i];
        blocks[i] = nullptr;
    }
}

int main() {
    packer();
    growingPacker();
    return 0;
}
