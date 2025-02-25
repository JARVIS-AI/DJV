// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvUI/Widget.h>

namespace djv
{
    namespace UI
    {
        namespace MDI
        {
            class Canvas;

            //! MDI move and resize handles.
            enum class Handle
            {
                None,
                Move,
                ResizeE,
                ResizeN,
                ResizeW,
                ResizeS,
                ResizeNE,
                ResizeNW,
                ResizeSW,
                ResizeSE,

                Count,
                First = None
            };

            //! Base class for MDI widgets.
            class IWidget : public Widget
            {
                DJV_NON_COPYABLE(IWidget);

            protected:
                void _init(const std::shared_ptr<System::Context>&);
                IWidget();

            public:
                virtual ~IWidget() = 0;

            protected:
                virtual std::map<Handle, std::vector<Math::BBox2f> > _getHandles() const;
                virtual std::map<Handle, std::vector<Math::BBox2f> > _getHandlesDraw() const;

                float _getMaximize() const;
                virtual void _setMaximize(float);
                void _setHandleHovered(Handle);
                void _setHandlePressed(Handle);

                virtual void _setActiveWidget(bool);

                void _paintEvent(System::Event::Paint&) override;
                void _paintOverlayEvent(System::Event::PaintOverlay&) override;

            private:
                DJV_PRIVATE();

                friend class Canvas;
            };
            
        } // namespace MDI
    } // namespace UI
} // namespace djv
