#include "bin-packing/packer.h"
#include <iostream>

using std::cout;
using std::endl;

int main() {
    Block* blocks[] = { new Block{ 16, 16 }, new Block{ 8, 8 }, new Block{ 4, 4 } };
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
