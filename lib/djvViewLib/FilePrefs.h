//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
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

#pragma once

#include <djvViewLib/AbstractPrefs.h>

#include <djvGraphics/PixelData.h>

#include <djvCore/FileInfo.h>

#include <QStringList>

namespace djv
{
    namespace ViewLib
    {
        //! \class FilePrefs
        //!
        //! This class provides the file group preferences.
        class FilePrefs : public AbstractPrefs
        {
            Q_OBJECT

        public:
            explicit FilePrefs(Context *, QObject * parent = nullptr);
            ~FilePrefs() override;

            //! Add a recent file.
            void addRecent(const Core::FileInfo &);

            //! Get the recent files.
            const Core::FileInfoList & recentFiles() const;

            //! Get the default for whether sequences are automatically opened.
            static bool autoSequenceDefault();

            //! Get whether sequences are automatically opened.
            bool hasAutoSequence() const;

            //! Get the default proxy scale.
            static Graphics::PixelDataInfo::PROXY proxyDefault();

            //! Get the proxy scale.
            Graphics::PixelDataInfo::PROXY proxy() const;

            //! Get the default for whether images are converted to 8-bits.
            static bool u8ConversionDefault();

            //! Get whether images are converted to 8-bits.
            bool hasU8Conversion() const;

            //! Get the default for whether the cache is enabled.
            static bool cacheDefault();

            //! Get whether the cache is enabled.
            bool hasCache() const;

            //! Get the default cache size in gigabytes.
            static float cacheSizeDefault();

            //! Get the cache size in gigabytes.
            float cacheSize() const;

            //! Get the default for whether the cache is pre-loaded.
            static bool preloadDefault();

            //! Get wheter the cache is pre-loaded.
            bool hasPreload() const;

            //! Get the default for whether the cache is displayed in the timeline.
            static bool displayCacheDefault();

            //! Get whether the cache is displayed in the timeline.
            bool hasDisplayCache() const;

        public Q_SLOTS:
            //! Set whether sequences are automatically opened.
            void setAutoSequence(bool);

            //! Set the proxy scale.
            void setProxy(djv::Graphics::PixelDataInfo::PROXY);

            //! Set whether images are converted to 8-bits.
            void setU8Conversion(bool);

            //! Set whether the cache is enabled.
            void setCache(bool);

            //! Set the cache size in gigabytes.
            void setCacheSize(float);

            //! Set whether the cache pre-load is enabled.
            void setPreload(bool);

            //! Set whether the cache is displayed in the timeline.
            void setDisplayCache(bool);

        Q_SIGNALS:
            //! This signal is emitted when the recent files are changed.
            void recentChanged(const djv::Core::FileInfoList &);

            //! This signal is emitted when automatic sequences is changed.
            void autoSequenceChanged(bool);

            //! This signal is emitted when the proxy scale is changed.
            void proxyChanged(djv::Graphics::PixelDataInfo::PROXY);

            //! This signal is emitted when 8-bit conversion is changed.
            void u8ConversionChanged(bool);

            //! This signal is emitted when the cache is enabled or disabled.
            void cacheChanged(bool);

            //! This signal is emitted when the cache size is changed.
            void cacheSizeChanged(float);

            //! This signal is emitted when the cache pre-load is changed.
            void preloadChanged(bool);

            //! This signal is emitted when the cache display is changed.
            void displayCacheChanged(bool);

        private:
            Core::FileInfoList             _recent;
            bool                           _autoSequence;
            Graphics::PixelDataInfo::PROXY _proxy;
            bool                           _u8Conversion;
            bool                           _cache;
            float                          _cacheSize;
            bool                           _preload;
            bool                           _displayCache;
        };

    } // namespace ViewLib
} // namespace djv
