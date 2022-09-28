# --------------------------------------------------------------------
# Boost
# --------------------------------------------------------------------
IF( NOT Boost_FOUND )
    MESSAGE( WARNING "Boost library is NOT found, so that modelgen will not be built!" )
    RETURN()
ENDIF( NOT Boost_FOUND )

# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------
BUILDSYS_ENABLE_EXCEPTIONS( TRUE )
BUILDSYS_ENABLE_RTTI( TRUE )

# Use version 2 of Boost.Filesystem
ADD_DEFINITIONS( -DBOOST_FILESYSTEM_VERSION=3 )
ADD_DEFINITIONS( -DBOOST_FILESYSTEM_NO_DEPRECATED )

# --------------------------------------------------------------------
# modelgen
# --------------------------------------------------------------------
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

SET(
    modelgen_SOURCES
    input.cpp
    main.cpp
    model.cpp
    output.cpp
    output_db.cpp
    output_db_base.cpp
    output_db_model.cpp
    output_entity.cpp
    output_enum.cpp
    output_dao.cpp
    output_dao_base.cpp
    output_model.cpp
    output_status.cpp
    output_struct.cpp
    output_tree_model.cpp
    output_value.cpp
)

ADD_EXECUTABLE( modelgen ${modelgen_SOURCES} )
TARGET_LINK_LIBRARIES( modelgen ${Boost_LIBRARIES} )
SET_TARGET_PROPERTIES( modelgen PROPERTIES AUTOMOC FALSE )
