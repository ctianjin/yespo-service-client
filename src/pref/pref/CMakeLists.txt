# --------------------------------------------------------------------
# prefdb_gen
# --------------------------------------------------------------------
SET( MODELGEN_FLAGS -n ns.Pref -p -M -O )
BUILDSYS_ADD_DATABASE( pref pref.schema ${MODELGEN_FLAGS} )
