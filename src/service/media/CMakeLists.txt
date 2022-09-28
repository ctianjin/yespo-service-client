# --------------------------------------------------------------------
# mediadb_gen
# --------------------------------------------------------------------
SET( MODELGEN_FLAGS -n ns.Media -d BUILDSYS_SERVICE_DECL -g serviceglobal.h -p -M -O )
BUILDSYS_ADD_DATABASE( media media.schema ${MODELGEN_FLAGS} )
