/**
 * @file lv_event.h
 *
 */

#ifndef LV_EVENT_H
#define LV_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>
#include <stdint.h>
#include "lv_types.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct _lv_event_dsc_t;

/**
 * Type of event being sent to the object.
 */
typedef enum {
    LV_OBJ_EVENT_ALL = 0,

    /** Input device events*/
    LV_OBJ_EVENT_PRESSED,             /**< The object has been pressed*/
    LV_OBJ_EVENT_PRESSING,            /**< The object is being pressed (called continuously while pressing)*/
    LV_OBJ_EVENT_PRESS_LOST,          /**< The object is still being pressed but slid cursor/finger off of the object */
    LV_OBJ_EVENT_SHORT_CLICKED,       /**< The object was pressed for a short period of time, then released it. Not called if scrolled.*/
    LV_OBJ_EVENT_LONG_PRESSED,        /**< Object has been pressed for at least `long_press_time`.  Not called if scrolled.*/
    LV_OBJ_EVENT_LONG_PRESSED_REPEAT, /**< Called after `long_press_time` in every `long_press_repeat_time` ms.  Not called if scrolled.*/
    LV_OBJ_EVENT_CLICKED,             /**< Called on release if not scrolled (regardless to long press)*/
    LV_OBJ_EVENT_RELEASED,            /**< Called in every cases when the object has been released*/
    LV_OBJ_EVENT_SCROLL_BEGIN,        /**< Scrolling begins. The event parameter is a pointer to the animation of the scroll. Can be modified*/
    LV_OBJ_EVENT_SCROLL_THROW_BEGIN,
    LV_OBJ_EVENT_SCROLL_END,          /**< Scrolling ends*/
    LV_OBJ_EVENT_SCROLL,              /**< Scrolling*/
    LV_OBJ_EVENT_GESTURE,             /**< A gesture is detected. Get the gesture with `lv_indev_get_gesture_dir(lv_indev_get_act());` */
    LV_OBJ_EVENT_KEY,                 /**< A key is sent to the object. Get the key with `lv_indev_get_key(lv_indev_get_act());`*/
    LV_OBJ_EVENT_FOCUSED,             /**< The object is focused*/
    LV_OBJ_EVENT_DEFOCUSED,           /**< The object is defocused*/
    LV_OBJ_EVENT_LEAVE,               /**< The object is defocused but still selected*/
    LV_OBJ_EVENT_HIT_TEST,            /**< Perform advanced hit-testing*/

    /** Drawing events*/
    LV_OBJ_EVENT_COVER_CHECK,        /**< Check if the object fully covers an area. The event parameter is `lv_cover_check_info_t *`.*/
    LV_OBJ_EVENT_REFR_EXT_DRAW_SIZE, /**< Get the required extra draw area around the object (e.g. for shadow). The event parameter is `lv_coord_t *` to store the size.*/
    LV_OBJ_EVENT_DRAW_MAIN_BEGIN,    /**< Starting the main drawing phase*/
    LV_OBJ_EVENT_DRAW_MAIN,          /**< Perform the main drawing*/
    LV_OBJ_EVENT_DRAW_MAIN_END,      /**< Finishing the main drawing phase*/
    LV_OBJ_EVENT_DRAW_POST_BEGIN,    /**< Starting the post draw phase (when all children are drawn)*/
    LV_OBJ_EVENT_DRAW_POST,          /**< Perform the post draw phase (when all children are drawn)*/
    LV_OBJ_EVENT_DRAW_POST_END,      /**< Finishing the post draw phase (when all children are drawn)*/
    LV_OBJ_EVENT_DRAW_PART_BEGIN,    /**< Starting to draw a part. The event parameter is `lv_obj_draw_dsc_t *`. */
    LV_OBJ_EVENT_DRAW_PART_END,      /**< Finishing to draw a part. The event parameter is `lv_obj_draw_dsc_t *`. */

    /** Special events*/
    LV_OBJ_EVENT_VALUE_CHANGED,       /**< The object's value has changed (i.e. slider moved)*/
    LV_OBJ_EVENT_INSERT,              /**< A text is inserted to the object. The event data is `char *` being inserted.*/
    LV_OBJ_EVENT_REFRESH,             /**< Notify the object to refresh something on it (for the user)*/
    LV_OBJ_EVENT_READY,               /**< A process has finished*/
    LV_OBJ_EVENT_CANCEL,              /**< A process has been cancelled */

    /** Other events*/
    LV_OBJ_EVENT_DELETE,              /**< Object is being deleted*/
    LV_OBJ_EVENT_CHILD_CHANGED,       /**< Child was removed, added, or its size, position were changed */
    LV_OBJ_EVENT_CHILD_CREATED,       /**< Child was created, always bubbles up to all parents*/
    LV_OBJ_EVENT_CHILD_DELETED,       /**< Child was deleted, always bubbles up to all parents*/
    LV_OBJ_EVENT_SCREEN_UNLOAD_START, /**< A screen unload started, fired immediately when scr_load is called*/
    LV_OBJ_EVENT_SCREEN_LOAD_START,   /**< A screen load started, fired when the screen change delay is expired*/
    LV_OBJ_EVENT_SCREEN_LOADED,       /**< A screen was loaded*/
    LV_OBJ_EVENT_SCREEN_UNLOADED,     /**< A screen was unloaded*/
    LV_OBJ_EVENT_SIZE_CHANGED,        /**< Object coordinates/size have changed*/
    LV_OBJ_EVENT_STYLE_CHANGED,       /**< Object's style has changed*/
    LV_OBJ_EVENT_LAYOUT_CHANGED,      /**< The children position has changed due to a layout recalculation*/
    LV_OBJ_EVENT_GET_SELF_SIZE,       /**< Get the internal size of a widget*/

    /** Events of optional LVGL components*/
#if LV_USE_MSG
    LV_OBJ_EVENT_MSG_RECEIVED,
#endif

    LV_DISP_EVENT_INVALIDATE_AREA,
    LV_DISP_EVENT_RENDER_START,
    LV_DISP_EVENT_RENDER_READY,
    LV_DISP_EVENT_RESOLUTION_CHANGED,

    _LV_OBJ_EVENT_LAST,               /** Number of default events*/


    LV_OBJ_EVENT_PREPROCESS = 0x80,   /** This is a flag that can be set with an event so it's processed
                                      before the class default event processing */
} lv_event_code_t;


typedef struct {
    struct _lv_event_dsc_t * dsc;
    uint32_t cnt;
} lv_event_list_t;

typedef struct _lv_event_t {
    void * target;
    void * current_target;
    lv_event_code_t code;
    void * user_data;
    void * param;
    struct _lv_event_t * prev;
    uint8_t deleted : 1;
    uint8_t stop_processing : 1;
    uint8_t stop_bubbling : 1;
} lv_event_t;

/**
 * @brief Event callback.
 * Events are used to notify the user of some action being taken on the object.
 * For details, see ::lv_event_t.
 */
typedef void (*lv_event_cb_t)(lv_event_t * e);

/**********************
 * GLOBAL PROTOTYPES
 **********************/


lv_res_t lv_event_send(lv_event_list_t * list, lv_event_t * e, bool prerpocess);

void lv_event_add_callback(lv_event_list_t * list, lv_event_cb_t cb, lv_event_code_t filter, void * user_data);

bool lv_event_remove_callback(lv_event_list_t * list, lv_event_cb_t cb);

bool lv_event_remove_callback_with_user_data(lv_event_list_t * list, lv_event_cb_t cb, const void * user_data);

void * lv_event_get_user_data_of_callback(lv_event_list_t * list, lv_event_cb_t event_cb);


/**
 * Get the object originally targeted by the event. It's the same even if the event is bubbled.
 * @param e     pointer to the event descriptor
 * @return      the target of the event_code
 */
void * lv_event_get_target(lv_event_t * e);

/**
 * Get the current target of the event. It's the object which event handler being called.
 * If the event is not bubbled it's the same as "normal" target.
 * @param e     pointer to the event descriptor
 * @return      pointer to the current target of the event_code
 */
void * lv_event_get_current_target(lv_event_t * e);

/**
 * Get the event code of an event
 * @param e     pointer to the event descriptor
 * @return      the event code. (E.g. `LV_OBJ_EVENT_CLICKED`, `LV_OBJ_EVENT_FOCUSED`, etc)
 */
lv_event_code_t lv_event_get_code(lv_event_t * e);

/**
 * Get the parameter passed when the event was sent
 * @param e     pointer to the event descriptor
 * @return      pointer to the parameter
 */
void * lv_event_get_param(lv_event_t * e);

/**
 * Get the user_data passed when the event was registered on the object
 * @param e     pointer to the event descriptor
 * @return      pointer to the user_data
 */
void * lv_event_get_user_data(lv_event_t * e);

/**
 * Stop the event from bubbling.
 * This is only valid when called in the middle of an event processing chain.
 * @param e     pointer to the event descriptor
 */
void lv_event_stop_bubbling(lv_event_t * e);

/**
 * Stop processing this event.
 * This is only valid when called in the middle of an event processing chain.
 * @param e     pointer to the event descriptor
 */
void lv_event_stop_processing(lv_event_t * e);

/**
 * Register a new, custom event ID.
 * It can be used the same way as e.g. `LV_OBJ_EVENT_CLICKED` to send custom events
 * @return      the new event id
 * @example
 * uint32_t LV_OBJ_EVENT_MINE = 0;
 * ...
 * e = lv_event_register_id();
 * ...
 * lv_obj_send_event(obj, LV_OBJ_EVENT_MINE, &some_data);
 */
uint32_t lv_event_register_id(void);

/**
 * Nested events can be called and one of them might belong to an object that is being deleted.
 * Mark this object's `event_temp_data` deleted to know that its `lv_obj_send_event` should return `LV_RES_INV`
 * @param target     pointer to an event target which was deleted
 */
void _lv_event_mark_deleted(void * target);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_EVENT_H*/