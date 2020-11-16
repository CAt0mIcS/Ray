# File contains all self-defined CMake functions

# Preserves directory structure by creating Visual Studio filters for folders
function(AddRecursiveFilters targetSrc)
    foreach(source IN ITEMS ${targetSrc})
        get_filename_component(sourcePath "${source}" PATH)
        if(IS_ABSOLUTE source)
            file(RELATIVE_PATH relSourcePath "${_src_root_path}" "${sourcePath}")
        else()
            set(relSourcePath "${sourcePath}")
        endif()

        string(REPLACE "/" "\\" groupPath "${relSourcePath}")
        source_group("${groupPath}" FILES "${source}")
    endforeach()

    GeneratePCHFilters()
endfunction()

# Add CMake-generated pch files into their separate filter
function(GeneratePCHFilters)
    get_directory_property(targets BUILDSYSTEM_TARGETS)
    set(configList "${CMAKE_CONFIGURATION_TYPES}")

    foreach(target "${targets}")
        foreach(configuration IN LISTS CMAKE_CONFIGURATION_TYPES ITEMS ${CMAKE_BUILD_TYPE})
            source_group("CMake" FILES "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir/${configuration}/cmake_pch.hxx")
        endforeach()
        source_group("CMake" FILES "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir/cmake_pch.cxx")
    endforeach()
endfunction()

# Set all output directories of a specific target
function(SetOutputDirectory target dir)
    if(MSVC)
        foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
            set(OUTCFGLOW "${OUTPUTCONFIG}")
            string( TOUPPER "${OUTPUTCONFIG}" OUTPUTCONFIG )

            set(OUTPUTPATH "${CMAKE_BINARY_DIR}/bin/${OUTCFGLOW}/${dir}")

            set_target_properties("${target}" PROPERTIES 
                LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${OUTPUTPATH}"
                RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${OUTPUTPATH}"
                ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${OUTPUTPATH}"
            )
        endforeach()
    else()
        set(OUTPUTPATH "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/${dir}")
        set_target_properties("${target}" PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${OUTPUTPATH}"
            LIBRARY_OUTPUT_DIRECTORY "${OUTPUTPATH}"
            RUNTIME_OUTPUT_DIRECTORY "${OUTPUTPATH}"
        )
    endif()
endfunction()
