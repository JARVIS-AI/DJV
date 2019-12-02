//------------------------------------------------------------------------------
// Copyright (c) 2019 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvScene/Scene.h>

#include <djvScene/Camera.h>
#include <djvScene/Primitive.h>

using namespace djv::Core;

namespace djv
{
    namespace Scene
    {
        Scene::Scene()
        {}

        std::shared_ptr<Scene> Scene::create()
        {
            auto out = std::shared_ptr<Scene>(new Scene);
            return out;
        }

        void Scene::addCamera(const std::shared_ptr<ICamera>& value)
        {
            _cameras.push_back(value);
        }

        void Scene::addPrimitive(const std::shared_ptr<IPrimitive>& value)
        {
            _primitives.push_back(value);
        }

        void Scene::addLight(const std::shared_ptr<ILight>& value)
        {
            _lights.push_back(value);
        }

        void Scene::addLayer(const std::shared_ptr<Layer>& value)
        {
            _layers.push_back(value);
        }

    } // namespace Scene
} // namespace djv
