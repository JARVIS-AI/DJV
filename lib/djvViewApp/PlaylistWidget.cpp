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

#include <djvViewApp/PlaylistWidget.h>

#include <djvViewApp/FileSystem.h>
#include <djvViewApp/Media.h>
#include <djvViewApp/MediaWidget.h>

#include <djvUI/ButtonGroup.h>
#include <djvUI/FormLayout.h>
#include <djvUI/IButton.h>
#include <djvUI/ImageWidget.h>
#include <djvUI/Label.h>
#include <djvUI/RowLayout.h>
#include <djvUI/ScrollWidget.h>
#include <djvUI/Splitter.h>

#include <djvAV/Render2D.h>

#include <djvCore/Context.h>
#include <djvCore/Path.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewApp
    {
        namespace
        {
            class Button : public UI::Button::IButton
            {
                DJV_NON_COPYABLE(Button);

            protected:
                void _init(const std::shared_ptr<Media>&, Context*);
                Button()
                {}

            public:
                static std::shared_ptr<Button> create(const std::shared_ptr<Media>&, Context*);

            protected:
                void _preLayoutEvent(Event::PreLayout&) override;
                void _layoutEvent(Event::Layout&) override;
                void _paintEvent(Event::Paint&) override;

                void _localeEvent(Event::Locale&) override;

            private:
                void _textUpdate();

                AV::IO::Info _info;
                std::shared_ptr<UI::Label> _label;
                std::shared_ptr<UI::FormLayout> _infoLayout;
                std::shared_ptr<UI::VerticalLayout> _layout;
                std::shared_ptr<ValueObserver<std::shared_ptr<AV::Image::Image> > > _imageObserver;
                std::shared_ptr<ValueObserver<AV::IO::Info> > _infoObserver;
            };

            void Button::_init(const std::shared_ptr<Media>& media, Context* context)
            {
                IButton::_init(context);

                setClassName("djv::ViewApp::PlaylistWidget::Button");

                _label = UI::Label::create(context);
                _label->setText(Core::FileSystem::Path(media->getFileName()).getFileName());
                _label->setTextHAlign(UI::TextHAlign::Left);
                _label->setFontSizeRole(UI::MetricsRole::FontLarge);

                auto imageWidget = UI::ImageWidget::create(context);
                imageWidget->setSizeRole(UI::MetricsRole::TextColumn);

                _infoLayout = UI::FormLayout::create(context);
                _infoLayout->setMargin(UI::MetricsRole::MarginSmall);
                _infoLayout->setSpacing(UI::MetricsRole::SpacingSmall);
                auto scrollWidget = UI::ScrollWidget::create(UI::ScrollType::Vertical, context);
                scrollWidget->setMinimumSizeRole(UI::MetricsRole::MarginSmall);
                scrollWidget->addChild(_infoLayout);

                _layout = UI::VerticalLayout::create(context);
                _layout->setMargin(UI::MetricsRole::Margin);
                _layout->addChild(_label);
                auto hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(imageWidget);
                hLayout->addChild(scrollWidget);
                hLayout->setStretch(scrollWidget, UI::RowStretch::Expand);
                _layout->addChild(hLayout);
                _layout->setStretch(hLayout, UI::RowStretch::Expand);
                addChild(_layout);

                _imageObserver = ValueObserver<std::shared_ptr<AV::Image::Image> >::create(
                    media->observeCurrentImage(),
                    [imageWidget](const std::shared_ptr<AV::Image::Image> & value)
                {
                    imageWidget->setImage(value);
                });

                auto weak = std::weak_ptr<Button>(std::dynamic_pointer_cast<Button>(shared_from_this()));
                _infoObserver = ValueObserver<AV::IO::Info>::create(
                    media->observeInfo(),
                    [weak](const AV::IO::Info & value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_info = value;
                        widget->_textUpdate();
                    }
                });
            }

            std::shared_ptr<Button> Button::create(const std::shared_ptr<Media> & media, Context* context)
            {
                auto out = std::shared_ptr<Button>(new Button);
                out->_init(media, context);
                return out;
            }

            void Button::_preLayoutEvent(Event::PreLayout& event)
            {
                auto style = _getStyle();
                const float m = style->getMetric(UI::MetricsRole::MarginSmall);
                glm::vec2 size = _layout->getMinimumSize();
                size.x += m;
                _setMinimumSize(size);
            }

            void Button::_layoutEvent(Event::Layout&)
            {
                auto style = _getStyle();
                BBox2f g = getGeometry();
                const float m = style->getMetric(UI::MetricsRole::MarginSmall);
                g.min.x += m;
                _layout->setGeometry(g);
            }

            void Button::_paintEvent(Event::Paint& event)
            {
                IButton::_paintEvent(event);
                auto render = _getRender();
                auto style = _getStyle();
                const BBox2f& g = getGeometry();
                const float m = style->getMetric(UI::MetricsRole::MarginSmall);
                if (_isToggled())
                {
                    render->setFillColor(_getColorWithOpacity(style->getColor(UI::ColorRole::Checked)));
                    render->drawRect(BBox2f(g.min.x, g.min.y, m, g.h()));
                }
                if (_isPressed())
                {
                    render->setFillColor(_getColorWithOpacity(style->getColor(UI::ColorRole::Pressed)));
                    render->drawRect(g);
                }
                else if (_isHovered())
                {
                    render->setFillColor(_getColorWithOpacity(style->getColor(UI::ColorRole::Hovered)));
                    render->drawRect(g);
                }
            }

            void Button::_localeEvent(Event::Locale& event)
            {
                _textUpdate();
            }

            void Button::_textUpdate()
            {
                _infoLayout->clearChildren();
                auto context = getContext();
                size_t j = 0;
                size_t k = 0;
                for (const auto& i : _info.video)
                {
                    std::string text;
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Video track")) << " #" << j;
                        text = ss.str();
                    }
                    auto label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    ++k;

                    {
                        std::stringstream ss;
                        ss << i.info.size;
                        text = ss.str();
                    }
                    label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Size")) << " :";
                        text = ss.str();
                    }
                    _infoLayout->setText(label, text);
                    ++k;

                    {
                        std::stringstream ss;
                        ss << i.info.getAspectRatio();
                        text = ss.str();
                    }
                    label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Aspect ratio")) << " :";
                        text = ss.str();
                    }
                    _infoLayout->setText(label, text);
                    ++k;

                    {
                        std::stringstream ss;
                        ss << i.info.type;
                        text = ss.str();
                    }
                    label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Type")) << " :";
                        text = ss.str();
                    }
                    _infoLayout->setText(label, text);
                    ++k;

                    {
                        std::stringstream ss;
                        ss << i.speed;
                        text = ss.str();
                    }
                    label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Speed")) << " :";
                        text = ss.str();
                    }
                    _infoLayout->setText(label, text);
                    ++k;

                    {
                        std::stringstream ss;
                        ss << i.info.type;
                        text = ss.str();
                    }
                    label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Duration")) << " :";
                        text = ss.str();
                    }
                    _infoLayout->setText(label, text);
                    ++k;

                    ++j;
                }
                j = 0;
                k = 0;
                for (const auto& i : _info.audio)
                {
                    std::string text;
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Audio track")) << " #" << j;
                        text = ss.str();
                    }
                    auto label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    ++k;

                    {
                        std::stringstream ss;
                        ss << i.info.channelCount;
                        text = ss.str();
                    }
                    label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Channels")) << " :";
                        text = ss.str();
                    }
                    _infoLayout->setText(label, text);
                    ++k;

                    {
                        std::stringstream ss;
                        ss << i.info.type;
                        text = ss.str();
                    }
                    label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Type")) << " :";
                        text = ss.str();
                    }
                    _infoLayout->setText(label, text);
                    ++k;

                    {
                        std::stringstream ss;
                        ss << i.info.sampleRate;
                        text = ss.str();
                    }
                    label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Sample rate")) << " :";
                        text = ss.str();
                    }
                    _infoLayout->setText(label, text);
                    ++k;

                    {
                        std::stringstream ss;
                        ss << i.info.type;
                        text = ss.str();
                    }
                    label = UI::Label::create(context);
                    label->setText(text);
                    label->setTextHAlign(UI::TextHAlign::Left);
                    _infoLayout->addChild(label);
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("Duration")) << " :";
                        text = ss.str();
                    }
                    _infoLayout->setText(label, text);
                    ++k;

                    ++j;
                }
            }

            class ListWidget : public UI::Widget
            {
                DJV_NON_COPYABLE(ListWidget);

            protected:
                void _init(Context*);
                ListWidget()
                {}

            public:
                static std::shared_ptr<ListWidget> create(Context*);

            protected:
                void _preLayoutEvent(Event::PreLayout&) override;
                void _layoutEvent(Event::Layout&) override;

            private:
                std::vector<std::shared_ptr<Media> > _media;
                std::shared_ptr<UI::ButtonGroup> _buttonGroup;
                std::shared_ptr<UI::VerticalLayout> _layout;
                std::shared_ptr<ListObserver<std::shared_ptr<Media> > > _mediaObserver;
                std::shared_ptr<ValueObserver<std::shared_ptr<Media> > > _currentMediaObserver;
            };

            void ListWidget::_init(Context* context)
            {
                Widget::_init(context);

                setClassName("djv::ViewApp::PlaylistWidget::ListWidget");

                _buttonGroup = UI::ButtonGroup::create(UI::ButtonType::Radio);
                
                _layout = UI::VerticalLayout::create(context);
                _layout->setSpacing(UI::MetricsRole::None);
                addChild(_layout);

                auto weak = std::weak_ptr<ListWidget>(std::dynamic_pointer_cast<ListWidget>(shared_from_this()));
                _buttonGroup->setRadioCallback(
                    [weak, context](int index)
                {
                    if (auto widget = weak.lock())
                    {
                        auto fileSystem = context->getSystemT<FileSystem>();
                        fileSystem->setCurrentMedia(widget->_media[index]);
                    }
                });

                if (auto fileSystem = context->getSystemT<FileSystem>())
                {
                    _mediaObserver = ListObserver<std::shared_ptr<Media>>::create(
                        fileSystem->observeMedia(),
                        [weak, context](const std::vector<std::shared_ptr<Media> > & value)
                    {
                        if (auto widget = weak.lock())
                        {
                            widget->_media = value;
                            widget->_buttonGroup->clearButtons();
                            widget->_layout->clearChildren();
                            size_t j = 0;
                            for (const auto& i : value)
                            {
                                auto button = Button::create(i, context);
                                widget->_buttonGroup->addButton(button);
                                widget->_layout->addChild(button);
                                if (j < value.size() - 1)
                                {
                                    widget->_layout->addSeparator();
                                }
                                ++j;
                            }
                        }
                    });

                    _currentMediaObserver = ValueObserver<std::shared_ptr<Media>>::create(
                        fileSystem->observeCurrentMedia(),
                        [weak](const std::shared_ptr<Media> & value)
                    {
                        if (auto widget = weak.lock())
                        {
                            const auto i = std::find(widget->_media.begin(), widget->_media.end(), value);
                            if (i != widget->_media.end())
                            {
                                widget->_buttonGroup->setChecked(i - widget->_media.begin());
                            }
                        }
                    });
                }
            }

            std::shared_ptr<ListWidget> ListWidget::create(Context* context)
            {
                auto out = std::shared_ptr<ListWidget>(new ListWidget);
                out->_init(context);
                return out;
            }

            void ListWidget::_preLayoutEvent(Event::PreLayout&)
            {
                _setMinimumSize(_layout->getMinimumSize());
            }

            void ListWidget::_layoutEvent(Event::Layout&)
            {
                _layout->setGeometry(getGeometry());
            }

            class MenuBarSpacer : public UI::Widget
            {
                DJV_NON_COPYABLE(MenuBarSpacer);

            protected:
                MenuBarSpacer()
                {}

            public:
                static std::shared_ptr<MenuBarSpacer> create(Context*);

            protected:
                void _preLayoutEvent(Event::PreLayout&) override;
            };

            std::shared_ptr<MenuBarSpacer> MenuBarSpacer::create(Context* context)
            {
                auto out = std::shared_ptr<MenuBarSpacer>(new MenuBarSpacer);
                out->_init(context);
                return out;
            }

            void MenuBarSpacer::_preLayoutEvent(Event::PreLayout&)
            {
                auto style = _getStyle();
                const float is = style->getMetric(UI::MetricsRole::Icon);
                _setMinimumSize(glm::vec2(0, is));
            }

        } // namespace

        struct PlaylistWidget::Private
        {
            std::shared_ptr<MediaWidget> mediaWidget;
            std::shared_ptr<ListWidget> listWidget;
            std::shared_ptr<UI::Layout::Splitter> splitter;
            std::shared_ptr<ValueObserver<std::shared_ptr<Media> > > currentMediaObserver;
        };

        void PlaylistWidget::_init(Context* context)
        {
            Widget::_init(context);

            DJV_PRIVATE_PTR();
            setClassName("djv::ViewApp::PlaylistWidget");

            p.mediaWidget = MediaWidget::create(context);

            p.listWidget = ListWidget::create(context);
            auto scrollWidget = UI::ScrollWidget::create(UI::ScrollType::Vertical, context);
            scrollWidget->setBorder(false);
            scrollWidget->addChild(p.listWidget);

            p.splitter = UI::Layout::Splitter::create(UI::Orientation::Horizontal, context);
            p.splitter->addChild(p.mediaWidget);
            auto vLayout = UI::VerticalLayout::create(context);
            vLayout->addChild(MenuBarSpacer::create(context));
            vLayout->addChild(scrollWidget);
            vLayout->setStretch(scrollWidget, UI::RowStretch::Expand);
            p.splitter->addChild(vLayout);
            p.splitter->setSplit({ .65f, 1.f });
            addChild(p.splitter);

            auto weak = std::weak_ptr<PlaylistWidget>(std::dynamic_pointer_cast<PlaylistWidget>(shared_from_this()));
            if (auto fileSystem = context->getSystemT<FileSystem>())
            {
                p.currentMediaObserver = ValueObserver<std::shared_ptr<Media>>::create(
                    fileSystem->observeCurrentMedia(),
                    [weak](const std::shared_ptr<Media> & value)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->mediaWidget->setMedia(value);
                    }
                });
            }
        }

        PlaylistWidget::PlaylistWidget() :
            _p(new Private)
        {}

        PlaylistWidget::~PlaylistWidget()
        {}

        std::shared_ptr<PlaylistWidget> PlaylistWidget::create(Context* context)
        {
            auto out = std::shared_ptr<PlaylistWidget>(new PlaylistWidget);
            out->_init(context);
            return out;
        }

        void PlaylistWidget::_preLayoutEvent(Event::PreLayout&)
        {
            _setMinimumSize(_p->splitter->getMinimumSize());
        }

        void PlaylistWidget::_layoutEvent(Event::Layout&)
        {
            _p->splitter->setGeometry(getGeometry());
        }

        void PlaylistWidget::_localeEvent(Event::Locale& event)
        {
            DJV_PRIVATE_PTR();
        }

    } // namespace ViewApp
} // namespace djv

