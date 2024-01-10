#pragma once

#include "feature.h"

#include <algorithm>
#include <memory>
#include <list>

class Layer {
public:
    Layer() = default;
    virtual ~Layer() = default;
    void add(std::shared_ptr<Feature> f) {
        features.emplace_back(f);
    }
    void remove(std::shared_ptr<Feature> f) {
        std::erase_if(features, [f](auto v) { return v.get() == f.get(); });
    }
    void clear() {
        features.clear();
    }

    std::list<FeatureIdType> getFeaturesIds() {
        std::list<FeatureIdType> list;
        std::transform(features.cbegin(), features.cend(), std::back_inserter(list),
                                            [](auto f) { return f->getId(); });
        return list;
    }
    std::shared_ptr<Feature> getFeature(FeatureIdType id) {
        auto it = std::find_if(features.begin(), features.end(),
                                            [id](auto f) {
                                                return  f->getId() == id;
                                            });
        return it != features.end() ? *it : nullptr;
    }

private:
    std::list<std::shared_ptr<Feature>> features;
};