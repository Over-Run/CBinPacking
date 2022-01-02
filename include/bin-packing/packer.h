#pragma once

namespace BinPacking {
    class Node {
    public:
        bool used;
        int x, y, w, h;
        Node* down;
        Node* right;

        Node(int __x, int __y, int __w, int __h);
        Node(bool __used, int __x, int __y, int __w, int __h, Node* __down, Node* __right);
        ~Node();
    };
    class Block {
    public:
        int w, h;
        Node* fit;

        Block(int __w, int __h);
    };
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
        new Block(100, 100),
        new Block(100, 100),
        new Block( 80,  80),
        new Block( 80,  80),
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
      for (int i = 0; i < blocks.length; ++i) {
        delete blocks[i];
        blocks[i] = nullptr;
      }
    ******************************************************************************/
    class Packer {
    public:
        int w, h;
        Node* root;

        Packer(int __w, int __h);
        void fit(Block** __blocks, size_t __numBlocks);
        template<size_t __numBlocks>
        void fit(Block* (&__blocks)[__numBlocks]) {
            fit(__blocks, __numBlocks);
        }
        Node* findNode(Node* __root, int __w, int __h);
        Node* splitNode(Node* __node, int __w, int __h);
        ~Packer();
    };
    /******************************************************************************
    This is a binary tree based bin packing algorithm that is more complex than
    the simple Packer (packer.js). Instead of starting off with a fixed width and
    height, it starts with the width and height of the first block passed and then
    grows as necessary to accomodate each subsequent block. As it grows it attempts
    to maintain a roughly square ratio by making 'smart' choices about whether to
    grow right or down.
    When growing, the algorithm can only grow to the right OR down. Therefore, if
    the new block is BOTH wider and taller than the current target then it will be
    rejected. This makes it very important to initialize with a sensible starting
    width and height. If you are providing sorted input (largest first) then this
    will not be an issue.
    A potential way to solve this limitation would be to allow growth in BOTH
    directions at once, but this requires maintaining a more complex tree
    with 3 children (down, right and center) and that complexity can be avoided
    by simply chosing a sensible starting block.
    Best results occur when the input blocks are sorted by height, or even better
    when sorted by max(width,height).
    Inputs:
    ------
      blocks: array of any objects that have .w and .h attributes
    Outputs:
    -------
      marks each block that fits with a .fit attribute pointing to a
      node with .x and .y coordinates
    Example:
    -------
      Block* blocks[] = {
        new Block(100, 100),
        new Block(100, 100),
        new Block( 80,  80),
        new Block( 80,  80),
        etc
        etc
      };
      GrowingPacker packer;
      packer.fit(blocks);
      for(int n = 0 ; n < blocks.length ; ++n) {
        Block* block = blocks[n];
        if (block->fit) {
          Draw(block->fit->x, block->fit->y, block->w, block->h);
        }
      }
      for (int i = 0; i < blocks.length; ++i) {
        delete blocks[i];
        blocks[i] = nullptr;
      }
    ******************************************************************************/
    class GrowingPacker {
    public:
        Node* root;

        GrowingPacker();
        void fit(Block** __blocks, size_t __numBlocks);
        template<size_t __numBlocks>
        void fit(Block* (&__blocks)[__numBlocks]) {
            fit(__blocks, __numBlocks);
        }
        Node* findNode(Node* __root, int __w, int __h);
        Node* splitNode(Node* __node, int __w, int __h);
        Node* growNode(int __w, int __h);
        Node* growRight(int __w, int __h);
        Node* growDown(int __w, int __h);
        ~GrowingPacker();
    };
}

#ifdef BINPACKING_IMPLEMENTATION

BinPacking::Node::Node(int __x, int __y, int __w, int __h) :
    Node(false, __x, __y, __w, __h, nullptr, nullptr) {}
BinPacking::Node::Node(bool __used, int __x, int __y, int __w, int __h, Node* __down, Node* __right) :
    used(__used),
    x(__x),
    y(__y),
    w(__w),
    h(__h),
    down(__down),
    right(__right) {}
BinPacking::Node::~Node() {
    if (right) {
        delete right;
        right = nullptr;
    }
    if (down) {
        delete down;
        down = nullptr;
    }
}

BinPacking::Block::Block(int __w, int __h) :
    w(__w),
    h(__h),
    fit(nullptr) {}

BinPacking::Packer::Packer(int __w, int __h) :
    w(__w),
    h(__h),
    root(new Node(0, 0, __w, __h)) {}
void BinPacking::Packer::fit(Block** __blocks, size_t __numBlocks) {
    Node* __node;
    Block* __block;
    for (size_t i = 0; i < __numBlocks; ++i) {
        __block = __blocks[i];
        if (__node = findNode(root, __block->w, __block->h))
            __block->fit = splitNode(__node, __block->w, __block->h);
    }
}
BinPacking::Node* BinPacking::Packer::findNode(Node* __root, int __w, int __h) {
    if (__root->used) {
        Node* __node = findNode(__root->right, __w, __h);
        return __node ? __node : findNode(__root->down, __w, __h);
    }
    if ((__w <= __root->w) && (__h <= __root->h))
        return __root;
    return nullptr;
}
BinPacking::Node* BinPacking::Packer::splitNode(Node* __node, int __w, int __h) {
    __node->used = true;
    if (__node->down)
        delete __node->down;
    __node->down = new Node(__node->x, __node->y + __h, __node->w, __node->h - __h);
    if (__node->right)
        delete __node->right;
    __node->right = new Node(__node->x + __w, __node->y, __node->w - __w, __h);
    return __node;
}
BinPacking::Packer::~Packer() {
    if (root) {
        delete root;
        root = nullptr;
    }
}

BinPacking::GrowingPacker::GrowingPacker() : root(nullptr) {}
void BinPacking::GrowingPacker::fit(Block** __blocks, size_t __numBlocks) {
    Node* __node;
    Block* __block;
    int __w = __numBlocks > 0 ? __blocks[0]->w : 0;
    int __h = __numBlocks > 0 ? __blocks[0]->h : 0;
    root = new Node(0, 0, __w, __h);
    for (size_t i = 0; i < __numBlocks; ++i) {
        __block = __blocks[i];
        if (__node = findNode(root, __block->w, __block->h))
            __block->fit = splitNode(__node, __block->w, __block->h);
        else
            __block->fit = growNode(__block->w, __block->h);
    }
}
BinPacking::Node* BinPacking::GrowingPacker::findNode(Node* __root, int __w, int __h) {
    if (__root->used) {
        Node* __node = findNode(__root->right, __w, __h);
        return __node ? __node : findNode(__root->down, __w, __h);
    }
    if ((__w <= __root->w) && (__h <= __root->h))
        return __root;
    return nullptr;
}
BinPacking::Node* BinPacking::GrowingPacker::splitNode(Node* __node, int __w, int __h) {
    __node->used = true;
    if (__node->down)
        delete __node->down;
    __node->down = new Node(__node->x, __node->y + __h, __node->w, __node->h - __h);
    if (__node->right)
        delete __node->right;
    __node->right = new Node(__node->x + __w, __node->y, __node->w - __w, __h);
    return __node;
}
BinPacking::Node* BinPacking::GrowingPacker::growNode(int __w, int __h) {
    bool canGrowDown = (__w <= root->w);
    bool canGrowRight = (__h <= root->h);
    // attempt to keep square-ish by growing right when height is much greater than width
    bool shouldGrowRight = canGrowRight && (root->h >= (root->w + __w));
    // attempt to keep square-ish by growing down  when width  is much greater than height
    bool shouldGrowDown = canGrowDown && (root->w >= (root->h + __h));
    if (shouldGrowRight)
        return growRight(__w, __h);
    if (shouldGrowDown)
        return growDown(__w, __h);
    if (canGrowRight)
        return growRight(__w, __h);
    if (canGrowDown)
        return growDown(__w, __h);
    return nullptr;
}
BinPacking::Node* BinPacking::GrowingPacker::growRight(int __w, int __h) {
    root = new Node(
        true,
        0,
        0,
        root->w + __w,
        root->h,
        root,
        new Node(
            root->w,
            0,
            __w,
            root->h)
    );
    Node* __node;
    if (__node = findNode(root, __w, __h))
        return splitNode(__node, __w, __h);
    return nullptr;
}
BinPacking::Node* BinPacking::GrowingPacker::growDown(int __w, int __h) {
    root = new Node(
        true,
        0,
        0,
        root->w,
        root->h + __h,
        new Node(
            0,
            root->h,
            root->w,
            __h),
        root
    );
    Node* __node;
    if (__node = findNode(root, __w, __h))
        return splitNode(__node, __w, __h);
    return nullptr;
}
BinPacking::GrowingPacker::~GrowingPacker() {
    if (root) {
        delete root;
        root = nullptr;
    }
}

#endif // BINPACKING_IMPLEMENTATION

