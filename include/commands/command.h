#pragma once

#include "feature.h"
#include "document.h"
#include "layer.h"

class Command {
public:
    Command(std::shared_ptr<Feature> f) : feature(f) { }
    virtual ~Command() = default;

    virtual void undo() { Logger::getInstance().log(std::string("undo: ") + name); }
    virtual void redo() { Logger::getInstance().log(std::string("redo: ") + name); }
    std::shared_ptr<Feature> getFeature() { return feature; }
protected:
    std::shared_ptr<Feature> feature {nullptr};
    std::string name;
};

class NodeCommand : public Command {
public:
    NodeCommand(std::shared_ptr<Layer> l, std::shared_ptr<Feature> f) : Command(f), layer(l) { }
    virtual ~NodeCommand() = default;
protected:
    std::weak_ptr<Layer> layer;
};

/**
*  @brief  Command to add node on layer.
*
*/
class AddNodeCommand : public NodeCommand {
public:
    AddNodeCommand(std::shared_ptr<Layer> l, std::shared_ptr<Feature> f) : NodeCommand(l, f) {
        name = std::string("Add ") + f->getName();
    }
    ~AddNodeCommand() = default;
    /**
    *  @brief  Cancel add node on layer.
    *
    *  Cancel command add node on layer
    */
    void undo() override {
        Command::undo();
        if (std::shared_ptr<Layer> l = layer.lock()) {
            l->remove(feature);
        }
        //...
    }
    /**
    *  @brief  Add node on layer.
    *
    *  Execute command add node on layer
    */
    void redo() override {
        //...
        if (std::shared_ptr<Layer> l = layer.lock()) {
            l->add(feature);
        }
        Command::redo();
    }
};

/**
*  @brief  Command to remove node from layer.
*
*/
class RemoveNodeCommand : public NodeCommand {
public:
    RemoveNodeCommand(std::shared_ptr<Layer> l, std::shared_ptr<Feature> f) : NodeCommand(l, f) {
        name = std::string("Remove ") + f->getName();
    }
    ~RemoveNodeCommand() = default;
    /**
    *  @brief  Cancel remove node from layer.
    *
    *  Cancel command remove node from layer
    */
    void undo() {
        Command::undo();
        if (std::shared_ptr<Layer> l = layer.lock()) {
            l->add(feature);
        }
        //...
    }
    /**
    *  @brief  Remove node from layer.
    *
    *  Execute command remove node from layer
    */
    void redo() {
        if (std::shared_ptr<Layer> l = layer.lock()) {
            l->remove(feature);
        }
        //...
        Command::redo();
    }
};