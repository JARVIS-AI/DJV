// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvTestLib/Test.h>

namespace djv
{
    namespace SystemTest
    {
        class FileTest : public Test::ITest
        {
        public:
            FileTest(
                const System::File::Path& tempPath,
                const std::shared_ptr<System::Context>&);
            
            void run() override;
        };
        
    } // namespace SystemTest
} // namespace djv

