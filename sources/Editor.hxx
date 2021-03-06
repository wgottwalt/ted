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
    static const int32_t DefSpace = 5;

    //--- public constructors ---
    Editor(const std::string &name, const std::string &filename = DefFilename) noexcept(false);
    Editor(const Editor &rhs) noexcept(false) = delete;
    Editor(Editor &&rhs) noexcept = delete;
    ~Editor() noexcept = default;

    //--- public operators ---
    Editor &operator=(const Editor &rhs) = delete;
    Editor &operator=(Editor &&rhs) = delete;

protected:
    //--- protected methods ---
    void setupMenu() noexcept(false);
    void setupEditor() noexcept(false);
    void setupUi() noexcept(false);
    bool saveIfEdited(const bool force_requester = false) noexcept(false);
    std::string fileDialog(const bool is_open_dialog = true) const noexcept(false);
    void redrawLineNumPanel(nana::paint::graphics &gfx) noexcept(false);

private:
    //--- private properties ---
    nana::place _layout;
    nana::menubar _menu;
    nana::panel<true> _linenum;
    nana::textbox _text;
};
