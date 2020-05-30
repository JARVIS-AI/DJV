// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvCore/FileInfo.h>

//#pragma optimize("", off)

namespace djv
{
    namespace Core
    {
        namespace FileSystem
        {
            std::string getFilePermissionsLabel(int in)
            {
                const std::vector<std::string> data =
                {
                    "",
                    "r",
                    "w",
                    "rw",
                    "x",
                    "rx",
                    "wx",
                    "rwx"
                };
                return data[in];
            }

            std::string FileInfo::getFileName(Frame::Number frame, bool path) const
            {
                std::stringstream ss;
                const bool isRoot = std::string(1, Path::getCurrentSeparator()) == _path.get();
                if (isRoot)
                {
                    ss << _path;
                }
                else
                {
                    if (path)
                    {
                        ss << _path.getDirectoryName();
                    }
                    ss << _path.getBaseName();
                    if (FileType::Sequence == _type && _sequence.ranges.size() && frame != Frame::invalid)
                    {
                        ss << Frame::toString(frame, _sequence.pad);
                    }
                    else if (FileType::Sequence == _type && _sequence.ranges.size())
                    {
                        ss << _sequence;
                    }
                    else
                    {
                        ss << _path.getNumber();
                    }
                    ss << _path.getExtension();
                }
                return ss.str();
            }

            void FileInfo::setPath(const Path& value, bool stat)
            {
                _path           = value;
                _exists         = false;
                _type           = FileType::File;
                _size           = 0;
                _user           = 0;
                _permissions    = 0;
                _time           = 0;
                _sequence       = Frame::Sequence();

                // Get information from the file system.
                if (stat)
                {
                    this->stat();
                }
            }

            void FileInfo::setPath(const Path& value, FileType fileType, const Frame::Sequence& sequence, bool stat)
            {
                _path           = value;
                _exists         = false;
                _type           = fileType;
                _size           = 0;
                _user           = 0;
                _permissions    = 0;
                _time           = 0;
                _sequence       = sequence;

                // Get information from the file system.
                if (stat)
                {
                    this->stat();
                }
            }

            void FileInfo::setSequence(const Frame::Sequence & in)
            {
                _sequence = in;
                std::stringstream s;
                s << _sequence;
                _path.setNumber(s.str());
            }

            FileInfo FileInfo::getFileSequence(const Path& path, const std::set<std::string>& extensions)
            {
                FileInfo out(path);
                DirectoryListOptions options;
                options.fileSequences = true;
                options.fileSequenceExtensions = extensions;
                std::string dir = path.getDirectoryName();
                if (dir.empty())
                {
                    dir = ".";
                }
                for (const auto& fileInfo : directoryList(Path(dir), options))
                {
                    if (fileInfo.isCompatible(out))
                    {
                        out = fileInfo;
                        break;
                    }
                }
                return out;
            }

            void FileInfo::_fileSequence(FileInfo& fileInfo, const DirectoryListOptions& options, std::vector<FileInfo>& out)
            {
                std::string extension = fileInfo.getPath().getExtension();
                std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
                const auto i = std::find(
                    options.fileSequenceExtensions.begin(),
                    options.fileSequenceExtensions.end(),
                    extension);
                if (options.fileSequences && i != options.fileSequenceExtensions.end())
                {
                    const size_t size = out.size();
                    size_t j = 0;
                    for (; j < size; ++j)
                    {
                        if (out[j].addToSequence(fileInfo))
                        {
                            break;
                        }
                    }
                    if (size == j)
                    {
                        out.push_back(fileInfo);
                    }
                }
                else
                {
                    out.push_back(fileInfo);
                }
            }

            void FileInfo::_sort(const DirectoryListOptions& options, std::vector<FileInfo>& out)
            {
                for (auto & i : out)
                {
                    if (FileType::Sequence == i._type)
                    {
                        i._sequence.sort();
                        std::stringstream s;
                        s << i._sequence;
                        i._path.setNumber(s.str());
                    }
                }

                switch (options.sort)
                {
                case DirectoryListSort::Name:
                    std::sort(
                        out.begin(), out.end(),
                        [&options](const FileInfo & a, const FileInfo & b)
                    {
                        return options.reverseSort ?
                            (a.getFileName(Frame::invalid, false) > b.getFileName(Frame::invalid, false)) :
                            (a.getFileName(Frame::invalid, false) < b.getFileName(Frame::invalid, false));
                    });
                    break;
                case DirectoryListSort::Size:
                    std::sort(
                        out.begin(), out.end(),
                        [&options](const FileInfo & a, const FileInfo & b)
                    {
                        return options.reverseSort ? (a.getSize() > b.getSize()) : (a.getSize() < b.getSize());
                    });
                    break;
                case DirectoryListSort::Time:
                    std::sort(
                        out.begin(), out.end(),
                        [&options](const FileInfo & a, const FileInfo & b)
                    {
                        return options.reverseSort ? (a.getTime() > b.getTime()) : (a.getTime() < b.getTime());
                    });
                    break;
                default: break;
                }
                if (options.sortDirectoriesFirst)
                {
                    std::stable_sort(
                        out.begin(), out.end(),
                        [](const FileInfo & a, const FileInfo & b)
                    {
                        return FileType::Directory == a.getType() && b.getType() != FileType::Directory;
                    });
                }
            }

        } // namespace FileSystem
    } // namespace Core

    DJV_ENUM_SERIALIZE_HELPERS_IMPLEMENTATION(
        Core::FileSystem,
        FileType,
        DJV_TEXT("file_type_file"),
        DJV_TEXT("file_type_sequence"),
        DJV_TEXT("file_type_directory"));

    DJV_ENUM_SERIALIZE_HELPERS_IMPLEMENTATION(
        Core::FileSystem,
        DirectoryListSort,
        DJV_TEXT("directory_list_sort_name"),
        DJV_TEXT("directory_list_sort_size"),
        DJV_TEXT("directory_list_sort_time"));

    picojson::value toJSON(Core::FileSystem::FileType value)
    {
        std::stringstream ss;
        ss << value;
        return picojson::value(ss.str());
    }

    picojson::value toJSON(Core::FileSystem::DirectoryListSort value)
    {
        std::stringstream ss;
        ss << value;
        return picojson::value(ss.str());
    }

    picojson::value toJSON(const Core::FileSystem::FileInfo& value)
    {
        picojson::value out(picojson::object_type, true);
        out.get<picojson::object>()["Path"] = toJSON(value.getPath());
        out.get<picojson::object>()["Type"] = toJSON(value.getType());
        return out;
    }

    void fromJSON(const picojson::value& value, Core::FileSystem::FileType& out)
    {
        if (value.is<std::string>())
        {
            std::stringstream ss(value.get<std::string>());
            ss >> out;
        }
        else
        {
            //! \todo How can we translate this?
            throw std::invalid_argument(DJV_TEXT("error_cannot_parse_the_value"));
        }
    }

    void fromJSON(const picojson::value& value, Core::FileSystem::DirectoryListSort& out)
    {
        if (value.is<std::string>())
        {
            std::stringstream ss(value.get<std::string>());
            ss >> out;
        }
        else
        {
            //! \todo How can we translate this?
            throw std::invalid_argument(DJV_TEXT("error_cannot_parse_the_value"));
        }
    }

    void fromJSON(const picojson::value& value, Core::FileSystem::FileInfo& out)
    {
        if (value.is<picojson::object>())
        {
            Core::FileSystem::Path path;
            Core::FileSystem::FileType type = Core::FileSystem::FileType::First;
            for (const auto& i : value.get<picojson::object>())
            {
                if ("Path" == i.first)
                {
                    fromJSON(i.second, path);
                }
                else if ("Type" == i.first)
                {
                    fromJSON(i.second, type);
                }
            }
            Core::Frame::Sequence sequence;
            if (Core::FileSystem::FileType::Sequence == type)
            {
                std::stringstream ss(path.getNumber());
                ss >> sequence;
            }
            out = Core::FileSystem::FileInfo(path, type, sequence);
        }
        else
        {
            //! \todo How can we translate this?
            throw std::invalid_argument(DJV_TEXT("error_cannot_parse_the_value"));
        }
    }

    std::ostream& operator << (std::ostream& s, const Core::FileSystem::FileInfo& value)
    {
        s << value.getPath();
        return s;
    }

} // namespace djv
