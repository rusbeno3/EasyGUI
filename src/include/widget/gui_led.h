/**	
 * \file            gui_led.h
 * \brief           LED widget
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
#ifndef __GUI_LED_H
#define __GUI_LED_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_widget.h"

/**
 * \ingroup         GUI_WIDGETS
 * \defgroup        GUI_LED Led
 * \brief           Simple LED
 * \{
 */

/**
 * \brief           List of available colors for LED
 */
typedef enum {
    GUI_LED_COLOR_ON = 0x00,                /*!< Color index for LED on status */
    GUI_LED_COLOR_OFF,                      /*!< Color index for LED off status */
    GUI_LED_COLOR_ON_BORDER,                /*!< Color index for LED border on status */
    GUI_LED_COLOR_OFF_BORDER                /*!< Color index for LED border off status */
} gui_led_color_t;

/**
 * \brief           List of available LED types
 */
typedef enum {
    GUI_LED_TYPE_RECT = 0x00,               /*!< Led is rectangular */
    GUI_LED_TYPE_CIRCLE = 0x01              /*!< Led is circle */
} gui_led_type_t;

#if defined(GUI_INTERNAL) || __DOXYGEN__

#define GUI_LED_FLAG_ON             0x01    /*!< Indicates LED is on */

/**
 * \brief           LED object structure
 */
typedef struct {
    gui_handle C;                           /*!< Global widget object */
    
    gui_led_type_t type;                    /*!< Led shape type */
    uint8_t flags;                         /*!< flags management for LED */
} GUI_LED_t;
#endif /* defined(GUI_INTERNAL) || __DOXYGEN__ */

gui_handle_p    gui_led_create(gui_id_t id, float x, float y, float width, float height, gui_handle_p parent, gui_widget_callback_t cb, uint16_t flags);
uint8_t         gui_led_setcolor(gui_handle_p h, gui_led_color_t index, gui_color_t color);
uint8_t         gui_led_settype(gui_handle_p h, gui_led_type_t type);
uint8_t         gui_led_toggle(gui_handle_p h);
uint8_t         gui_led_set(gui_handle_p h, uint8_t state);
uint8_t         gui_led_ison(gui_handle_p h);

/**
 * \}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* __GUI_LED_H */
