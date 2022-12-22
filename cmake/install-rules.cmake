if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/open62541cpp-${PROJECT_VERSION}"
      CACHE PATH ""
  )
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package open62541cpp)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT open62541cpp_Development
)

install(
    TARGETS open62541cpp_open62541cpp
    EXPORT open62541cppTargets
    RUNTIME #
    COMPONENT open62541cpp_Runtime
    LIBRARY #
    COMPONENT open62541cpp_Runtime
    NAMELINK_COMPONENT open62541cpp_Development
    ARCHIVE #
    COMPONENT open62541cpp_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    open62541cpp_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(open62541cpp_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${open62541cpp_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT open62541cpp_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${open62541cpp_INSTALL_CMAKEDIR}"
    COMPONENT open62541cpp_Development
)

install(
    EXPORT open62541cppTargets
    NAMESPACE open62541cpp::
    DESTINATION "${open62541cpp_INSTALL_CMAKEDIR}"
    COMPONENT open62541cpp_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
