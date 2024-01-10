#include "model.h"
#include "controller.h"

#include <exception>
#include <memory>
#include <iostream>

class View : public Subscriber, public std::enable_shared_from_this<View> {
public:
    View() = default;
    void createDocument() { controller->createDocument(); }
    void importFromFile(const std::string &fileName) { controller->importFromFile(fileName); }
    void exportToFile(const std::string &fileName) { controller->exportToFile(fileName); }
    void createNode(int x, int y) { controller->createNode(x, y); }
    void deleteNode(FeatureIdType id) { controller->deleteNode(id); }

    /**
    *  @brief  Set %controller to @a contr.
    *  @param[in]  contr  a shared_ptr to Controller.
    * 
    *  Unsubscribe View from the previous controller and
    *  subscribe View to notification from new controller
    */
    void setController(std::shared_ptr<IController> contr) {
        if (controller) {
            controller->deleteSubscriber(shared_from_this());
        }
        controller = contr;
        if (controller) {
            controller->addSubscriber(shared_from_this());
        }
    }

    void notify() { Logger::getInstance().log("Update view"); }
private:
    std::shared_ptr<IController> controller;
};

/**
 * @brief Graphic vector editor
 */
int main(int argc, char const *argv[])
{
    auto model = std::make_shared<Model>();
    auto controller = std::make_shared<Controller>();
    controller->setModel(model);
    auto view = std::make_shared<View>();
    view->setController(controller);

    view->createDocument();
    view->importFromFile("importFileName");
    view->exportToFile("exportFileName");
    view->createNode(5, 7);
    auto list = model->getFeaturesIds();
    if (!list.empty()) {
        view->deleteNode(list.back());
    }

    return 0;
}
