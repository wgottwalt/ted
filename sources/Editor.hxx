#pragma once

#include <string>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/textbox.hpp>

class Editor : public nana::form {
public:
    //--- public constants and types ---
    static constexpr char DefFilename[] = "noname.txt";

    //--- public constructors ---
    Editor(const std::string &name, const std::string &filename = DefFilename);
    Editor(const Editor &rhs) = delete;
    Editor(Editor &&rhs) = delete;
    ~Editor() noexcept;

    //--- public operators ---
    Editor &operator=(const Editor &rhs) = delete;
    Editor &operator=(Editor &&rhs) = delete;

    //--- public methods ---
    bool save(const std::string &filename = "");
    bool load(const std::string &filename = "");

protected:
    //--- protected methods ---
    void setupMenu();
    void setupEditor();
    void setupUi();
    bool isSaveNeeded(const std::string &filename);
    std::string fileDialog(const bool is_open) const;

private:
    //--- private properties ---
    nana::place _widget;
    nana::menubar _menu;
    nana::textbox _text;
    std::string _filename;
};
