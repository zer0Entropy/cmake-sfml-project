#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class RandomNumberGenerator;
class LogMgr;

class BSPTree {
public:

    struct Node {
        sf::IntRect     rect;
        Node*           leftChild;
        Node*           rightChild;
        static constexpr unsigned int minWidth{6};
        static constexpr unsigned int minHeight{6};

        Node() = delete;
        Node(sf::IntRect rect);
        ~Node();
        bool            IsLeaf() const;
    };

    BSPTree() = delete;
    BSPTree(const sf::Vector2u mapSize, LogMgr& logMgr);
    BSPTree(const BSPTree& copy) = delete;
    BSPTree(BSPTree& copy) = delete;
    BSPTree(BSPTree&& move) = delete;
    ~BSPTree();

    std::vector<Node*>  GetLeafList() const;

    Node*               CreateRoot();
    bool                Split(RandomNumberGenerator& rng);

private:
    bool                SplitNode(Node& node, RandomNumberGenerator& rng);
    Node*               CreateNode(sf::IntRect rect);

    sf::Vector2u                            mapSize;
    LogMgr&                                 logMgr;
    Node*                                   root;
    std::vector<std::unique_ptr<Node>>      nodeList;
};