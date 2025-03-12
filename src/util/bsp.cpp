#include "../../include/util/bsp.hpp"
#include "../../include/rng.hpp"
#include "../../include/log.hpp"

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

BSPTree::BSPTree(const sf::Vector2u mapSize, LogMgr& logMgr):
    mapSize{mapSize},
    logMgr{logMgr} {

}
BSPTree::~BSPTree() {

}

BSPTree::Node* BSPTree::CreateRoot() {
    nodeList.clear();
    root = CreateNode(sf::IntRect{
        sf::Vector2i{0, 0},
        sf::Vector2i{(int)mapSize.x, (int)mapSize.y}
    });
    return root;
}

bool BSPTree::Split(int minWidth, int maxWidth, int minHeight, int maxHeight,RandomNumberGenerator& rng) {
    bool successful{true};

    if(!root) {
        root = CreateNode(
            sf::IntRect{
                sf::Vector2i{0, 0},
                sf::Vector2i{(int)mapSize.x, (int)mapSize.y}
            }
        );
    }

    auto leafList{GetLeafList()};
    for(auto& leaf : leafList) {
        successful = SplitNode(*leaf, minWidth, maxWidth, minHeight, maxHeight, rng);
    }

    return successful;
}

bool BSPTree::SplitNode(Node& node, int minWidth, int maxWidth, int minHeight, int maxHeight, RandomNumberGenerator& rng) {
    bool successful{true};
    // Ensure that we can fit two new nodes within this one
    if(node.rect.size.x > maxWidth || node.rect.size.y > maxHeight) {
        bool splitHorizontal{false};
        // If width and height are equal, choose horizontal/vertical with 50:50 odds
        if(node.rect.size.x == node.rect.size.y) {
            unsigned int coinFlip{rng.GetRandom(0, 1)};
            if(coinFlip) {
                splitHorizontal = true;
            }
            else {
                splitHorizontal = false;
            }
        }
        // Otherwise, split horizontally if and only if height > width
        else if(node.rect.size.y > node.rect.size.x) {
            splitHorizontal = true;
        }
        else {
            splitHorizontal = false;
        }
        // Make sure this node is splittable
        bool canSplit{false};
        if(node.rect.size.x > minWidth && node.rect.size.y > minHeight) {
            canSplit = true;
        }

        if(canSplit) {
            if(splitHorizontal) {
                std::string header{"Splitting horizontally"};
                std::string body{"BSPNode {position=("};
                body += std::to_string(node.rect.position.x) + ", ";
                body += std::to_string(node.rect.position.y) + ") size=(";
                body += std::to_string(node.rect.size.x) + ", ";
                body += std::to_string(node.rect.size.y)+ ")}";
                logMgr.CreateMessage(header, body);

                int newHeight{minHeight + (int)rng.GetRandom(0, node.rect.size.y - minHeight)};
                sf::IntRect left{
                    sf::Vector2i{node.rect.position.x, node.rect.position.y},
                    sf::Vector2i{node.rect.size.x, newHeight}
                };
                sf::IntRect right{
                    sf::Vector2i{node.rect.position.x, node.rect.position.y + newHeight},
                    sf::Vector2i{node.rect.size.x, node.rect.size.y - newHeight}
                };
                node.leftChild = CreateNode(left);
                node.rightChild = CreateNode(right);
            }
            else {
                std::string header{"Splitting vertically"};
                std::string body{"BSPNode {position=("};
                body += std::to_string(node.rect.position.x) + ", ";
                body += std::to_string(node.rect.position.y) + ") size=(";
                body += std::to_string(node.rect.size.x) + ", ";
                body += std::to_string(node.rect.size.y)+ ")}";
                logMgr.CreateMessage(header, body);

                int newWidth{minWidth + (int)rng.GetRandom(0, node.rect.size.x - minWidth)};
                sf::IntRect left{
                    sf::Vector2i{node.rect.position.x, node.rect.position.y},
                    sf::Vector2i{newWidth, node.rect.size.y}
                };
                sf::IntRect right{
                    sf::Vector2i{node.rect.position.x + newWidth, node.rect.position.y},
                    sf::Vector2i{node.rect.size.x - newWidth, node.rect.size.y}
                };
                node.leftChild = CreateNode(left);
                node.rightChild = CreateNode(right);
            }
        }
        else {
            successful = false;
        }
    }
    else {
        successful = false;
    }

    return successful;
}

BSPTree::Node* BSPTree::CreateNode(sf::IntRect rect) {
    std::string header{"Creating node #"};
    header += std::to_string(nodeList.size());
    std::string body{"position=("};
    body += std::to_string(rect.position.x) + ", ";
    body += std::to_string(rect.position.y) + ") size=(";
    body += std::to_string(rect.size.x) + ", ";
    body += std::to_string(rect.size.y) + ")";
    logMgr.CreateMessage(header, body);
    
    nodeList.emplace_back(std::make_unique<Node>(rect));
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