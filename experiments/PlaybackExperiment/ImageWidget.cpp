// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020 Darby Johnston
// All rights reserved.

#include "ImageWidget.h"

#include <djvUI/Style.h>

#include <djvRender2D/Render.h>

using namespace djv;

void ImageWidget::_init(const std::shared_ptr<System::Context>& context)
{
    Widget::_init(context);
}

ImageWidget::ImageWidget()
{}

ImageWidget::~ImageWidget()
{}

std::shared_ptr<ImageWidget> ImageWidget::create(const std::shared_ptr<System::Context>& context)
{
    auto out = std::shared_ptr<ImageWidget>(new ImageWidget);
    out->_init(context);
    return out;
}

void ImageWidget::setImage(const std::shared_ptr<djv::Image::Image>& value)
{
    if (_image == value)
        return;
    _image = value;
    _redraw();
}

void ImageWidget::_preLayoutEvent(System::Event::PreLayout&)
{
    const auto& style = _getStyle();
    const float sa = style->getMetric(UI::MetricsRole::ScrollArea);
    _setMinimumSize(glm::vec2(sa, sa));
}

void ImageWidget::_paintEvent(System::Event::Paint&)
{
    const auto& g = getGeometry();
    const auto& render = _getRender();
    render->setFillColor(Image::Color(0.F, 0.F, 0.F));
    render->drawRect(g);
    if (_image)
    {
        render->setFillColor(Image::Color(1.F, 1.F, 1.F));
        render->drawImage(_image, g.min);
    }
}
