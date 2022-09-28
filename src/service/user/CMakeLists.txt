# --------------------------------------------------------------------
# userdb_gen
# --------------------------------------------------------------------
SET( MODELGEN_FLAGS -n ns.User -d BUILDSYS_SERVICE_DECL -g serviceglobal.h -p -M -O )
BUILDSYS_ADD_DATABASE( user user.schema ${MODELGEN_FLAGS} )
