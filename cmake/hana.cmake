include(ExternalProject)

find_program(Git git REQUIRED)

set(BoostHanaVer v1.6.0)

ExternalProject_Add(
  get_boost_hana
  PREFIX thirdparties
  GIT_REPOSITORY https://github.com/boostorg/hana
  GIT_TAG ${BoostHanaVer}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  )

add_library(boost_hana INTERFACE)
add_library(boost::hana ALIAS boost_hana)

ExternalProject_Get_Property(get_boost_hana SOURCE_DIR)
set_target_properties(boost_hana PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)
set_target_properties(boost_hana PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)

add_dependencies(boost_hana get_boost_hana)
