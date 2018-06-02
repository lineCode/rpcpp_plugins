/**
 * MIT License
 *
 * Copyright (c) 2018 Younguk Kim (bluekyu)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <string>

#include <imgui.h>

namespace rpplugins {

class WindowInterface
{
public:
    WindowInterface(const std::string& title);

    virtual ~WindowInterface() = default;

    void draw();

    virtual void draw_contents() = 0;

    void show();
    void hide();

protected:
    std::string title_;
    bool is_open_ = false;
    ImGuiWindowFlags window_flags_ = 0;
};

// ************************************************************************************************

inline WindowInterface::WindowInterface(const std::string& title = "no-name") : title_(title)
{
}

inline void WindowInterface::draw()
{
    if (!is_open_)
        return;

    if (!ImGui::Begin(title_.c_str(), &is_open_, window_flags_))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    draw_contents();

    ImGui::End();
}

inline void WindowInterface::show()
{
    is_open_ = true;
}

inline void WindowInterface::hide()
{
    is_open_ = false;
}

}