#pragma once
#include "logger.h"

using FeatureIdType = unsigned long;

class Feature {
public:
    Feature(std::shared_ptr<Feature> p = nullptr) : parent(p) {
        id = (FeatureIdType)this;
    };
    virtual ~Feature() {};
    FeatureIdType getId() const { return id; }
    std::string getName() const { return name; }
private:
    std::weak_ptr<Feature> parent;
    std::list<std::weak_ptr<Feature>> childs;
    FeatureIdType id;
protected:
    std::string name;
};

class Node : public Feature {
public:
    Node(int x, int y, std::shared_ptr<Feature> parent = nullptr)
        : Feature(parent)
        , x(x)
        , y(y) {
        name = "node (x: " + std::to_string(x) + ", y: "+ std::to_string(y) + ")";
        Logger::getInstance().log("Create " + name);
    }
    virtual ~Node() {
        Logger::getInstance().log("Delete " + name);
    }
private:
    int x {0};
    int y {0};
};