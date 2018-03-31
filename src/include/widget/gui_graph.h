/**	
 * \file            gui_graph.c
 * \brief           Graph widget
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
#ifndef __GUI_GRAPH_H
#define __GUI_GRAPH_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup      GUI_WIDGETS
 * \{
 */
#include "gui_widget.h"

/**
 * \defgroup        GUI_GRAPH Graph
 * \brief           Graph which can display plots
 * \{
 */
    
/**
 * \brief           Graph color list enumeration
 * \sa              gui_graph_setcolor
 */
typedef enum GUI_GRAPH_COLOR_t {
    GUI_GRAPH_COLOR_BG = 0x00,              /*!< Background color index */
    GUI_GRAPH_COLOR_FG = 0x01,              /*!< Foreground color index (background of plotting area) */
    GUI_GRAPH_COLOR_BORDER = 0x02,          /*!< Border color index */
    GUI_GRAPH_COLOR_GRID = 0x03             /*!< Grid color index */
} GUI_GRAPH_COLOR_t;

/**
 * \brief           Graph border list enumeration
 */
typedef enum GUI_GRAPH_BORDER_t {
    GUI_GRAPH_BORDER_TOP = 0x00,            /*!< Border top index */
    GUI_GRAPH_BORDER_RIGHT = 0x01,          /*!< Border right index */
    GUI_GRAPH_BORDER_BOTTOM = 0x02,         /*!< Border bottom index */
    GUI_GRAPH_BORDER_LEFT = 0x03            /*!< Border left index */
} GUI_GRAPH_BORDER_t;

/**
 * \brief           Graph type enumeration
 */
typedef enum GUI_GRAPH_TYPE_t {
    GUI_GRAPH_TYPE_YT = 0x00,               /*!< Data type is Y versus time */
    GUI_GRAPH_TYPE_XY = 0x01,               /*!< Data type is Y versus X [Y(x)] */
} GUI_GRAPH_TYPE_t;

/**
 * \brief           Graph data widget structure
 */
typedef struct GUI_GRAPH_DATA_p {
#if GUI_CFG_WIDGET_GRAPH_DATA_AUTO_INVALIDATE || __DOXYGEN__
    gui_linkedlistroot_t Root;              /*!< Root linked list object of graph widgets */
#endif /* GUI_CFG_WIDGET_GRAPH_DATA_AUTO_INVALIDATE */
    
    int16_t* Data;                          /*!< Pointer to actual data object */
    size_t Length;                          /*!< Size of data array */
    size_t Ptr;                             /*!< Read/Write start pointer */
    
    gui_color_t Color;                      /*!< Curve color */
    GUI_GRAPH_TYPE_t Type;                  /*!< Plot data type */
} GUI_GRAPH_DATA_t;

typedef GUI_GRAPH_DATA_t *  GUI_GRAPH_DATA_p;   /*!< GUI Graph data pointer */

#if defined(GUI_INTERNAL) || __DOXYGEN__
/**
 * \brief           Graph widget structure
 */
typedef struct GUI_GRAPH_t {
    gui_handle C;                           /*!< GUI handle object, must always be first on list */
    gui_linkedlistroot_t Root;              /*!< Linked list root object for data objects. It stores where first in last data exists for this graph */
    
    gui_dim_t Border[4];                    /*!< Borders for widgets */
    uint8_t Rows;                           /*!< Number of rows in plot represented with lines */
    uint8_t Columns;                        /*!< Number of columns in plot represented with lines */
    float MinX;                             /*!< Minimal X value for plot */
    float MaxX;                             /*!< Maximal X value for plot */
    float MinY;                             /*!< Minimal Y value for plot */
    float MaxY;                             /*!< Maximal Y value for plot */
    float VisibleMinX;                      /*!< Visible minimal X value for plot */
    float VisibleMaxX;                      /*!< Visible maximal X value for plot */
    float VisibleMinY;                      /*!< Visible minimal Y value for plot */
    float VisibleMaxY;                      /*!< Visible maximal Y value for plot */
} GUI_GRAPH_t;
#endif /* defined(GUI_INTERNAL) || __DOXYGEN__ */

gui_handle_p gui_graph_create(gui_id_t id, float x, float y, float width, float height, gui_handle_p parent, GUI_WIDGET_CALLBACK_t cb, uint16_t flags);
uint8_t gui_graph_setcolor(gui_handle_p h, GUI_GRAPH_COLOR_t index, gui_color_t color);
uint8_t gui_graph_setminx(gui_handle_p h, float v);
uint8_t gui_graph_setmaxx(gui_handle_p h, float v);
uint8_t gui_graph_setminy(gui_handle_p h, float v);
uint8_t gui_graph_setmaxy(gui_handle_p h, float v);
uint8_t gui_graph_zoomreset(gui_handle_p h);
uint8_t gui_graph_zoom(gui_handle_p h, float zoom, float x, float y);
uint8_t gui_graph_attachdata(gui_handle_p h, GUI_GRAPH_DATA_p data);
uint8_t gui_graph_detachdata(gui_handle_p h, GUI_GRAPH_DATA_p data);

GUI_GRAPH_DATA_p gui_graph_data_create(GUI_GRAPH_TYPE_t type, size_t length);
uint8_t gui_graph_data_addvalue(GUI_GRAPH_DATA_p data, int16_t x, int16_t y);
uint8_t gui_graph_data_setcolor(GUI_GRAPH_DATA_p data, gui_color_t color);
 
/**
 * \}
 */

/**
 * \}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* __GUI_GRAPH_H */
