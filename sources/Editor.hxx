#pragma once

#include <string>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/panel.hpp>
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

protected:
    //--- protected methods ---
    void setupMenu();
    void setupEditor();
    void setupUi();
    bool saveIfEdited(const bool force_requester = false);
    std::string fileDialog(const bool is_open_dialog = true) const;

private:
    //--- private properties ---
    nana::place _layout;
    nana::menubar _menu;
    nana::panel<true> _linenum;
    nana::textbox _text;
};
