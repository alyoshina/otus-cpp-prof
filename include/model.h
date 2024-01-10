#pragma once

#include "observer.h"
#include "document.h"

#include <memory>

class IModel : public Publisher {
public:
    IModel() = default;
    virtual ~IModel() = default;
    virtual void createNewDocument() = 0;
    virtual void loadFromFile(const std::string &fileName) = 0;
    virtual void loadToFile(const std::string &fileName) = 0;
    virtual void createNode(int x, int y) = 0;
    virtual void deleteNode(FeatureIdType id) = 0;
};

class Model : public IModel {
public:
    Model() = default;
    ~Model() = default;
    /**
    *  @brief  Create new document.
    * 
    *  Create new document and notify subscribers on update
    */
    void createNewDocument() override {
        if (document) {
            //TODO document.isSave()
        }
        document = std::make_shared<Document>();
        notifyUpdate();
    }
    /**
    *  @brief  Load document from file.
    *  @param[in]  fileName name of file for load
    * 
    *  Create new document, load data from file to created document 
    *  and notify subscribers on update
    */
    void loadFromFile(const std::string &fileName) {
        createNewDocument();
        document->loadFromFile(fileName);
        notifyUpdate();
    }
    /**
    *  @brief  Load document to file.
    *  @param[in]  fileName name of file for load
    * 
    *  Load data to file and notify subscribers on update
    */
    void loadToFile(const std::string &fileName) {
        if (document) {
            document->loadToFile(fileName);
            notifyUpdate();
        }
    }
    /**
    *  @brief  Create and add node to document layer.
    *  @param[in]  x coordinate
    *  @param[in]  y coordinate
    * 
    *  Add node to document layer using AddNodeCommand and notify subscribers on update
    */
    void createNode(int x, int y) {
        document->addCommand(std::make_shared<AddNodeCommand>(document->getLayer(),
                                                                std::make_shared<Node>(x, y)));
        notifyUpdate();
    }
    /**
    *  @brief  Delete node from document layer.
    *  @param[in]  id feature id that needs to be deleted
    * 
    *  Delete node from document layer using RemoveNodeCommand and notify subscribers on update
    */
    void deleteNode(FeatureIdType id) {
        std::shared_ptr<Feature> node = document->getFeature(id);
        document->addCommand(std::make_shared<RemoveNodeCommand>(document->getLayer(), node));
        notifyUpdate();
    }
    /**
    *  @brief  Get features ids list on document layer.
    *  @param[out]  features ids list
    */
    std::list<FeatureIdType> getFeaturesIds() { return document->getFeaturesIds(); }
private:
    std::shared_ptr<Document> document;

};