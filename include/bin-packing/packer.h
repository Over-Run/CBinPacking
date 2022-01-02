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
