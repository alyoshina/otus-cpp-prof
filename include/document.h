#pragma once

#include "logger.h"
#include "command_history.h"
#include "layer.h"

class Document {
public:
    Document() : layer(std::make_shared<Layer>()) {
        Logger::getInstance().log("Create document");
    }
    ~Document() { 
        history.cleanup();
        layer->clear();
        Logger::getInstance().log("Delete document");
    }
    //bool isSave() { return isSave; }
    /**
    *  @brief  Load document from file.
    *  @param[in]  fileName name of file for load
    * 
    */
    void loadFromFile(const std::string &fileName) {
        history.cleanup();
        Logger::getInstance().log(std::string("Load document from file ") + fileName);
    }
    /**
    *  @brief  Load document to file.
    *  @param[in]  fileName name of file for load
    * 
    */
    void loadToFile(const std::string &fileName) {
        Logger::getInstance().log(std::string("Load document to file ") + fileName);
    }
    /**
    *  @brief  Add command to CommandHistory.
    *  @param[in]  cmd command for add
    */
    void addCommand(std::shared_ptr<Command> cmd) {
        history.add(cmd);
    }
    /**
    *  @brief  Get layer pointer in document.
    *  @param[out]  layer pointer
    */
    std::shared_ptr<Layer> getLayer() { return layer; };

    /**
    *  @brief  Get features ids list on document layer.
    *  @param[out]  features ids list
    */
    std::list<FeatureIdType> getFeaturesIds() { return getLayer()->getFeaturesIds(); }
    /**
    *  @brief  Get feature pointer by feature id.
    *  @param[in]  id feature id
    *  @param[out]  feature pointer
    */
    std::shared_ptr<Feature> getFeature(FeatureIdType id) { return getLayer()->getFeature(id); }

private:
    CommandHistory history;
    //bool isSave {false};
    std::shared_ptr<Layer> layer;
};