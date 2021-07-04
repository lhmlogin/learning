#ifdef WINDOWS_NT
    #define ODB_EXPORT          __declspec(dllexport)
#else
    #define ODB_EXPORT          __attribute__ ((visibility("default")))
#endif

#define ODB_EXP(name, param) ODB_EXPORT int name param; int name param
#define _ODB_START(id)      ODB_EXP(id, (void))


#define ODB_L_DESC_NULL_VALUE {-1, -1}
typedef struct {         /* used for layer & list */
   int descriptor;
   int run_number;
} odb_obj_desc_struc;

#define _ODB_D_STATIC static
#define ODB_D_LAYER(d) _ODB_D_STATIC odb_obj_desc_struc d = ODB_L_DESC_NULL_VALUE
#define ODB_D_LIST(d)  _ODB_D_STATIC odb_obj_desc_struc d = ODB_L_DESC_NULL_VALUE


_ODB_START(valor_analysis_surf) {

    ODB_D_LIST(layer_list);
    ODB_D_LAYER(layer);
}
