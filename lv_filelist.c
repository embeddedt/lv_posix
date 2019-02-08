/**
 * @file lv_filelist.c
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "lv_filelist.h"
#if USE_LV_FILELIST != 0

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_res_t lv_filelist_rel_action(lv_obj_t * filelist);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_signal_func_t ancestor_signal;
static lv_design_func_t ancestor_design;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a filelist object
 * @param par pointer to an object, it will be the parent of the new filelist
 * @param copy pointer to a filelist object, if not NULL then the new object will be copied from it
 * @return pointer to the created filelist
 */
lv_obj_t * lv_filelist_create(lv_obj_t * par, const lv_obj_t * copy)
{
    LV_LOG_TRACE("filelist create started");

    /*Create the ancestor of filelist*/
    lv_obj_t * new_filelist = lv_list_create(par, copy);
    lv_mem_assert(new_filelist);
    if(new_filelist == NULL) return NULL;

    /*Allocate the filelist type specific extended data*/
    lv_filelist_ext_t * ext = lv_obj_allocate_ext_attr(new_filelist, sizeof(lv_filelist_ext_t));
    lv_mem_assert(ext);
    if(ext == NULL) return NULL;
    if(ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_func(new_filelist);
    if(ancestor_design == NULL) ancestor_design = lv_obj_get_design_func(new_filelist);

    /*Initialize the allocated 'ext' */
    memset(ext->current_path, 0, PATH_MAX); /*to be on the safe side*/

    /*Init the new filelist object*/
    if(copy == NULL) {
    	strncpy(ext->current_path, LV_FILELIST_DEFAULT_PATH, PATH_MAX-1);
    }
    /*Copy an existing filelist*/
    else {
        lv_filelist_ext_t * copy_ext = lv_obj_get_ext_attr(copy);

        memcpy(ext->current_path, copy_ext->current_path, PATH_MAX);
        /*Refresh the style with new signal function*/
        lv_obj_refresh_style(new_filelist);
    }

    lv_filelist_update_list(new_filelist);
    LV_LOG_INFO("filelist created");

    return new_filelist;
}

/*=====================
 * Setter functions
 *====================*/


/**
 * Set a style of a filelist.
 * @param filelist pointer to filelist object
 * @param type which style should be set
 * @param style pointer to a style
 */
void lv_filelist_set_style(lv_obj_t * filelist, lv_list_style_t type, lv_style_t * style)
{
    lv_list_set_style(filelist, type, style);
}

/*=====================
 * Getter functions
 *====================*/

/**
 * Get style of a filelist.
 * @param filelist pointer to filelist object
 * @param type which style should be get
 * @return style pointer to the style
 */
lv_style_t * lv_filelist_get_style(const lv_obj_t * filelist, lv_list_style_t type)
{
    return lv_list_get_style(filelist, type);
}

/*=====================
 * Other functions
 *====================*/

static int lv_filelist_filter(const struct dirent * entry)
{
	if(!strcmp(entry->d_name, "."))
		return 0;
	return 1;
}

lv_res_t lv_filelist_update_list(lv_obj_t *filelist) {
	lv_filelist_ext_t * ext = lv_obj_get_ext_attr(filelist);
	struct stat st;
	struct dirent **entries;
	char *tmp_buf;
	const char *symbol;
	int n = 0;

	/*Remove existing items from the list*/
	lv_list_clean(filelist);

	tmp_buf = lv_mem_alloc(PATH_MAX + 1);
	lv_mem_assert(tmp_buf);
	getcwd(tmp_buf, PATH_MAX);

	n = scandir(ext->current_path, &entries, lv_filelist_filter, alphasort);
	/* Keep reading entries from the directory */
	for(int i = 0; i < n; i++) {
		struct dirent * entry = entries[i];
		chdir(ext->current_path);
		stat(entry->d_name, &st);
		if(S_ISDIR(st.st_mode))
			symbol = SYMBOL_DIRECTORY;
		else
			symbol = SYMBOL_FILE;
		if(!strcmp(entry->d_name, "..")) {
			if(!strcmp(ext->current_path, "/")) {
				free(entry);
				continue;
			}
			strcpy(entry->d_name, "Up");
			symbol = SYMBOL_UP;
		}
		lv_list_add(filelist, symbol, entry->d_name, lv_filelist_rel_action);
		free(entry);
	}
	free(entries);
	chdir(tmp_buf);
	lv_mem_free(tmp_buf);
	return LV_RES_OK;
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

static lv_res_t lv_filelist_rel_action(lv_obj_t * listItem) {
	lv_obj_t * fileList = lv_obj_get_parent(lv_obj_get_parent(listItem));
	char * tmp_buf = lv_mem_alloc(PATH_MAX + 1);
	const char * name = lv_list_get_btn_text(listItem);
	const char * symbol = (const char *) lv_img_get_src(lv_list_get_btn_img(listItem));
	lv_filelist_ext_t * ext = lv_obj_get_ext_attr(fileList);


	lv_mem_assert(tmp_buf);

	if(!strcmp(symbol, SYMBOL_UP) && !strcmp(name, "Up"))
		name = "..";

	getcwd(tmp_buf, PATH_MAX);
	chdir(ext->current_path);
	chdir(name);
	getcwd(ext->current_path, PATH_MAX);
	chdir(tmp_buf);
	lv_mem_free(tmp_buf);
	lv_filelist_update_list(fileList);
	return LV_RES_OK;
}

#endif
