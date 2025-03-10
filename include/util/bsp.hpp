#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class BSPTree {
public:

    struct Node {
        sf::IntRect     rect;
        Node*           leftChild;
        Node*           rightChild;
        static constexpr unsigned int minWidth{2};
        static constexpr unsigned int minHeight{2};

        bool            IsLeaf() const;
        bool            Split();
    };

    BSPTree() = delete;
    BSPTree(const sf::Vector2u mapSize);
    BSPTree(const BSPTree& copy) = delete;
    BSPTree(BSPTree& copy) = delete;
    BSPTree(BSPTree&& move) = delete;
    ~BSPTree();

private:
    sf::Vector2u        mapSize;
    Node                root;
};