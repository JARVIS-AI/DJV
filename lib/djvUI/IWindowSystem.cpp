//------------------------------------------------------------------------------
// Copyright (c) 2004-2019 Darby Johnston
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

#include <djvUI/IWindowSystem.h>

#include <djvUI/UISystem.h>
#include <djvUI/Window.h>

//#pragma optimize("", off)

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        struct IWindowSystem::Private
        {
            std::shared_ptr<ListSubject<std::shared_ptr<Window> > > windows;
            std::shared_ptr<ValueSubject<std::shared_ptr<Window> > > currentWindow;
            std::set<std::shared_ptr<Style::Style> > dirtyStyles;
        };

        void IWindowSystem::_init(const std::string & name, Core::Context * context)
        {
            ISystem::_init(name, context);
            DJV_PRIVATE_PTR();
            p.windows = ListSubject< std::shared_ptr<Window> >::create();
            p.currentWindow = ValueSubject<std::shared_ptr<Window> >::create();
        }

        IWindowSystem::IWindowSystem() :
            _p(new Private)
        {}

        IWindowSystem::~IWindowSystem()
        {}

        std::shared_ptr<Core::IListSubject<std::shared_ptr<Window> > > IWindowSystem::observeWindows() const
        {
            return _p->windows;
        }

        std::shared_ptr<Core::IValueSubject<std::shared_ptr<Window> > > IWindowSystem::observeCurrentWindow() const
        {
            return _p->currentWindow;
        }

        void IWindowSystem::tick(float dt)
        {
            DJV_PRIVATE_PTR();
            if (auto uiSystem = getContext()->getSystemT<UISystem>().lock())
            {
                auto style = uiSystem->getStyle();
                if (style->isDirty())
                {
                    Event::Style styleEvent;
                    for (auto window : p.windows->get())
                    {
                        _styleRecursive(window, styleEvent);
                    }
                    style->setClean();
                }
            }
        }

        void IWindowSystem::_addWindow(const std::shared_ptr<Window>& window)
        {
            DJV_PRIVATE_PTR();
            p.windows->pushBack(window);
            p.currentWindow->setIfChanged(window);
        }

        void IWindowSystem::_removeWindow(const std::shared_ptr<Window>& window)
        {
            DJV_PRIVATE_PTR();
            const size_t index = p.windows->indexOf(window);
            if (index != invalidListIndex)
            {
                p.windows->removeItem(index);
            }
            if (window == p.currentWindow->get())
            {
                const size_t size = p.windows->getSize();
                p.currentWindow->setIfChanged(size > 0 ? p.windows->getItem(size - 1) : nullptr);
            }
        }

        void IWindowSystem::_pushClipRect(const Core::BBox2f &)
        {}

        void IWindowSystem::_popClipRect()
        {}

        bool IWindowSystem::_resizeRequest(const std::shared_ptr<UI::Widget>& widget) const
        {
            bool out = widget->_resizeRequest;
            widget->_resizeRequest = false;
            return out;
        }

        bool IWindowSystem::_redrawRequest(const std::shared_ptr<UI::Widget>& widget) const
        {
            bool out = widget->_redrawRequest;
            widget->_redrawRequest = false;
            return out;
        }

        void IWindowSystem::_styleRecursive(const std::shared_ptr<UI::Widget>& widget, Event::Style& event)
        {
            widget->event(event);
            for (const auto& child : widget->getChildrenT<UI::Widget>())
            {
                _styleRecursive(child, event);
            }
        }

        void IWindowSystem::_preLayoutRecursive(const std::shared_ptr<UI::Widget>& widget, Event::PreLayout& event)
        {
            for (const auto& child : widget->getChildrenT<UI::Widget>())
            {
                _preLayoutRecursive(child, event);
            }
            widget->event(event);
        }

        void IWindowSystem::_layoutRecursive(const std::shared_ptr<UI::Widget>& widget, Event::Layout& event)
        {
            if (widget->isVisible())
            {
                widget->event(event);
                for (const auto& child : widget->getChildrenT<UI::Widget>())
                {
                    _layoutRecursive(child, event);
                }
            }
        }

        void IWindowSystem::_clipRecursive(const std::shared_ptr<UI::Widget>& widget, Event::Clip& event)
        {
            widget->event(event);
            const BBox2f clipRect = event.getClipRect();
            for (const auto& child : widget->getChildrenT<UI::Widget>())
            {
                event.setClipRect(clipRect.intersect(child->getGeometry()));
                _clipRecursive(child, event);
            }
            event.setClipRect(clipRect);
        }

        void IWindowSystem::_paintRecursive(const std::shared_ptr<UI::Widget>& widget, Event::Paint& event, Event::PaintOverlay& overlayEvent)
        {
            if (widget->isVisible() && !widget->isClipped())
            {
                const BBox2f clipRect = event.getClipRect();
                _pushClipRect(clipRect);
                widget->event(event);
                for (const auto& child : widget->getChildrenT<UI::Widget>())
                {
                    const BBox2f newClipRect = clipRect.intersect(child->getGeometry());
                    event.setClipRect(newClipRect);
                    overlayEvent.setClipRect(newClipRect);
                    _paintRecursive(child, event, overlayEvent);
                }
                widget->event(overlayEvent);
                _popClipRect();
                event.setClipRect(clipRect);
                overlayEvent.setClipRect(clipRect);
            }
        }

    } // namespace UI
} // namespace djv
