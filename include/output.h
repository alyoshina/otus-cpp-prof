#pragma once

#include <ostream>
#include <fstream>

class IOutput {
public:
    IOutput() {}
    virtual ~IOutput() = default;
    virtual void setName(const std::string& str) = 0;
    virtual IOutput& operator<<(const std::string& str) = 0;
    virtual IOutput& endl() = 0; 
    virtual bool presetting() = 0;
    virtual void postsetting() = 0;
    bool getStatus() const { return status; }
protected:
    bool status {true};
};

/** @brief Output command information to the console.
*
*/
class ConsoleOutput : public IOutput {
public:
    ConsoleOutput(std::ostream &o) : out(o) {}
    ~ConsoleOutput() = default;
    void setName(const std::string& ) {}
    IOutput& operator<<(const std::string& str) {
        out << str;
        return *this;
    }
    IOutput& endl() override {
        out << std::endl;
        return *this;
    }
    bool presetting() override { return true; }
    void postsetting() override {}
private:
    std::ostream& out;
};

/** @brief Output command information to the file.
*
*/
class FileOutput : public IOutput {
public:
    FileOutput() {}
    ~FileOutput() = default;
    void setName(const std::string& str) { fileName = str; }
    IOutput& operator<<(const std::string& str) {
        if (getStatus()) {
            out << str;
        }
        return *this;
    }
    IOutput& endl() override {
        if (getStatus()) {
            out << std::endl;
        }
        return *this;
    }
    bool presetting() override {
        out.open(fileName, std::ios::binary);
        if (!out.is_open()) {
            //std::cout << "error, file " << fileName << " dosen't open" << std::endl;
            status = false;
        } else {
            status = true;
        }
        return status;
    }
    void postsetting() override {
        if (getStatus()) {
            out.close();
        }
    }
private:
    std::string fileName;
    std::ofstream out;
};