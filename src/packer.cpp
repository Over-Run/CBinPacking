#include "bin-packing/stdafx.h"
#include "bin-packing/packer.h"

Node::~Node() {
    if (right) {
        delete right;
        right = nullptr;
    }
    if (down) {
        delete down;
        down = nullptr;
    }
}

Block::~Block() {
    delete fit;
    fit = nullptr;
}

Packer::Packer(int __w, int __h) :
    w(__w),
    h(__h),
    root(new Node{ 0, 0, __w, __h }) {}
void Packer::fit(Block** __blocks, int __numBlocks) {
    Node* __node;
    Block* __block;
    for (int i = 0; i < __numBlocks; ++i) {
        __block = __blocks[i];
        if (__node = findNode(root, __block->w, __block->h)) {
            __block->fit = splitNode(__node, __block->w, __block->h);
        }
    }
}
Node* Packer::findNode(Node* __root, int __w, int __h) {
    if (__root->used) {
        Node* __node = findNode(__root->right, __w, __h);
        return __node ? __node : findNode(__root->down, __w, __h);
    }
    if ((__w <= __root->w) && (__h <= __root->h)) {
        return __root;
    }
    return nullptr;
}
Node* Packer::splitNode(Node* __node, int __w, int __h) {
    __node->used = true;
    if (__node->down) {
        delete __node->down;
    }
    if (__node->right) {
        delete __node->right;
    }
    __node->down = new Node{ __node->x, __node->y + __h, __node->w, __node->h - __h };
    __node->right = new Node{ __node->x + __w, __node->y, __node->w - __w, __h };
    return __node;
}
Packer::~Packer() {
    delete root;
    root = nullptr;
}
