/**
 * MIT License
 * 
 * Copyright (c) 2016-2017 Center of Human-centered Interaction for Coexistence
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

#include <showBoundsEffect.h>

#include <fmt/format.h>

#include <render_pipeline/rpcore/globals.hpp>
#include <render_pipeline/rpcore/rpobject.hpp>

#include "restapi/resources/common.hpp"
#include "restapi/resolve_message.hpp"

#include "api_server_interface.hpp"

namespace rpeditor {

extern APIServerInterface* global_server;

bool resolve_nodepath(const rapidjson::Document& doc)
{
    const std::string& method = doc["method"].GetString();
    if (method == RPEDITOR_API_READ_STRING)
    {
        const NodePath& np = get_nodepath(doc["message"]["path"]);
        if (!np)
            return false;

        rapidjson::Document new_doc;
        rapidjson::Value& message = init_document(new_doc, "NodePath", RPEDITOR_API_UPDATE_STRING);
        auto& allocator = new_doc.GetAllocator();

        // pose
        const auto& translation = np.get_pos();
        rapidjson::Value translation_array(rapidjson::kArrayType);
        translation_array.PushBack(translation[0], allocator);
        translation_array.PushBack(translation[1], allocator);
        translation_array.PushBack(translation[2], allocator);
        message.AddMember("translation", translation_array, allocator);

        const auto& hpr = np.get_hpr();
        rapidjson::Value hpr_array(rapidjson::kArrayType);
        hpr_array.PushBack(hpr[0], allocator);
        hpr_array.PushBack(hpr[1], allocator);
        hpr_array.PushBack(hpr[2], allocator);
        message.AddMember("hpr", hpr_array, allocator);

        const auto& scale = np.get_scale();
        rapidjson::Value scale_array(rapidjson::kArrayType);
        scale_array.PushBack(scale[0], allocator);
        scale_array.PushBack(scale[1], allocator);
        scale_array.PushBack(scale[2], allocator);
        message.AddMember("scale", scale_array, allocator);

        // visibility
        message.AddMember("visible", !np.is_hidden(), allocator);

        if (auto effect = np.get_effect(ShowBoundsEffect::get_class_type()))
            message.AddMember("tight_bounds", DCAST(ShowBoundsEffect, effect)->get_tight(), allocator);
        else
            message.AddMember("tight_bounds", false, allocator);

        message.AddMember("wireframe", np.get_render_mode() == RenderModeAttrib::Mode::M_wireframe, allocator);

        global_server->broadcast(new_doc);
    }
    else if (method == RPEDITOR_API_UPDATE_STRING)
    {
        const auto& message = doc["message"];

        NodePath np = get_nodepath(doc["message"]["path"]);
        if (!np)
            return false;

        np.set_pos_hpr_scale(
            LVecBase3(message["translation"][0].GetFloat(), message["translation"][1].GetFloat(), message["translation"][2].GetFloat()),
            LVecBase3(message["hpr"][0].GetFloat(), message["hpr"][1].GetFloat(), message["hpr"][2].GetFloat()),
            LVecBase3(message["scale"][0].GetFloat(), message["scale"][1].GetFloat(), message["scale"][2].GetFloat()));

        if (message["visible"].GetBool())
            np.show();
        else
            np.hide();

        if (message["tight_bounds"].GetBool())
            np.show_tight_bounds();
        else
            np.hide_bounds();

        if (message["wireframe"].GetBool())
            np.set_render_mode_wireframe();
        else
            np.clear_render_mode();
    }
    else
    {
        rpcore::RPObject::global_error("plugin::" RPPLUGINS_ID_STRING, fmt::format("Unknown method: ", method));
        return false;
    }

    return true;
}

// ************************************************************************************************

ConfigureStaticInit(NodePath)
{
    auto& resolver_map = get_resolver_map();
    resolver_map["NodePath"] = resolve_nodepath;
}

}    // namespace rpeditor
