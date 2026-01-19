#include "RedBlackTree.h"

RedBlackTree::RedBlackTree()
    : root(nullptr)
{
}

RedBlackTree::~RedBlackTree()
{
    clear();
}

void RedBlackTree::clear()
{
    clear(root);
    root = nullptr;
}

void RedBlackTree::clear(ResearcherNode *node)
{
    //TODO:
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

ResearcherNode *RedBlackTree::findResearcher(const std::string &fullName) const
{
    return find(root, fullName);
}

ResearcherNode *RedBlackTree::find(ResearcherNode *node, const std::string &fullName) const
{

//TODO: search and find researcher by name
if (node == nullptr) {return nullptr;}
    if (fullName == node->data.fullName) {return node;}
    if (fullName < node->data.fullName) {return find(node->left, fullName);}
    return find(node->right, fullName);
}

bool RedBlackTree::insertResearcher(const Researcher &researcher)
{
    //attempt to insert, erturn true if success, false otherwise
    ResearcherNode *z = new ResearcherNode(researcher);
    z->color = RED;
    if (researcher.capacity > 0) {
        z->data.assignedArtifacts = new int[researcher.capacity];
        
        for (int i = 0; i < researcher.numAssigned; i++) {
            z->data.assignedArtifacts[i] = researcher.assignedArtifacts[i];
        }
    } 
    else {
        z->data.assignedArtifacts = nullptr;
    }
    
    bool inserted = false;
    root = bstInsert(root, z, inserted);

    if (!inserted) {
        delete z; 
        return false;
    }

    if (z->parent == nullptr) {
        z->color = BLACK;
    } 
    else {
        insertFixup(z);
    }
    
    if (root) {
        root->color = BLACK;
    }
    return true;
}

ResearcherNode *RedBlackTree::bstInsert(ResearcherNode *current, ResearcherNode *node, bool &inserted)
{
    // TODO:
    // 1) Standard BST insert by node->data.fullName.
    // 2) If tree is empty, node becomes root (set inserted = true).
    // 3) If name already exists, inserted = false and return current without inserting.
    // 4) Otherwise insert and set inserted = true.
    // 5) Do not modify colors here (node->color already RED).
    if (current == nullptr) {
        inserted = true;
        return node; 
    }

    if (node->data.fullName < current->data.fullName) {
        current->left = bstInsert(current->left, node, inserted);
 
        if (current->left == node) {
            node->parent = current;
        }
    }
    else if (node->data.fullName > current->data.fullName) {
        current->right = bstInsert(current->right, node, inserted);

        if (current->right == node) {
            node->parent = current;
        }
    }
    else {
        inserted = false;
        
    }

    return current;
}

void RedBlackTree::insertFixup(ResearcherNode *node)
{
    // TODO: Standard Red-Black insertion fixup.
    // While parent is RED, handle cases based on uncle's color and position.
    // Ensure root becomes BLACK at the end.
    while (node != root && node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            ResearcherNode *y = node->parent->parent->right; 
            if (y != nullptr && y->color == RED) { 
                node->parent->color = BLACK;
                y->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } 
            else {
                if (node == node->parent->right) { 
                    node = node->parent;
                    rotateLeft(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateRight(node->parent->parent);
            }
        } 
        else {
            ResearcherNode *y = node->parent->parent->left;
            if (y != nullptr && y->color == RED) {
                node->parent->color = BLACK;
                y->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateLeft(node->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

bool RedBlackTree::removeResearcher(const std::string &fullName)
{
    // TODO:
    // 1) Find node z with data.fullName == fullName.
    // 2) If not found, return false.
    // 3) Perform standard RBT delete:
    //    - Track original color of removed node.
    //    - If a black node is removed, call deleteFixup on the appropriate child.
    // 4) Free node memory.
    // 5) Return true.
    ResearcherNode *z = findResearcher(fullName);
    if (z == nullptr) {
        return false;
    }

    ResearcherNode *y = z; 
    ResearcherNode *x;     
    ResearcherNode *x_parent; 
    Color yOrgColor = y->color;

    if (z->left == nullptr) {
        x = z->right;
        x_parent = z->parent; 
        
        if (z->parent == nullptr) {
            root = z->right;
            }
        else if (z == z->parent->left) {
            z->parent->left = z->right;}
        else {
            z->parent->right = z->right;
            }
        
        if (z->right != nullptr) {
            z->right->parent = z->parent;
            }
    } 
    else if (z->right == nullptr) {
        x = z->left;
        x_parent = z->parent;
        
        if (z->parent == nullptr) {
            root = z->left;}
        else if (z == z->parent->left) {
            z->parent->left = z->left;
            }
        else {
            z->parent->right = z->left;
            }
        
        if (z->left != nullptr) {
            z->left->parent = z->parent;
        }
    } 
    else {
        y = minimum(z->right);
        yOrgColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x_parent = y; 
        } 
        else {
            x_parent = y->parent; 
            
            if (y->parent == nullptr) {
                root = y->right;
                }
            else if (y == y->parent->left) {
                y->parent->left = y->right;
                }
            else {
                y->parent->right = y->right;}
            
            if (y->right != nullptr) {
                y->right->parent = y->parent;}

            y->right = z->right;
            y->right->parent = y;
        }
        if (z->parent == nullptr) {
            root = y;
            }
        else if (z == z->parent->left) {
            z->parent->left = y;
            }
        else {
            z->parent->right = y;
            }
        
        y->parent = z->parent;
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color; 
    }

    delete z;

    if (yOrgColor == BLACK) {
        deleteFixup(x, x_parent);
    }

    if (root != nullptr) {root->color = BLACK;
    }

    return true;
}

void RedBlackTree::deleteFixup(ResearcherNode *node, ResearcherNode *parent)
{
    // TODO: Standard Red-Black deletion fixup.
    ResearcherNode *w;
    while ((node == nullptr || node->color == BLACK) && node != root)
    {
        if (node == parent->left)
        {
            w = parent->right; 
            if (w->color == RED)
            {
                w->color = BLACK;
                parent->color = RED;
                rotateLeft(parent);
                w = parent->right; 
            }
            if ((w->left == nullptr || w->left->color == BLACK) && 
                (w->right == nullptr || w->right->color == BLACK))
            {
                w->color = RED;
                node = parent; 
                parent = node->parent;
            }
            else
            {
                if (w->right == nullptr || w->right->color == BLACK)
                {
                    if (w->left != nullptr) {
                        w->left->color = BLACK;
                        }
                    w->color = RED;
                    rotateRight(w);
                    w = parent->right;
                }
                w->color = parent->color;
                parent->color = BLACK;
                if (w->right != nullptr) {
                    w->right->color = BLACK;}
                rotateLeft(parent);
                node = root; 
            }
        }
        else
        {
            w = parent->left; 
            if (w->color == RED)
            {
                w->color = BLACK;
                parent->color = RED;
                rotateRight(parent);
                w = parent->left;
            }
            if ((w->right == nullptr || w->right->color == BLACK) && 
                (w->left == nullptr || w->left->color == BLACK))
            {
                w->color = RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (w->left == nullptr || w->left->color == BLACK)
                {
                    if (w->right != nullptr) {
                        w->right->color = BLACK;
                        }
                    w->color = RED;
                    rotateLeft(w);
                    w = parent->left;
                }
                w->color = parent->color;
                parent->color = BLACK;
                if (w->left != nullptr) {
                    w->left->color = BLACK;
                    }
                rotateRight(parent);
                node = root;
            }
        }
    }
    if (node != nullptr)
        {node->color = BLACK;
        }
}



ResearcherNode *RedBlackTree::minimum(ResearcherNode *node) const
{
    // TODO: Return leftmost node in subtree.
    if (node == nullptr) {
        return nullptr;
    }
    
    while (node->left != nullptr) {
        node = node->left;
    }
    
    return node;
}

void RedBlackTree::rotateLeft(ResearcherNode *x)
{
    // TODO: Standard left rotation.
    if (x == nullptr || x->right == nullptr) {return;} 

    ResearcherNode *y = x->right;
    x->right = y->left;
    
    if (y->left != nullptr) {
        y->left->parent = x;}
    
    y->parent = x->parent;
    
    if (x->parent == nullptr) {
        root = y;
        }
    else if (x == x->parent->left) {
        x->parent->left = y;
        }
    else {x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rotateRight(ResearcherNode *y)
{
    // TODO: Standard right rotation.
    if (y == nullptr || y->left == nullptr) {return;} 

    ResearcherNode *x = y->left;
    y->left = x->right;
    
    if (x->right != nullptr) {
        x->right->parent = y;
        }
    
    x->parent = y->parent;
    
    if (y->parent == nullptr) {
        root = x;}
    else if (y == y->parent->right) {
        y->parent->right = x;
        }
    else {
        y->parent->left = x;
    }
    
    x->right = y;
    y->parent = x;
}

int RedBlackTree::getResearcherCount() const
{
    return getResearcherCount(root);
}

int RedBlackTree::getResearcherCount(ResearcherNode *node) const
{
    // TODO: return size of subtree.
    if (!node) {return 0;}
    return 1 + getResearcherCount(node->left) + getResearcherCount(node->right);
}

int RedBlackTree::getTotalLoad() const
{
    return getTotalLoad(root);
}

int RedBlackTree::getTotalLoad(ResearcherNode *node) const
{
    // TODO: sum of data.numAssigned in subtree.
    if (!node) {return 0;}
    return node->data.numAssigned + getTotalLoad(node->left) + getTotalLoad(node->right);
}

void RedBlackTree::traversePreOrderForStats() const
{
    traversePreOrderForStats(root);
}

void RedBlackTree::traversePreOrderForStats(ResearcherNode *node) const
{
    // TODO:
    // Pre-order traversal (node, left, right).
    // Students will decide what exactly to print in PRINT_STATS.
    if (!node) {return;}
    std::cout << node->data.fullName << " " 
              << node->data.capacity << " " 
              << node->data.numAssigned << std::endl;
    traversePreOrderForStats(node->left);
    traversePreOrderForStats(node->right);
}
