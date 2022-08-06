# CMake Distribution

This project demonstrates building a distributable project
with CMake and CPack. The project is a cross-platform shared library and application for converting between physical units. It demonstrates:

1. Specifying installation files as belonging to either "Runtime" or "Development" components, which are selectable at install time. This project specifies that the "Runtime" component is required, and that it is a dependency of the "Development" component.

1. Bunding the Visual Studio Redistributable package with the NSIS installer, so that it's invoked as part of this project's installation.

1. Configuring CPack Debian generator to create a unique package for each component.
