/******************************************************************************
This is a very simple binary tree based bin packing algorithm that is initialized
with a fixed width and height and will fit each block into the first node where
it fits and then split that node into 2 parts (down and right) to track the
remaining whitespace.
Best results occur when the input blocks are sorted by height, or even better
when sorted by max(width,height).
Inputs:
------
  w:      width of target rectangle
  h:      height of target rectangle
  blocks: array of any objects that have .w and .h attributes
Outputs:
-------
  marks each block that fits with a .fit attribute pointing to a
  node with .x and .y coordinates
Example:
-------
  Block* blocks[] = {
    new Block{ 100, 100 },
    new Block{ 100, 100 },
    new Block{  80,  80 },
    new Block{  80,  80 },
    etc
    etc
  };
  Packer packer(500, 500);
  packer.fit(blocks);
  for(int n = 0 ; n < blocks.length ; ++n) {
    Block* block = blocks[n];
    if (block->fit) {
      Draw(block->fit->x, block->fit->y, block->w, block->h);
    }
  }
  for(int n = 0 ; n < blocks.length ; ++n) {
    delete blocks[n];
    blocks[n] = nullptr;
  }
******************************************************************************/

#pragma once
#include "stdafx.h"

class BINPACKING_API Node {
public:
    int x, y, w, h;
    bool used;
    Node* down;
    Node* right;

    ~Node();
};
class BINPACKING_API Block {
public:
    int w, h;
    Node* fit;

    ~Block();
};
class BINPACKING_API Packer {
public:
    int w, h;
    Node* root;

    Packer(int __w, int __h);
    void fit(Block** __blocks, int __numBlocks);
    template<int __numBlocks>
    void fit(Block* (&__blocks)[__numBlocks]) {
        fit(__blocks, __numBlocks);
    }
    Node* findNode(Node* __root, int __w, int __h);
    Node* splitNode(Node* __node, int __w, int __h);
    ~Packer();
};
