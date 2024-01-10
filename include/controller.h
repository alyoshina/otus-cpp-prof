#pragma once

#include "observer.h"

#include <memory>

class IController: public Subscriber, public Publisher {
public:    
    IController() {}
    virtual ~IController() = default;
    virtual void setModel(std::shared_ptr<IModel> m) = 0;
    virtual void createDocument() = 0;
    virtual void importFromFile(const std::string &fileName) = 0;
    virtual void exportToFile(const std::string &fileName) = 0;
    virtual void createNode(int x, int y) = 0;
    virtual void deleteNode(FeatureIdType id) = 0;
};

class Controller: public IController, public std::enable_shared_from_this<Controller> {
public:    
    Controller() = default;
    virtual ~Controller() = default;
    /**
    *  @brief  Set %model to @a m.
    *  @param  m  A shared_ptr .
    * 
    *  unsubscribe Controller from the previous model and
    *  subscribe Controller to notification from new model
    */
    virtual void setModel(std::shared_ptr<IModel> m) final {
        if (model) {
            model->deleteSubscriber(shared_from_this());
        }
        model = m;
        if (model) {
            model->addSubscriber(shared_from_this());
        }
    }
    /**
    *  @brief  Create new document.
    * 
    */
    virtual void createDocument() final {
        if (isModelSet()) {
            model->createNewDocument();
        }
    };
    /**
    *  @brief  Import document from file.
    *  @param[in]  fileName name of file for load
    * 
    */
    virtual void importFromFile(const std::string &fileName) final {
        if (isModelSet()) {
            model->loadFromFile(fileName);
        }
    };
    /**
    *  @brief  Export document to file.
    *  @param[in]  fileName name of file for load
    * 
    */
    virtual void exportToFile(const std::string &fileName) final {
        if (isModelSet()) {
            model->loadToFile(fileName);
        }
    };
    /**
    *  @brief  Create and add node to document layer.
    *  @param[in]  x coordinate
    *  @param[in]  y coordinate
    * 
    */
    virtual void createNode(int x, int y) final {
        if (isModelSet()) {
            model->createNode(x, y);
        }
    }
    /**
    *  @brief  Delete node from document layer.
    *  @param[in]  id feature id that needs to be deleted
    * 
    */
    virtual void deleteNode(FeatureIdType id) final {
        if (isModelSet()) {
            model->deleteNode(id);
        }
    };
    /**
    *  @brief  notify about model change.
    * 
    *  Notify subscribers (view) about model change and need to update
    */
    virtual void notify() final {
        //Update view
        notifyUpdate();
    }
private:
    std::shared_ptr<IModel> model;
    
    bool isModelSet() {
        if (!model) {
            Logger::getInstance().log("Model dosen't set");
            return false;
        }
        return true;
    }
};