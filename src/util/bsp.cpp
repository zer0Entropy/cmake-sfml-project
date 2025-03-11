#include "../../include/util/bsp.hpp"
#include "../../include/rng.hpp"

BSPTree::Node::Node(sf::IntRect rect):
    rect{rect},
    leftChild{nullptr},
    rightChild{nullptr} {

}

BSPTree::Node::~Node() {

}

bool BSPTree::Node::IsLeaf() const {
    bool isLeaf{false};
    if(!leftChild && !rightChild) {
        isLeaf = true;
    }
    return isLeaf;
}

BSPTree::BSPTree(const sf::Vector2u mapSize):
    mapSize{mapSize} {

}
BSPTree::~BSPTree() {

}

bool BSPTree::SplitNode(Node& node, RandomNumberGenerator& rng) {
    bool successful{true};
    if(!node.IsLeaf()) {
        successful = SplitNode(*node.leftChild, rng);
        successful = successful && SplitNode(*node.rightChild, rng);
        return successful;
    }

    // If width > height, split vertically
    if(node.rect.size.x > node.rect.size.y) {
        int splitPointMin = node.rect.position.x + (4 * node.rect.size.x / 10);
        int splitPointMax = node.rect.position.x + (6 * node.rect.size.x / 10);
        if(splitPointMin - node.rect.position.x < Node::minWidth) {
            if(splitPointMax >= Node::minWidth) {
                splitPointMin = splitPointMax;
            }
            else {
                successful = false;
                return successful;
            }
        }
        int splitPoint = rng.GetRandom(splitPointMin, splitPointMax);
        node.leftChild = CreateNode(sf::IntRect{
            {
                node.rect.position.x,
                node.rect.position.y
            }, {
                splitPoint - node.rect.position.x,
                node.rect.size.y
            }
        });
        node.rightChild = CreateNode(sf::IntRect{
            {
                splitPoint + 1,
                node.rect.position.y
            }, {
                node.rect.position.x + node.rect.size.x - splitPoint - 1,
                node.rect.size.y
            }
        });
    }
    // Otherwise, split horizontally
    else {
        int splitPointMin = node.rect.position.y + (4 * node.rect.size.y / 10);
        int splitPointMax = node.rect.position.y + (6 * node.rect.size.y / 10);
        if(splitPointMin - node.rect.position.y < Node::minHeight) {
            if(splitPointMax >= Node::minHeight) {
                splitPointMin = splitPointMax;
            }
            else {
                successful = false;
                return successful;
            }
        }
        int splitPoint = rng.GetRandom(splitPointMin, splitPointMax);
        node.leftChild = CreateNode(sf::IntRect{
            {
                node.rect.position.x,
                node.rect.position.y
            }, {
                node.rect.size.x,
                splitPoint - node.rect.position.y
            }
        });
        node.rightChild = CreateNode(sf::IntRect{
            {
                node.rect.position.x,
                splitPoint + 1
            }, {
                node.rect.size.x,
                node.rect.position.y + node.rect.size.y - splitPoint - 1
            }
        });
    }
    return successful;
}

BSPTree::Node* BSPTree::CreateNode(sf::IntRect rect) {
    nodeList.emplace_back(std::make_unique<Node>(rect));
    if(!root) {
        root = (*nodeList.rbegin()).get();
    }
    return (*nodeList.rbegin()).get();
}

std::vector<BSPTree::Node*> BSPTree::GetLeafList() const {
    std::vector<Node*>  leafList;
    for(auto& node : nodeList) {
        if(node->IsLeaf()) {
            leafList.push_back(node.get());
        }
    }
    return leafList;
}