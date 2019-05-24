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

#include <djvAV/Color.h>

#include <string.h>

using namespace djv::Core;

namespace djv
{
    namespace AV
    {
        namespace Image
        {
            void Color::zero()
            {
                memset(_data.data(), 0, getByteCount(_type));
            }

            Color Color::convert(Type type) const
            {
                Color out(type);
                Image::convert(_data.data(), _type, out._data.data(), type, 1);
                return out;
            }

            void Color::rgbToHSV(const float in[3], float out[3])
            {
                const float & min = std::min(in[0], std::min(in[1], in[2]));
                const float & max = std::max(in[0], std::max(in[1], in[2]));
                out[2] = max;
                out[1] = max != 0.f ? (1.f - min / max) : 0.f;
                const float v = (max - min) * 6.f;
                if (max == min)
                {
                    out[0] = 0.f;
                }
                else if (in[0] == max)
                {
                    if (in[1] >= in[2])
                    {
                        out[0] = (in[1] - in[2]) / v;
                    }
                    else
                    {
                        out[0] = 1.f + (in[1] - in[2]) / v;
                    }
                }
                else if (in[1] == max)
                {
                    out[0] = 1.f / 3.f + (in[2] - in[0]) / v;
                }
                else if (in[2] == max)
                {
                    out[0] = 2.f / 3.f + (in[0] - in[1]) / v;
                }
            }

            namespace
            {
                inline float mod(float value, float mod)
                {
                    float tmp = value;
                    if (mod != 0.f)
                    {
                        tmp = value - (static_cast<int>(value / mod) * mod);
                        if (tmp < 0.f)
                        {
                            tmp += mod;
                        }
                    }
                    return tmp;
                }

            } // namespace

            void Color::hsvToRGB(const float in[3], float out[3])
            {
                const float h = mod(in[0] * 6.f, 6.f);
                const int   i = static_cast<int>(floorf(h));
                const float f = h - i;
                const float p = in[2] * (1.f - in[1]);
                const float q = in[2] * (1.f - (in[1] * f));
                const float t = in[2] * (1.f - (in[1] * (1.f - f)));
                switch (i)
                {
                case 0:
                    out[0] = in[2];
                    out[1] = t;
                    out[2] = p;
                    break;
                case 1:
                    out[0] = q;
                    out[1] = in[2];
                    out[2] = p;
                    break;
                case 2:
                    out[0] = p;
                    out[1] = in[2];
                    out[2] = t;
                    break;
                case 3:
                    out[0] = p;
                    out[1] = q;
                    out[2] = in[2];
                    break;
                case 4:
                    out[0] = t;
                    out[1] = p;
                    out[2] = in[2];
                    break;
                case 5:
                    out[0] = in[2];
                    out[1] = p;
                    out[2] = q;
                    break;
                }
            }

        } // namepsace Image
    } // namespace AV

    std::ostream & operator << (std::ostream & os, const AV::Image::Color & value)
    {
        const auto type = value.getType();
        os << type;
        const size_t channelCount = AV::Image::getChannelCount(type);
        switch (AV::Image::getDataType(type))
        {
        case AV::Image::DataType::U8:
        {
            os << " ";
            for (size_t i = 0; i < channelCount; ++i)
            {
                os << static_cast<uint32_t>(value.getU8(i));
                if (i < channelCount - 1)
                {
                    os << " ";
                }
            }
            break;
        }
        case AV::Image::DataType::U10:
        {
            os << " ";
            for (size_t i = 0; i < channelCount; ++i)
            {
                os << value.getU10(i);
                if (i < channelCount - 1)
                {
                    os << " ";
                }
            }
            break;
        }
        case AV::Image::DataType::U16:
        {
            os << " ";
            for (size_t i = 0; i < channelCount; ++i)
            {
                os << value.getU16(i);
                if (i < channelCount - 1)
                {
                    os << " ";
                }
            }
            break;
        }
        case AV::Image::DataType::U32:
        {
            os << " ";
            for (size_t i = 0; i < channelCount; ++i)
            {
                os << value.getU32(i);
                if (i < channelCount - 1)
                {
                    os << " ";
                }
            }
            break;
        }
        case AV::Image::DataType::F16:
        {
            os << " ";
            for (size_t i = 0; i < channelCount; ++i)
            {
                os << value.getF16(i);
                if (i < channelCount - 1)
                {
                    os << " ";
                }
            }
            break;
        }
        case AV::Image::DataType::F32:
        {
            os << " ";
            for (size_t i = 0; i < channelCount; ++i)
            {
                os << value.getF32(i);
                if (i < channelCount - 1)
                {
                    os << " ";
                }
            }
            break;
        }
        default: break;
        }
        return os;
    }

    std::istream & operator >> (std::istream & is, AV::Image::Color & value)
    {
        AV::Image::Type type = AV::Image::Type::None;
        is >> type;
        value = AV::Image::Color(type);
        const size_t channelCount = AV::Image::getChannelCount(type);
        switch (AV::Image::getDataType(type))
        {
        case AV::Image::DataType::U8:
        {
            for (size_t i = 0; i < channelCount; ++i)
            {
                uint32_t tmp = 0;
                is >> tmp;
                value.setU8(tmp, i);
            }
            break;
        }
        case AV::Image::DataType::U10:
        {
            for (size_t i = 0; i < channelCount; ++i)
            {
                AV::Image::U10_T tmp = 0;
                is >> tmp;
                value.setU10(tmp, i);
            }
            break;
        }
        case AV::Image::DataType::U16:
        {
            for (size_t i = 0; i < channelCount; ++i)
            {
                AV::Image::U16_T tmp = 0;
                is >> tmp;
                value.setU16(tmp, i);
            }
            break;
        }
        case AV::Image::DataType::U32:
        {
            for (size_t i = 0; i < channelCount; ++i)
            {
                AV::Image::U32_T tmp = 0;
                is >> tmp;
                value.setU32(tmp, i);
            }
            break;
        }
        case AV::Image::DataType::F16:
        {
            for (size_t i = 0; i < channelCount; ++i)
            {
                AV::Image::F16_T tmp = 0.f;
                is >> tmp;
                value.setF16(tmp, i);
            }
            break;
        }
        case AV::Image::DataType::F32:
        {
            for (size_t i = 0; i < channelCount; ++i)
            {
                AV::Image::F32_T tmp = 0.f;
                is >> tmp;
                value.setF32(tmp, i);
            }
            break;
        }
        default: break;
        }
        return is;
    }

} // namespace djv

