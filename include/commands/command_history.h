#pragma once

#include "command.h"
#include <list>

class CommandHistory {
public:
    CommandHistory() = default;
    virtual ~CommandHistory() = default;

    /**
    *  @brief  Clean cancel history and change history.
    */
    void cleanup() {
        ch_history.clear();
        cancel_history.clear();
    }
    /**
    *  @brief  Execute last command from cancel_history list.
    *  Execute command from cancel_history and add this command to ch_history
    */
    void redo() {
        if (cancel_history.empty()) {
            return;
        }
        auto cmd = cancel_history.back();
        cmd->redo();
        cancel_history.pop_back();
        ch_history.emplace_back(cmd);

        if (ch_history.size() > history_size) {
            ch_history.erase(ch_history.begin());
        }
    }
    /**
    *  @brief  Cancel command.
    * 
    *  Cancel command, remove command from ch_history and
    *  add command to cancel_history
    */
    void undo() {
        if (ch_history.empty()) {
            return;
        }
        auto cmd = ch_history.back();
        cmd->undo();
        ch_history.pop_back();
        cancel_history.emplace_back(cmd);
    }
    /**
    *  @brief  Execute command.
    *  @param[in]  cmd command for execute
    *  Execute command and add command to ch_history
    */
    void add(std::shared_ptr<Command> cmd) {
        cancel_history.emplace_back(cmd);
        redo();
    }
private:
    std::list<std::shared_ptr<Command>> ch_history; /**< change history list */
    std::list<std::shared_ptr<Command>> cancel_history; /**< cancel history list */
    std::size_t history_size {4};
};