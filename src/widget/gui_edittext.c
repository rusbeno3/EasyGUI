/**	
 * \file            gui_edittext.c
 * \brief           Edittext widget
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
#include "widget/gui_edittext.h"
#include "widget/gui_widget.h"

#if GUI_CFG_USE_KEYBOARD
#include "gui/gui_keyboard.h"
#endif /* GUI_CFG_USE_KEYBOARD */

#define __GE(x)             ((GUI_EDITTEXT_t *)(x))
    
#define CFG_MULTILINE       0x01
#define CFG_VALIGN          0x02
#define CFG_HALIGN          0x03
    
static uint8_t gui_edittext_callback(gui_handle_p h, gui_wc_t ctrl, gui_widget_param_t* param, gui_widget_result_t* result);

/**
 * \brief           List of default color in the same order of widget color enumeration
 */
static const
gui_color_t colors[] = {
    GUI_COLOR_WHITE,                                /*!< Default background color index */
    GUI_COLOR_BLACK,                                /*!< Default foreground color index */
    GUI_COLOR_BLACK,                                /*!< Default border color index */
};

/**
 * \brief           Widget initialization structure
 */
static const
gui_widget_t widget = {
    .name = _GT("EDITTEXT"),                        /*!< Widget name */
    .size = sizeof(GUI_EDITTEXT_t),                 /*!< Size of widget for memory allocation */
    .flags = 0,                                     /*!< List of widget flags */
    .callback = gui_edittext_callback,              /*!< Control function */
    .colors = colors,                               /*!< List of default colors */
    .color_count = GUI_COUNT_OF(colors),            /*!< Number of colors */
};

#define e          ((GUI_EDITTEXT_t *)h)

/* Check if edit text is multiline */
#define is_multiline(h)            (__GE(h)->flags & GUI_EDITTEXT_FLAG_MULTILINE)

/**
 * \brief           Default widget callback function
 * \param[in]       h: Widget handle
 * \param[in]       ctr: Callback type
 * \param[in]       param: Input parameters for callback type
 * \param[out]      result: Result for callback type
 * \return          1 if command processed, 0 otherwise
 */
static uint8_t
gui_edittext_callback(gui_handle_p h, gui_wc_t ctrl, gui_widget_param_t* param, gui_widget_result_t* result) {
    __GUI_ASSERTPARAMS(h != NULL && h->widget == &widget);  /* Check input parameters */
    switch (ctrl) {                                 /* Handle control function if required */
        case GUI_WC_PreInit: {
            __GE(h)->valign = GUI_EDITTEXT_VALIGN_CENTER;
            __GE(h)->halign = GUI_EDITTEXT_HALIGN_LEFT;
            return 1;
        }
        case GUI_WC_SetParam: {                     /* Set parameter for widget */
            gui_widget_param* p = GUI_WIDGET_PARAMTYPE_WIDGETPARAM(param);
            switch (p->type) {
                case CFG_MULTILINE:                 /* Enable/Disable multiline */
                    if (*(uint8_t *)p->data && !is_multiline(h)) {
                        __GE(h)->flags |= GUI_EDITTEXT_FLAG_MULTILINE;
                    } else if (!*(uint8_t *)p->data && is_multiline(h)) {
                        __GE(h)->flags &= ~GUI_EDITTEXT_FLAG_MULTILINE;
                    }
                    break; /* Set max X value to widget */
                case CFG_HALIGN: 
                    e->halign = *(gui_edittext_halign_t *)p->data;
                    break;
                case CFG_VALIGN: 
                    e->valign = *(gui_edittext_valign_t *)p->data;
                    break;
                default: break;
            }
            GUI_WIDGET_RESULTTYPE_U8(result) = 1;   /* Save result */
            return 1;
        }
        case GUI_WC_Draw: {
            gui_dim_t x, y, width, height;
            gui_display_t* disp = GUI_WIDGET_PARAMTYPE_DISP(param);
    
            x = guii_widget_getabsolutex(h);       /* Get absolute X coordinate */
            y = guii_widget_getabsolutey(h);       /* Get absolute Y coordinate */
            width = guii_widget_getwidth(h);       /* Get widget width */
            height = guii_widget_getheight(h);     /* Get widget height */
            
            gui_draw_rectangle3d(disp, x, y, width, height, GUI_DRAW_3D_State_Lowered);
            gui_draw_filledrectangle(disp, x + 2, y + 2, width - 4, height - 4, guii_widget_getcolor(h, GUI_EDITTEXT_COLOR_BG));
            
            if (guii_widget_isfocused(h)) {        /* Check if widget is in focus */
                gui_draw_rectangle(disp, x + 3, y + 3, width - 6, height - 6, guii_widget_getcolor(h, GUI_EDITTEXT_COLOR_BORDER));
            }
            
            if (guii_widget_isfontandtextset(h)) { /* Ready to write string */
                gui_draw_font_t f;
                gui_draw_font_init(&f);             /* Init font drawing */
                
                f.x = x + 5;
                f.y = y + 5;
                f.width = width - 10;
                f.height = height - 10;
                f.align = (uint8_t)__GE(h)->halign | (uint8_t)__GE(h)->valign;
                f.color1width = f.width;
                f.color1 = guii_widget_getcolor(h, GUI_EDITTEXT_COLOR_FG);
                f.flags |= GUI_FLAG_FONT_RIGHTALIGN | GUI_FLAG_FONT_EDITMODE;
                
                if (is_multiline(h)) {
                    f.flags |= GUI_FLAG_FONT_MULTILINE; /* Set multiline flag for widget */
                }
                
                gui_draw_writetext(disp, guii_widget_getfont(h), guii_widget_gettext(h), &f);
            }
            return 1;
        }
        case GUI_WC_FocusIn:
#if GUI_CFG_USE_KEYBOARD
            guii_keyboard_show(h);                 /* Show keyboard */
#endif /* GUI_CFG_USE_KEYBOARD */
            return 1;
        case GUI_WC_FocusOut:
#if GUI_CFG_USE_KEYBOARD
            guii_keyboard_hide();                  /* Hide keyboard */
#endif /* GUI_CFG_USE_KEYBOARD */
            return 1;
#if GUI_CFG_USE_TOUCH
        case GUI_WC_TouchStart: {
            GUI_WIDGET_RESULTTYPE_TOUCH(result) = touchHANDLED;
            return 1;
        }
#endif /* GUI_CFG_USE_TOUCH */
#if GUI_CFG_USE_KEYBOARD
        case GUI_WC_KeyPress: {
            guii_keyboard_data_t* kb = GUI_WIDGET_PARAMTYPE_KEYBOARD(param);    /* Get keyboard data */
            if (guii_widget_processtextkey(h, kb)) {
                GUI_WIDGET_RESULTTYPE_KEYBOARD(result) = keyHANDLED;
            }
            return 1;
        }
#endif /* GUI_CFG_USE_KEYBOARD */
        default:                                    /* Handle default option */
            GUI_UNUSED3(h, param, result);          /* Unused elements to prevent compiler warnings */
            return 0;                               /* Command was not processed */
    }
}

#undef e

/**
 * \brief           Create new edit text widget
 * \param[in]       id: Widget unique ID to use for identity for callback processing
 * \param[in]       x: Widget X position relative to parent widget
 * \param[in]       y: Widget Y position relative to parent widget
 * \param[in]       width: Widget width in units of pixels
 * \param[in]       height: Widget height in uints of pixels
 * \param[in]       parent: Parent widget handle. Set to NULL to use current active parent widget
 * \param[in]       cb: Pointer to \ref gui_widget_callback_t callback function. Set to NULL to use default widget callback
 * \param[in]       flags: flags for widget creation
 * \return          \ref gui_handle_p object of created widget on success, NULL otherwise
 */
gui_handle_p
gui_edittext_create(gui_id_t id, float x, float y, float width, float height, gui_handle_p parent, gui_widget_callback_t cb, uint16_t flags) {
    return (gui_handle_p)guii_widget_create(&widget, id, x, y, width, height, parent, cb, flags);  /* Allocate memory for basic widget */
}

/**
 * \brief           Set color to specific part of widget
 * \param[in,out]   h: Widget handle
 * \param[in]       index: Color index. This parameter can be a value of \ref GUI_EDITTEXT_COLOR_t enumeration
 * \param[in]       color: Color value
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_edittext_setcolor(gui_handle_p h, gui_edittext_color_t index, gui_color_t color) {
    __GUI_ASSERTPARAMS(h != NULL && h->widget == &widget);  /* Check input parameters */
    return guii_widget_setcolor(h, (uint8_t)index, color);  /* Set color */
}

/**
 * \brief           Set edit text to multi line mode
 * \note            When multiline is enabled, vertical text alignment is always top positioned
 * \param[in,out]   h: Widget handle
 * \param[in]       multiline: Set to 1 to enable multiline or 0 to disable
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_edittext_setmultiline(gui_handle_p h, uint8_t multiline) {
    __GUI_ASSERTPARAMS(h != NULL && h->widget == &widget);  /* Check input parameters */
    return guii_widget_setparam(h, CFG_MULTILINE, &multiline, 1, 0);   /* Set parameter */
}

/**
 * \brief           Set vertical align for text inside text box
 * \param[in,out]   h: Widget handle
 * \param[in]       align: Vertical align. This parameter can be a value of \ref GUI_EDITTEXT_VALIGN_t enumeration
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_edittext_setvalign(gui_handle_p h, gui_edittext_valign_t align) {
    __GUI_ASSERTPARAMS(h != NULL && h->widget == &widget);  /* Check input parameters */
    return guii_widget_setparam(h, CFG_VALIGN, &align, 1, 1);  /* Set parameter */
}

/**
 * \brief           Set horizontal align for text inside text box
 * \param[in,out]   h: Widget handle
 * \param[in]       align: Vertical align. This parameter can be a value of \ref GUI_EDITTEXT_HALIGN_t enumeration
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_edittext_sethalign(gui_handle_p h, gui_edittext_halign_t align) {
    __GUI_ASSERTPARAMS(h != NULL && h->widget == &widget);  /* Check input parameters */
    return guii_widget_setparam(h, CFG_HALIGN, &align, 1, 1);  /* Set parameter */
}
