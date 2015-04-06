# Packaging configuration

##### Common
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR})
set(CPACK_PACKAGE_CONTACT "Sylvain Joubert <joubert.sy@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Pensieve is a (hopefully) simple todo-list-like application suite")

##### DEB
set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://github.com/sjoubert/pensieve")

include(CPack)
