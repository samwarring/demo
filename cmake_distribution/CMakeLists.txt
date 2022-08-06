cmake_minimum_required(VERSION 3.23)
project(UnitsCalculator VERSION 0.1.0)

# -------------------------------------------------------------------
# Define a shared library. If built for Windows, an export header
# must be generated to handle the __declspec directives.
# -------------------------------------------------------------------
add_library(my_units SHARED my_units.cpp)
include(GenerateExportHeader)
generate_export_header(my_units)
target_include_directories(my_units PUBLIC . ${CMAKE_CURRENT_BINARY_DIR})

# -------------------------------------------------------------------
# Define an application that uses the shared library.
# -------------------------------------------------------------------
add_executable(units_calculator units_calculator.cpp)
target_link_libraries(units_calculator PRIVATE my_units)

# -------------------------------------------------------------------
# Define installation rules for the targets. For "Runtime"
# installations, user installs the application and shared library.
# For "Development" installations, user must also install the
# header files.
# -------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS my_units units_calculator
        COMPONENT Runtime
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES my_units.h "${CMAKE_CURRENT_BINARY_DIR}/my_units_export.h"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT Development)

# -------------------------------------------------------------------
# Windows installers should bundle the installer for the Visual
# Studio Redistributable files. The installer should run as part
# of our own installation process. This part simply locates the
# redistributable package. Chaining it to our installer is handled
# further down. The *correct* logic to locate the appropriate redist
# package is rather involved, and ideally should handle all types
# of CMake configurations (e.g. VS generator, Ninja generator with
# MSVC compilers, cross-compiling, etc.). Demonstrating this logic
# is not the purpose of this project, so it just hard-codes a few
# well-known paths.
# -------------------------------------------------------------------
if (MSVC)
    if ("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 17 2022")
        if ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "Win32")
            set(VCREDIST_ARCH x86)
        else()
            set(VCREDIST_ARCH x64)
        endif()
        set(VCREDIST_FILE "vc_redist.${VCREDIST_ARCH}.exe")
        set(VCREDIST_PATH "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Redist/MSVC/v143/${VCREDIST_FILE}")
    else()
        message(FATAL_ERROR "Unknown Visual Studio Version")
    endif()
    if (NOT EXISTS "${VCREDIST_PATH}")
        message(FATAL_ERROR "File does not exist: ${VCREDIST_PATH}")
    endif()
    # Update path to use Windows path separators
    string(REPLACE "/" "\\" VCREDIST_PATH "${VCREDIST_PATH}")
endif()

# -------------------------------------------------------------------
# CPack variables not specific to any single generator
# -------------------------------------------------------------------

# Override default of "${PROJECT_NAME} ${PROJECT_VERSION}" which leads
# to odd installation paths on windows like "C:/Program Files/UnitsCalculator 0.1.0".
# On Windows, the version typically doesn't appear in the Program Files folder.
set(CPACK_PACKAGE_INSTALL_DIRECTORY "UnitsCalculator")

# -------------------------------------------------------------------
# CPack variables for WIX generator. Supports component-based
# installation by default.
# -------------------------------------------------------------------
set(CPACK_WIX_UPGRADE_GUID 13F8BED2-9972-40B1-A84D-49B5BAB7CF65)

# -------------------------------------------------------------------
# CPack variables for NSIS generator. Supports component-based
# installation by default. Add commands to bundle and execute the
# VCRedist installer
# -------------------------------------------------------------------
set(CPACK_NSIS_MANIFEST_DPI_AWARE ON)
set(CPACK_NSIS_MODIFY_PATH ON)

# This instructs the NSIS installer to bundle the VCRedist, and execute it
# during installation. Some observations:
#   - Escaping double-quotes in the commands does not work. This uses
#     single-quotes instead.
#   - Multiple commands can be provided if they are separated by newlines
#     Here, we make the variable a list, then replace the list separator
#     (;) with newlines.
#   - The vcredist installer is passed arguments to install silently and
#     NOT perform an automatic reboot upon completion.
set(CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS      
    "File     \'${VCREDIST_PATH}\'"               
    "ExecWait \'$INSTDIR\\${VCREDIST_FILE} /install /quiet /norestart\'"  
    "Delete   \'$INSTDIR\\${VCREDIST_FILE}\'")   
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS}")

# -------------------------------------------------------------------
# Generate CPack configuration file.
# -------------------------------------------------------------------
include(CPack)

# -------------------------------------------------------------------
# Specify inter-component dependencies and more.
# -------------------------------------------------------------------
cpack_add_component(Development
                    DISPLAY_NAME "Development files"
                    DESCRIPTION "Files required for building aginst the my_units library"
                    DEPENDS Runtime)
cpack_add_component(Runtime
                    DISPLAY_NAME "Applicaiton"
                    DESCRIPTION "The units_calculator application"
                    REQUIRED)