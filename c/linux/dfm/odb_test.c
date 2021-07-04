#include<stdio.h>
#ifndef FILENAME_MAX
#define FILENAME_MAX 260
#endif


#define ODB_L_PARAM_STRING_LEN  255
#define ODB_L_LEN_NAME           64
typedef char odb_name[ODB_L_LEN_NAME+1];
#define ODB_L_LEN_LAYER_DEF     2047
typedef char odb_layer_def[ODB_L_LEN_LAYER_DEF+1];
typedef char odb_function_def[FILENAME_MAX];
typedef char odb_detch_setup_filename[FILENAME_MAX];

typedef odb_layer_def  odb_param_layer;
typedef int       odb_param_int;
typedef double    odb_param_double;
typedef int       odb_param_p_inch;
typedef int       odb_param_p_mil;
typedef int       odb_param_opt;
typedef char      odb_param_string[ODB_L_PARAM_STRING_LEN+1];
typedef odb_function_def odb_param_function;

#define PIXELS_MIL 10160 /* Pixels in one mil    */
#define PIXELS_MIC 400   /* Pixels in one micron */

#define INT(x) ((x)>=0 ? (int)((x)+0.5) : (int)((x)-0.5))
#define DEG(x) (180.*(x)/M_PI)
#define RAD(x) (M_PI*(x)/180.)

#define I2P(x)   INT((x) * (PIXELS_MIL * 1000.))
#define P2I(x)   ((double)((x) / (PIXELS_MIL * 1000.)))
#define MIL2P(x) INT((x) * (PIXELS_MIL))
#define P2MIL(x) ((double) ((x) / (double)(PIXELS_MIL)))

#define MM2P(x)  INT((x) * (PIXELS_MIC * 1000.))
#define P2MM(x)  ((double)((x) / (PIXELS_MIC * 1000.)))
#define MIC2P(x) INT((x) * (PIXELS_MIC))
#define P2MIC(x) ((double) ((x) / (double)(PIXELS_MIC)))



#define _ODB_PARAM_LAYER(name)  #name,name
#define _ODB_PARAM_INT(name)    #name,&name
#define _ODB_PARAM_DOUBLE(name) #name,&name
#define _ODB_PARAM_P_INCH(name) #name,&name
#define _ODB_PARAM_P_MIL(name)  #name,&name
#define _ODB_PARAM_RADIO(name)  #name,&name
#define _ODB_PARAM_SET(name)    #name,&name
#define _ODB_PARAM_MENU(name)   #name,&name
#define _ODB_PARAM_STRING(name) #name,name
#define _ODB_PARAM_FUNCTION(name) #name,name

typedef struct {
    char *name;
    char *addr;
} odb_prog_params;

typedef struct {
   char *name;
   char *title;
   char *description;
} odb_categories;

#define _ODB_D_STATIC static

typedef	struct {         /* used for layer & list */
   int descriptor;
   int run_number;
} odb_obj_desc_struc;

#define ODB_L_DESC_NULL_VALUE {-1, -1}

#define ODB_D_LAYER(d) _ODB_D_STATIC odb_obj_desc_struc d = ODB_L_DESC_NULL_VALUE
#define ODB_D_LIST(d)  _ODB_D_STATIC odb_obj_desc_struc d = ODB_L_DESC_NULL_VALUE
#define ODB_D_FUNC(d)  _ODB_D_STATIC odb_obj_desc_struc d = ODB_L_DESC_NULL_VALUE
#define ODB_D_SRL(d)   _ODB_D_STATIC odb_obj_desc_struc d = ODB_L_DESC_NULL_VALUE
#define ODB_D_DETCH_SETUP(d)  _ODB_D_STATIC odb_obj_desc_struc d = ODB_L_DESC_NULL_VALUE

#define ODB_D_F(f) \
   _ODB_D_STATIC odb_f_struc _##f = \
   { ODB_MAGIC_F,              \
     ODB_L_NULL_NF,            \
     { 0 },                    \
     ODB_L_DESC_NULL_VALUE, }; \
   _ODB_D_STATIC odb_f_struc *f   = &_##f

#define ODB_D_COMP(comp) \
   _ODB_D_STATIC odb_comp_struc _##comp = \
   { ODB_MAGIC_COMP,              \
     ODB_L_DESC_NULL_VALUE,    \
     ODB_L_NULL_NF,            \
     0  }; \
   _ODB_D_STATIC odb_comp_struc *comp   = &_##comp

#define ODB_D_SHAPE(shape) \
   _ODB_D_STATIC odb_shape_struc _##shape = \
   { ODB_MAGIC_SHAPE ,  \
     ODB_L_DESC_NULL_VALUE, \
     ODB_L_NULL_FCD,    \
     ODB_L_NULL_NFC,    \
     ODB_L_NULL_NET,    \
     TRUE,              \
     ODB_L_NULL_NF,     \
     ODB_L_F_TYPE_NONE, \
     ODB_L_F_SYM_NONE,  \
     0 }; \
   _ODB_D_STATIC odb_shape_struc *shape   = &_##shape



static odb_param_layer pp_layer = ".affected";
static odb_param_p_mil pp_small = MIL2P(5);
static odb_param_opt pp_small_mode = 0x0002;
static odb_param_p_mil pp_max_fold_length = 0;
static odb_param_p_mil pp_small_edge_length = 0;

static odb_prog_params params [] = {
    _ODB_PARAM_LAYER(pp_layer),
    _ODB_PARAM_P_MIL(pp_small),
    _ODB_PARAM_RADIO(pp_small_mode),
    _ODB_PARAM_P_MIL(pp_max_fold_length),
    _ODB_PARAM_P_MIL(pp_small_edge_length)
};

#define NLSE(x)  x
typedef enum {
    C_SELFINTERSECTION_IN_SURFACE = 0,
    C_SELFINTERSECTION_IN_SYMBOL,
    C_PROBLEMATIC_SURFACE,
    C_WRONG_NESTING_ISLAND,
    C_WRONG_NESTING_HOLE,
    C_SMALL_POLY,
    C_SMALL_POLY_SYM,
#ifdef CHECK_INTERSECTING_AND_TANGENT_POLYS
    C_INTERSECTING_POLYS,
    C_INTERSECTING_POLYS_SYM,
    C_TANGENT_POLYS,
    C_TANGENT_POLYS_SYM,
#endif
    C_FOLD,
    C_FOLD_SYM,
    C_SMALL_EDGE,
    C_SMALL_EDGE_SYM,
    C_WRONG_ORDER_OF_HOLE_ISLAND,
    C_CATEGORY_NUM                    /* MUST BE LAST - DEFENITION OF NUMBER OF CATEGORIES */
} category_enum;



static odb_categories categories[] = {
    "selfintersection_in_surface", NLSE("Self-intersection in surface"),  NLSE("surface"),
    "selfintersection_in_symbol",  NLSE("Self-intersection in symbol"),   NLSE("symbol"),
    "problematic_surface",         NLSE("Problematic surface"),           NLSE("surface"),
    "wrong_nesting_island",        NLSE("Wrong nesting for island"),      NLSE("surface"),
    "wrong_nesting_hole",          NLSE("Wrong nesting for hole"),        NLSE("surface"),
    "small_poly",                  NLSE("Small Polygon"),                 NLSE("surface"),
    "small_poly_sym",              NLSE("Small Polygon in symbol"),       NLSE("symbol"),
#ifdef CHECK_INTERSECTING_AND_TANGENT_POLYS
    "intersecting_polys",          NLSE("Intersecting Polygons"),         NLSE("surface"),
    "intersecting_polys_sym",      NLSE("Intersecting Polygons in symbol"), NLSE("symbol"),
    "tangent_polys",               NLSE("Tangent Polygons"),              NLSE("surface"),
    "tangent_polys_sym",           NLSE("Tangent Polygons in symbol"),    NLSE("symbol"),
#endif
    "fold",                        NLSE("Fold"),                          NLSE("surface"),
    "fold_sym",                    NLSE("Fold in symbol"),                NLSE("symbol"),
    "small_edge",                  NLSE("Small edge in surface"),         NLSE("surface"),
    "small_edge_sym",              NLSE("Small edge in symbol"),          NLSE("symbol"),
    "wrong_order_of_hole_island",  NLSE("Wrong order of hole/island"),    NLSE("surface")
};





int main(void) {

    ODB_D_LIST(layer_list);
    ODB_D_LAYER(layer);
    ODB_D_F ( feature );
    ODB_D_SHAPE ( rect );
    ODB_D_LIST( v_types );
    ODB_D_LIST( v_marks );
    ODB_D_LIST( v_values );

    // int a = (int) (50800.5);
    // char (* b)[4];
    // b=&a;
    // printf("%d\n",sizeof(a));
    // printf("%d\n",sizeof(*b));
    // printf("%d\n",a);
    // printf("%d\n",*b);
    // printf("%p\n",&a);
    // printf("%p\n",b);

    printf("%s\n",params[0].name);
    printf("%p\n",params[0].addr);
    printf("%s\n",params[0].addr);
    printf("%s\n",params[1].name);
    // printf("%p\n",params[1].addr);
    int * p;
    p = params[1].addr;

    printf("%d\n",*p);
    printf("%d\n",(int)*params[1].addr);
    printf("%s\n",params[2].name);
    printf("%d\n",*params[2].addr);
    // printf("%s\n",params[2].addr);
}
