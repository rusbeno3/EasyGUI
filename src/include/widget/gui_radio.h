/**	
 * \file            gui_radio.h
 * \brief           Radio box widget
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
#ifndef __GUI_RADIO_H
#define __GUI_RADIO_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_widget.h"

/**
 * \ingroup         GUI_WIDGETS
 * \defgroup        GUI_RADIO Radio box
 * \brief           Radio box widget
 * \{
 */
    
/**
 * \brief           List of radio colors
 */
typedef enum {
    GUI_RADIO_COLOR_BG = 0x00,              /*!< Background color index */
    GUI_RADIO_COLOR_FG,                     /*!< Foreground color index */
    GUI_RADIO_COLOR_BORDER,                 /*!< Border color index */
    GUI_RADIO_COLOR_DISABLED_BG,            /*!< Background color index when disabled */
} gui_radio_color_t;
    
#if defined(GUI_INTERNAL) || __DOXYGEN__
    
#define GUI_FLAG_RADIO_CHECKED              0x01    /*!< Indicates radio is currently checked */
#define GUI_FLAG_RADIO_DISABLED             0x02    /*!< Indicates radio is currently disabled */

/**
 * \brief           Radio object structure
 */
typedef struct {
    gui_handle C;                           /*!< GUI handle object, must always be first on list */
    
    uint8_t group_id;                       /*!< Group ID for radio box */
    uint32_t value;                         /*!< Single radio value when selected */
    uint32_t selected_value;                /*!< Currently selected value in radio group. 
                                                    All Widgets in the same group share the same value in this field all the time */
    uint8_t flags;                          /*!< flags for checkbox */
} gui_radio_t;
#endif /* defined(GUI_INTERNAL) || __DOXYGEN__ */

gui_handle_p    gui_radio_create(gui_id_t id, float x, float y, float width, float height, gui_handle_p parent, gui_widget_callback_t cb, uint16_t flags);
uint8_t         gui_radio_setcolor(gui_handle_p h, gui_radio_color_t index, gui_color_t color);
uint8_t         gui_radio_setgroup(gui_handle_p h, uint8_t groupId);
uint8_t         gui_radio_getgroup(gui_handle_p h);
uint8_t         gui_radio_setvalue(gui_handle_p h, uint32_t value);
uint32_t        gui_radio_getvalue(gui_handle_p h);
uint32_t        gui_radio_getselectedvalue(gui_handle_p h);
uint8_t         gui_radio_setdisabled(gui_handle_p h, uint8_t disabled);
uint8_t         gui_radio_isdisabled(gui_handle_p h);
uint8_t         gui_radio_setselected(gui_handle_p h);
    
/**
 * \}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* __GUI_RADIO_H */
