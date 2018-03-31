/**	
 * \file            gui_dialog.c
 * \brief           Dialog widget
 */
 
/*
 * Copyright (c) 2017 Tilen Majerle
 *  
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author:          Tilen Majerle <tilen@majerle.eu>
 */
#define GUI_INTERNAL
#include "gui/gui_private.h"
#include "widget/gui_dialog.h"

typedef struct {
    gui_linkedlist_t list;                          /*!< Linked list entry element, must always be first on list */
    gui_id_t id;                                    /*!< Dialog ID */
    gui_handle_p h;                                 /*!< Pointer to dialog address */
    volatile int status;                            /*!< Status on dismissed call */
#if GUI_CFG_OS
    gui_sys_sem_t sem;                              /*!< Semaphore handle for blocking */
    uint8_t ib;                                     /*!< Indication if dialog is blocking */
#endif /* GUI_CFG_OS */
} dissmissed_dialog_list_t;

#define __GW(x)             ((GUI_WINDOW_t *)(x))

static uint8_t gui_dialog_callback(gui_handle_p h, GUI_WC_t ctrl, gui_widget_param_t* param, gui_widget_result_t* result);

/**
 * \brief           List of dismissed dialog elements not yet read
 */
static
gui_linkedlistroot_t ddlist;

/**
 * \brief           Widget initialization structure
 */
static const
gui_widget_t widget = {
    .Name = _GT("DIALOG"),                          /*!< Widget name */
    .Size = sizeof(GUI_DIALOG_t),                   /*!< Size of widget for memory allocation */
    .Flags = GUI_FLAG_WIDGET_ALLOW_CHILDREN | GUI_FLAG_WIDGET_DIALOG_BASE,  /*!< List of widget flags */
    .Callback = gui_dialog_callback,                /*!< Control function */
    .Colors = NULL,                                 /*!< Pointer to colors array */
    .ColorsCount = 0,                               /*!< Number of colors */
};


/* Add widget to active dialogs (not yet dismissed) */
static dissmissed_dialog_list_t *
add_to_active_dialogs(gui_handle_p h) {
    dissmissed_dialog_list_t* l;
    
    l = GUI_MEMALLOC(sizeof(*l));                   /* Allocate memory for dismissed dialog list */
    if (l != NULL) {
        l->h = h;
        l->id = guii_widget_getid(h);
        gui_linkedlist_add_gen(&ddlist, &l->list);  /* Add entry to linked list */
    }
    return l;
}

/* Remove and free memory from linked list */
static void
remove_from_active_dialogs(dissmissed_dialog_list_t* l) {
    gui_linkedlist_remove_gen(&ddlist, &l->list);   /* Remove entry from linked list first */
    GUI_MEMFREE(l);                                 /* Free memory */
}

/* Get entry from linked list for specific dialog */
static dissmissed_dialog_list_t *
get_dialog(gui_handle_p h) {
    dissmissed_dialog_list_t* l = NULL;
    gui_id_t id;
    
    id = guii_widget_getid(h);                     /* Get id of widget */
    
    for (l = (dissmissed_dialog_list_t *)gui_linkedlist_getnext_gen((gui_linkedlistroot_t *)&ddlist, NULL); l != NULL;
        l = (dissmissed_dialog_list_t *)gui_linkedlist_getnext_gen(NULL, (gui_linkedlist_t *)l)) {
        if (l->h == h && l->id == id) {             /* Check match for handle and id */
            break;
        }
    }
    return l;
}

/**
 * \brief           Default widget callback function
 * \param[in]       h: Widget handle
 * \param[in]       ctr: Callback type
 * \param[in]       param: Input parameters for callback type
 * \param[out]      result: Result for callback type
 * \return          1 if command processed, 0 otherwise
 */
static uint8_t
gui_dialog_callback(gui_handle_p h, GUI_WC_t ctrl, gui_widget_param_t* param, gui_widget_result_t* result) {
    __GUI_ASSERTPARAMS(h != NULL && h->Widget == &widget);  /* Check input parameters */
    switch (ctrl) {                                 /* Handle control function if required */
        default:                                    /* Handle default option */
            GUI_UNUSED3(h, param, result);          /* Unused elements to prevent compiler warnings */
            return 0;                               /* Command was not processed */
    }
}

/**
 * \brief           Create new dialog base element without any "design" style
 * \param[in]       id: Widget unique ID to use for identity for callback processing
 * \param[in]       x: Widget X position relative to parent widget
 * \param[in]       y: Widget Y position relative to parent widget
 * \param[in]       width: Widget width in units of pixels
 * \param[in]       height: Widget height in uints of pixels
 * \param[in]       func: Widget create function used as dialog base. In most cases \ref gui_container_create will be used to create empty container
 * \param[in]       cb: Pointer to \ref GUI_WIDGET_CALLBACK_t callback function. Set to NULL to use default widget callback
 * \param[in]       flags: Flags for widget creation
 * \return          \ref gui_handle_p object of created widget on success, NULL otherwise
 */
gui_handle_p
gui_dialog_create(gui_id_t id, float x, float y, float width, float height, GUI_WIDGET_CreateFunc_t func, GUI_WIDGET_CALLBACK_t cb, uint16_t flags) {
    gui_handle_p ptr;
    if (func == NULL) {                             /* Check create function */
        return NULL;
    }
    
    ptr = func(id, x, y, width, height, NULL, cb, flags | GUI_FLAG_WIDGET_CREATE_PARENT_DESKTOP);   /* Create desired widget */
    if (ptr != NULL) {
        __GUI_ENTER();                              /* Enter GUI */
        guii_widget_setflag(ptr, GUI_FLAG_WIDGET_DIALOG_BASE); /* Add dialog base flag to widget */
        gui_linkedlist_widgetmovetobottom(ptr);     /* Move to bottom on linked list make it on top now with flag set as dialog */
        add_to_active_dialogs(ptr);                 /* Add this dialog to active dialogs */
        __GUI_LEAVE();                              /* Leave GUI */
    }
    
    return (gui_handle_p)ptr;
}

#if GUI_CFG_OS || __DOXYGEN__
/**
 * \brief           Create new dialog base element without any "design" style and wait for dismiss status
 * \note            Function will block thread until dialog is dismissed using \ref gui_dialog_dismiss function by user
 *
 * \param[in]       id: Widget unique ID to use for identity for callback processing
 * \param[in]       x: Widget X position relative to parent widget
 * \param[in]       y: Widget Y position relative to parent widget
 * \param[in]       width: Widget width in units of pixels
 * \param[in]       height: Widget height in uints of pixels
 * \param[in]       func: Widget create function used as dialog base. In most cases \ref gui_container_create will be used to create empty container
 * \param[in]       cb: Pointer to \ref GUI_WIDGET_CALLBACK_t callback function. Set to NULL to use default widget callback
 * \param[in]       flags: Flags for widget creation
 * \return          Value passed to \ref gui_dialog_dismiss when dialog is dismissed on success, -1 otherwise
 */
int
gui_dialog_createblocking(gui_id_t id, gui_idim_t x, gui_idim_t y, gui_dim_t width, gui_dim_t height, GUI_WIDGET_CreateFunc_t func, GUI_WIDGET_CALLBACK_t cb, uint16_t flags) {
    gui_handle_p ptr;
    int resp = -1;                                  /* Dialog not created error */
    
    ptr = gui_dialog_create(id, x, y, width, height, func, cb, flags);  /* Create dialog first */
    if (ptr != NULL) {                              /* Widget created */
        dissmissed_dialog_list_t* l;
        
        __GUI_ENTER();                              /* Enter GUI */
        l = get_dialog(ptr);                        /* Get entry from active dialogs */
        if (l != NULL) {                            /* Check if successfully added widget to active dialogs */
            l->ib = 1;                              /* Blocking entry */
            if (gui_sys_sem_create(&l->sem, 0)) {   /* Create semaphore and lock it immediatelly */
                __GUI_SYS_UNPROTECT();              /* Disable protection while waiting for semaphore */
                gui_sys_sem_wait(&l->sem, 0);       /* Wait for semaphore again, should be released from dismiss function */
                __GUI_SYS_PROTECT();                /* Protect back before continuing */
                gui_sys_sem_delete(&l->sem);        /* Delete system semaphore */
                resp = l->status;                   /* Get new status */
                remove_from_active_dialogs(l);      /* Remove from active dialogs */
            } else {
                guii_widget_remove(ptr);           /* Remove widget */
            }
        } else {
            guii_widget_remove(ptr);               /* Remove widget */
        }
        __GUI_LEAVE();                              /* Leave GUI */
    }
    
    return resp;
}
#endif /* GUI_CFG_OS || __DOXYGEN__ */

/**
 * \brief           Dismiss (close) dialog with status
 * \param[in]       h: Widget handle
 * \param[in]       status: Dismiss status. Do not use value -1 as it is reserved for error detection
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_dialog_dismiss(gui_handle_p h, int status) {
    dissmissed_dialog_list_t* l;
    uint8_t ret = 0;
    gui_widget_param_t param = {0};
    
    /* Do not check widget type as it is not dialog type but create function widget type */
    __GUI_ASSERTPARAMS(h);                          /* Check input parameters */
    __GUI_ENTER();                                  /* Enter GUI */
    
    l = get_dialog(h);                              /* Get entry from list */
    if (l != NULL) {
        l->status = status;                         /* Save status for later */
        
        GUI_WIDGET_PARAMTYPE_INT(&param) = l->status;
        guii_widget_callback(h, GUI_WC_OnDismiss, &param, NULL);   /* Process callback */
#if GUI_CFG_OS
        if (l->ib && gui_sys_sem_isvalid(&l->sem)) {/* Check if semaphore is valid */
            gui_sys_sem_release(&l->sem);           /* Release locked semaphore */
        } else 
#endif /* GUI_CFG_OS */
        {
            remove_from_active_dialogs(l);          /* Remove from active dialogs */
        }
        guii_widget_remove(h);                     /* Remove widget */
    }
    
    __GUI_LEAVE();                                  /* Leave GUI */
    return ret;
}
