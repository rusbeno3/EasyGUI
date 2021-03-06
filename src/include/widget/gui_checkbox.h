/**	
 * \file            gui_checkbox.h
 * \brief           Check bar widget
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
#ifndef __GUI_CHECKBOX_H
#define __GUI_CHECKBOX_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_widget.h"

/**
 * \ingroup         GUI_WIDGETS
 * \defgroup        GUI_CHECKBOX Checkbox
 * \brief           Checkbox widget
 * \{
 */
    
/**
 * \brief           List of checkbox colors
 */
typedef enum {
    GUI_CHECKBOX_COLOR_BG = 0x00,           /*!< Background color index */
    GUI_CHECKBOX_COLOR_FG,                  /*!< Foreground color index */
    GUI_CHECKBOX_COLOR_BORDER,              /*!< Border color index */
    GUI_CHECKBOX_COLOR_DISABLED_BG,         /*!< Background color index when disabled */
    GUI_CHECKBOX_COLOR_TEXT,                /*!< Text color index */
} gui_checkbox_color_t;

#if defined(GUI_INTERNAL) || __DOXYGEN__

#define GUI_FLAG_CHECKBOX_CHECKED           0x01    /*!< Indicates checkbox is checked */
#define GUI_FLAG_CHECKBOX_DISABLED          0x02    /*!< Indicates checkbox is disabled */
    
/**
 * \brief           Checkbox object structure
 */
typedef struct {
    gui_handle C;                           /*!< GUI handle object, must always be first on list */
    
    uint8_t flags;                          /*!< Widget flags */
} gui_checkbox_t;
#endif /* defined(GUI_INTERNAL) || __DOXYGEN__ */

gui_handle_p    gui_checkbox_create(gui_id_t id, float x, float y, float width, float height, gui_handle_p parent, gui_widget_callback_t cb, uint16_t flags);
uint8_t         gui_checkbox_setcolor(gui_handle_p h, gui_checkbox_color_t index, gui_color_t color);
uint8_t         gui_checkbox_setchecked(gui_handle_p h, uint8_t checked);
uint8_t         gui_checkbox_ischecked(gui_handle_p h);
uint8_t         gui_checkbox_setdisabled(gui_handle_p h, uint8_t disabled);
uint8_t         gui_checkbox_isdisabled(gui_handle_p h);

/**
 * \}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* __GUI_CHECKBOX_H */
