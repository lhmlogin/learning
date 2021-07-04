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


#define NLSE(x)  x

typedef struct {
    char *name;
    char *addr;
} odb_prog_params;

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


#define ODB_L_DESC_NULL_VALUE {-1, -1}
typedef struct {         /* used for layer & list */
   int descriptor;
   int run_number;
} odb_obj_desc_struc;

#define _ODB_D_STATIC static
#define ODB_D_LAYER(d) _ODB_D_STATIC odb_obj_desc_struc d = ODB_L_DESC_NULL_VALUE
#define ODB_D_LIST(d)  _ODB_D_STATIC odb_obj_desc_struc d = ODB_L_DESC_NULL_VALUE
#define LIC_NAME "analyze"






#include "enums.h"  //define ODB_ACT_EDT_OPEN_LIC

typedef enum _odb_act odb_act_enum;

enum _odb_ptype
   { ODB_PTYPE_S = 100,
     ODB_PTYPE_I,
     ODB_PTYPE_SYM,
     ODB_PTYPE_D,
     ODB_PTYPE_SP,
     ODB_PTYPE_IP,
     ODB_PTYPE_SYM_P,
     ODB_PTYPE_DP,
     ODB_PTYPE_SHAPE_P,
     ODB_PTYPE_F_P,
     ODB_PTYPE_CATEGORIES_P,
     ODB_PTYPE_PROG_PARAMS_P,
     ODB_PTYPE_REL_P,
     ODB_PTYPE_CAP,
     ODB_PTYPE_OBJ,
     ODB_PTYPE_OBJ_P,
     ODB_PTYPE_VOID_P,
     ODB_PTYPE_COMP_P
};

typedef enum _odb_ptype odb_ptype_enum;

typedef int odb_sym_t;

enum _odb_boolean
   { ODB_FALSE = 0,
     ODB_TRUE  = 1 };

typedef u_enum _odb_boolean boolean;

enum _odb_f_type
   { ODB_L_F_TYPE_LINE,
     ODB_L_F_TYPE_PAD,
     ODB_L_F_TYPE_SURFACE,
     ODB_L_F_TYPE_ARC,
     ODB_L_F_TYPE_TEXT,
     ODB_L_F_TYPE_NONE = 15};

typedef u_enum _odb_f_type odb_f_type_enum;

enum _odb_f_sym
   { ODB_L_F_SYM_ROUND,
     ODB_L_F_SYM_SQUARE,
     ODB_L_F_SYM_SPECIAL,
     ODB_L_F_SYM_NONE = 7 };

typedef u_enum _odb_f_sym odb_f_sym_enum;







enum _odb_shape_type
   { ODB_L_SHAPE_TYPE_NONE   =   0,
     ODB_L_SHAPE_TYPE_POINT  =   1,
     ODB_L_SHAPE_TYPE_SEG    =   2,
     ODB_L_SHAPE_TYPE_CURVE  =   4,
     ODB_L_SHAPE_TYPE_LINE   =   8,
     ODB_L_SHAPE_TYPE_CIRCLE =  16,
     ODB_L_SHAPE_TYPE_SQUARE =  32,
     ODB_L_SHAPE_TYPE_RECT   =  64,
     ODB_L_SHAPE_TYPE_ARC    = 128,
     ODB_L_SHAPE_TYPE_POLY   = 256,
     ODB_L_SHAPE_TYPE_CONT   = 512 };

typedef u_enum _odb_shape_type odb_shape_type_enum;


enum _odb_rel
   { ODB_L_DISJOINT,
     ODB_L_1_IN_2,
     ODB_L_2_IN_1,
     ODB_L_INTERSECT,
     ODB_L_EQUAL };

typedef u_enum _odb_rel odb_rel_enum;


/*---------------- FEATURE STRUCTURES ---------------*/

typedef struct {
   int x;
   int y;
} odb_point_struc;

typedef struct {
   odb_point_struc ps;
   odb_point_struc pe;
} _odb_f_line_struc;

typedef struct {
   odb_point_struc pc;
} _odb_f_pad_struc;

typedef struct {
   void *cont;
} _odb_f_surface_struc;

typedef struct {
   odb_point_struc ps;
   odb_point_struc pe;
   odb_point_struc pc;
} _odb_f_arc_struc;

typedef struct {
   odb_point_struc pos;
   char           *str;
   void*          fntid;
   union {
      struct {
     int   xsize, ysize;
     float w_factor;
      } string;
      struct {
     int width;
     int height;
     unsigned int type     :3; /* odb_bar_type_enum     */
     unsigned int fasc     :1; /* odb_bar_asc_enum      */
     unsigned int cs       :1; /* odb_bar_cs_enum       */
     unsigned int bg       :1; /* odb_bar_bg_enum       */
     unsigned int astr     :1; /* odb_bar_astr_enum     */
     unsigned int astr_pos :1; /* odb_bar_astr_pos_enum */
     unsigned int spare    :24;
      } bar;
   } u;
} _odb_f_text_struc;

typedef struct {
   int                magic;
   int                nf;
   union {
      odb_sym_t       sym;
      intptr_t        lyrid;
   } usym; 
   odb_obj_desc_struc ld;
   union {
      _odb_f_line_struc    line;
      _odb_f_pad_struc     pad;
      _odb_f_surface_struc surface;
      _odb_f_arc_struc     arc;
      _odb_f_text_struc    text;
   } f;
   double rotation;   /* rotation in radians (valid when angle = ODB_L_ANGLE_ANY ) */
   struct {
      unsigned int type     :4; /* odb_f_type_enum */
      unsigned int sym_type :3; /* odb_f_sym_enum */
      unsigned int polarity :1; /* odb_pol_enum */
      unsigned int angle    :3; /* odb_angle_enum. angle_idx: 0-0, 1-90, 2-180, 3-270, 7 - any angle rotation  */
      unsigned int mirror   :1; /* odb_mirror_enum */
      unsigned int cw       :1; /* odb_cw_ccw_enum */
      unsigned int spare1   :1;
      unsigned int select   :1; /* boolean */
      unsigned int drill    :1; /* boolean */
      unsigned int attr     :1; /* boolean */
      unsigned int bar      :1; /* odb_text_str_bar_enum */
      unsigned int dcode    :13;
      unsigned int text_ver :1; /* odb_text_ver_enum */
   } flags;
} odb_f_struc;

/*--------------- SHAPE STRUCTURES ----------------*/

typedef struct {
   odb_point_struc min;
   odb_point_struc max;
} odb_limits_struc;

typedef struct {
   odb_point_struc ps, pe;              /* Start & End points   */
} _odb_seg_struc;

typedef struct {
   odb_point_struc ps, pe;              /* Start & End points   */
   odb_point_struc pc;                  /* Center of circle */
   struct {
      unsigned int cw        : 1;       /* odb_cw_ccw_enum */
      unsigned int convexity : 1;       /* odb_convexity_enum */
      unsigned int spare     : 30;
   } flags;
} _odb_curve_struc;

typedef struct {
   _odb_seg_struc  seg;                 /* segment (skeleton)   */
   struct {
      unsigned int cap        :  1;     /* odb_cap_enum     */
      unsigned int r          : 31;     /* Radius (Half width)  */
   } w;
} _odb_line_struc;

typedef struct {
   odb_point_struc  pc;         /* Center              */
   int          r;          /* Radius              */
} _odb_circle_struc;

typedef struct {
   odb_point_struc  pc;         /* Center              */
   int          r;      /* Radius (Half width) */
} _odb_square_struc;

typedef struct {
   odb_point_struc  pll;        /* Lower left corner   */
   int         w, h;        /* Width & Height      */
} _odb_rect_struc;

typedef struct {
   _odb_curve_struc  c;     /* skeleton            */
   struct {
     unsigned int   cap : 1;   /* odb_cap_enum     */
     unsigned int   r   :31;   /* Radius (Half width) */
   } w;
} _odb_arc_struc;

typedef struct {
   int  xff;
   int  yff;
   odb_limits_struc limits;    /* bounding limits of the contour/polygon */
} _odb_cont_lim_struc;

typedef struct {
   void              *cont;    /* For internal use */
   odb_obj_desc_struc obj;     /* For internal use */
} _odb_cont;






typedef struct {
   odb_shape_type_enum type;
   union {
       odb_point_struc  point;
      _odb_seg_struc    seg;
      _odb_curve_struc  curve;
      _odb_line_struc   line;
      _odb_circle_struc circle;
      _odb_square_struc square;
      _odb_rect_struc   rect;
      _odb_arc_struc    arc;
      _odb_cont         cont;
   } s;
} odb_mat_shape_struc;




typedef struct {
   int                 magic;
   odb_obj_desc_struc  ld;
   void*           fcd;
   int                 nfc;
   int                 net;
   enum _odb_boolean   single;
   int                 nf;
   odb_f_type_enum     f_type;
   odb_f_sym_enum      f_sym_type;
   odb_sym_t           f_sym;
   odb_mat_shape_struc mshape;
} odb_shape_struc;


enum _odb_cap
   { ODB_L_CAP_ROUND,
     ODB_L_CAP_SQUARE };

typedef u_enum _odb_cap odb_cap_enum;


typedef struct {
   char *name;
   char *title;
   char *description;
} odb_categories;

typedef struct {
   int          magic;
   odb_obj_desc_struc   ld;
   int          ncomp ;
   odb_name     name ;
   odb_name     part_name ;
   odb_name     pkg_name ;
   odb_point_struc  position ;
   double       rotation ;
   int          height ;
   int          pitch ;
   boolean      mirror ;
   int          num_toeprints ;
   int          spare[20];
} odb_comp_struc ;


typedef struct {
   odb_ptype_enum         type;
   union {
      char               *s;
      int                 i;
      odb_sym_t           sym;
      double              d;
      char              **sp;
      int                *ip;
      odb_sym_t      *sym_p;
      double             *dp;
      odb_shape_struc    *shape_p;
      odb_f_struc        *f_p;
      odb_rel_enum       *rel_p;
      odb_cap_enum        cap;
      odb_categories     *categories_p;
      odb_prog_params    *prog_params_p;
      odb_obj_desc_struc  obj;
      odb_obj_desc_struc *obj_p;
      void               *void_p;
      odb_comp_struc     *comp_p;
   } v;
} odb_param_struc;



typedef struct {
   odb_act_enum action;
   char        *asc_action;
   struct {
      void     *run_data;
      int       edt_num;
      char     *fname;
      int   line;
   } env;
   int num_params;
   odb_param_struc params[ODB_MAX_PARAMS];
} odb_params_struc;

static odb_params_struc odb_params;

#define ODB_NUM_PARAMS odb_params.num_params

#define ODB_PARAMS_INIT(_action)     \
 { odb_params.action     = _action;  \
   odb_params.asc_action = #_action; \
   odb_params.env.fname  = __FILE__; \
   odb_params.env.line   = __LINE__; \
   odb_params.num_params = 0; }

#define ODB_PARAM_SET_S(_pvalue)                          \
 { odb_params.params[ODB_NUM_PARAMS].type = ODB_PTYPE_S;  \
   odb_params.params[ODB_NUM_PARAMS].v.s  = _pvalue;      \
   ODB_NUM_PARAMS++; }

#define ODB_PARAM_SET_IP(_pvalue)                          \
 { odb_params.params[ODB_NUM_PARAMS].type = ODB_PTYPE_IP;  \
   odb_params.params[ODB_NUM_PARAMS].v.ip = &(_pvalue);    \
   ODB_NUM_PARAMS++; }

extern int odb__call_do(odb_params_struc *_odb_params);
extern void odb__exec_error(int odb_status, int line);
#define ODB_L_STOPPED (-1)
static boolean          odb_ignore_one_status=FALSE ;

#define ODB_CALL                              \
 { odb_status = odb__call_do(&odb_params);    \
   if (odb_status == ODB_L_STOPPED ||         \
      (odb_status != 0 && !odb_ignore_one_status)){ \
      odb__exec_error(odb_status, __LINE__);  \
      return(odb_status); } \
   odb_ignore_one_status = FALSE ; }

#define ODB_EDT_OPEN_LIC(_license, _success)   \
 { ODB_PARAMS_INIT(ODB_ACT_EDT_OPEN_LIC);      \
   ODB_PARAM_SET_S (_license);                 \
   ODB_PARAM_SET_IP(_success);                 \
   ODB_CALL; }


#define ODB_EDT_MESSAGE(_message)              \
 { ODB_PARAMS_INIT(ODB_ACT_EDT_MESSAGE);       \
   ODB_PARAM_SET_S (_message);                 \
   ODB_CALL; }

#define ODB_UTILS_UNITS(_units)                  \
 { ODB_PARAMS_INIT(ODB_ACT_UTILS_UNITS);         \
   ODB_PARAM_SET_IP(_units);                     \
   ODB_CALL; }

#define ODB_L_OK       0          /* allow error handling */
#define ODB_L_STOPPED (-1)
#define ODB_L_ERR      1
#define ODB_RETURN_OK      return(ODB_L_OK);
#define ODB_RETURN_STOPPED return(ODB_L_STOPPED);
#define ODB_RETURN_ERR    \
   { odb__exec_error(ODB_L_ERR,__LINE__); return(ODB_L_ERR);}





int main(void) {
    // int a = (int) (50800.5);
    // char (* b)[4];
    // b=&a;
    // printf("%d\n",sizeof(a));
    // printf("%d\n",sizeof(*b));
    // printf("%d\n",a);
    // printf("%d\n",*b);
    // printf("%p\n",&a);
    // printf("%p\n",b);
    // printf("%s\n",params[0].name);
    // printf("%p\n",params[0].addr);
    // printf("%s\n",params[0].addr);
    // printf("%s\n",params[1].name);
    // // printf("%p\n",params[1].addr);
    // int * p;
    // p = params[1].addr;

    // printf("%d\n",*p);
    // printf("%d\n",(int)*params[1].addr);
    // printf("%s\n",params[2].name);
    // printf("%d\n",*params[2].addr);
    // printf("%s\n",params[2].addr);

    ODB_D_LIST(layer_list);
    ODB_D_LAYER(layer);
    int exists;
    int units;

    ODB_EDT_OPEN_LIC(LIC_NAME, exists);
    if (!exists) {
        ODB_EDT_MESSAGE("Could not get license");
        ODB_RETURN_ERR; 
    }

    ODB_UTILS_UNITS(units);

    /*get the setup parameters*/
    ODB_SETUP_GET_PARAMS(params);

    /*create the categories array*/
    ODB_RES_ADD_CATEGORIES(categories);

    /*Get list of layers */
    ODB_LAYER_NAME_TO_LIST( pp_layer , layer_list);
}
