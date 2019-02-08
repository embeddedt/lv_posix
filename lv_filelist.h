/**
 * @file lv_filelist.h
 *
 */


/* TODO Remove these instructions
 * Search an replace: filelist -> object normal name with lower case (e.g. button, label etc.)
 *                    filelist -> object short name with lower case(e.g. btn, label etc)
 *                    FILELIST -> object short name with upper case (e.g. BTN, LABEL etc.)
 *
 */

#ifndef LV_FILELIST_H
#define LV_FILELIST_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#else
#include "../lv_conf.h"
#endif

#if USE_LV_FILELIST != 0

/*Testing of dependencies*/
#if USE_LV_LIST == 0
#error "lv_filelist: lv_list is required. Enable it in lv_conf.h (USE_LV_LIST  1) "
#endif

/*Default configuration*/
#ifndef LV_FILELIST_DEFAULT_PATH
#define LV_FILELIST_DEFAULT_PATH "/"
#endif

#include "lvgl/lv_core/lv_obj.h"
#include "lvgl/lv_objx/lv_list.h"
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/*Data of filelist*/
typedef struct {
    lv_list_ext_t list; /*Ext. of ancestor*/
    /*New data for this type */
    char current_path[PATH_MAX + 1];
} lv_filelist_ext_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a filelist objects
 * @param par pointer to an object, it will be the parent of the new filelist
 * @param copy pointer to a filelist object, if not NULL then the new object will be copied from it
 * @return pointer to the created filelist
 */
lv_obj_t * lv_filelist_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
 * Add/remove functions
 *=====================*/


/*=====================
 * Setter functions
 *====================*/

/**
 * Set a style of a filelist.
 * @param filelist pointer to filelist object
 * @param type which style should be set
 * @param style pointer to a style
 */
void lv_filelist_set_style(lv_obj_t * filelist, lv_list_style_t type, lv_style_t *style);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get style of a filelist.
 * @param filelist pointer to filelist object
 * @param type which style should be get
 * @return style pointer to the style
 */
lv_style_t * lv_filelist_get_style(const lv_obj_t * filelist, lv_list_style_t type);

/*=====================
 * Other functions
 *====================*/

lv_res_t lv_filelist_update_list(lv_obj_t *filelist);

/**********************
 *      MACROS
 **********************/

#endif  /*USE_LV_FILELIST*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /*LV_FILELIST_H*/
