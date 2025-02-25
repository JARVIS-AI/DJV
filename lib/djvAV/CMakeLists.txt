set(header
    AVSystem.h
    Cineon.h
    DPX.h
    IFF.h
    IO.h
    IOInline.h
    IOPlugin.h
    IOPluginInline.h
    IOSystem.h
    PFM.h
    PPM.h
    RLA.h
    SGI.h
    SequenceIO.h
    Speed.h
    Targa.h
    ThumbnailSystem.h
    Time.h
    TimeInline.h)
set(source
    AVSystem.cpp
    Cineon.cpp
    CineonRead.cpp
    CineonWrite.cpp
    DPX.cpp
    DPXRead.cpp
    DPXWrite.cpp
    IFF.cpp
    IFFRead.cpp
    IO.cpp
    IOPlugin.cpp
    IOSystem.cpp
    PFM.cpp
    PFMRead.cpp
    PPM.cpp
    PPMRead.cpp
    PPMWrite.cpp
    RLA.cpp
    RLARead.cpp
    SequenceIO.cpp
    SGI.cpp
    SGIRead.cpp
    Speed.cpp
    Targa.cpp
    TargaRead.cpp
    ThumbnailSystem.cpp
    Time.cpp)
if(FFmpeg_FOUND)
    set(header
        ${header}
		FFmpeg.h)
    set(source
        ${source}
		FFmpeg.cpp
		FFmpegRead.cpp)
endif()
if(JPEG_FOUND)
    set(header
        ${header}
        JPEG.h)
    set(source
        ${source}
        JPEG.cpp
        JPEGRead.cpp
        JPEGWrite.cpp)
endif()
if(PNG_FOUND)
    set(header
        ${header}
        PNG.h)
    set(source
        ${source}
        PNG.cpp
        PNGRead.cpp
        PNGWrite.cpp)
endif()
if(OpenEXR_FOUND)
    set(header
        ${header}
        OpenEXR.h)
    set(source
        ${source}
        OpenEXR.cpp
        OpenEXRRead.cpp
        OpenEXRWrite.cpp)
endif()
if(TIFF_FOUND)
    set(header
        ${header}
        TIFF.h)
    set(source
        ${source}
        TIFF.cpp
        TIFFRead.cpp
        TIFFWrite.cpp)
endif()

add_library(djvAV ${header} ${source})
set(LIBRARIES
    djvOCIO
    djvAudio
    #OpenAL
    RtAudio)
if(FFmpeg_FOUND)
    set(LIBRARIES ${LIBRARIES} FFmpeg)
endif()
if(JPEG_FOUND)
    set(LIBRARIES ${LIBRARIES} JPEG)
endif()
if(PNG_FOUND)
    set(LIBRARIES ${LIBRARIES} PNG)
endif()
if(OpenEXR_FOUND)
    set(LIBRARIES ${LIBRARIES} OpenEXR::IlmImf)
endif()
if(TIFF_FOUND)
    set(LIBRARIES ${LIBRARIES} TIFF)
endif()
target_link_libraries(djvAV ${LIBRARIES})
set_target_properties(
    djvAV
    PROPERTIES
    FOLDER lib
    CXX_STANDARD 11)

#install(
#    TARGETS djvAV
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib)
#install(FILES ${header} DESTINATION include/djvAV)

