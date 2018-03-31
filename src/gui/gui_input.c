/**	
 * \file            gui_input.c
 * \brief           Input manager
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
#include "gui/gui_input.h"
#include "system/gui_sys.h"

#if GUI_CFG_USE_TOUCH
static GUI_BUFFER_t TSBuffer;
static GUI_Byte_t TSBufferData[GUI_CFG_TOUCH_BUFFER_SIZE * sizeof(gui_touchdata_t) + 1];
#endif /* GUI_CFG_USE_TOUCH */
#if GUI_CFG_USE_KEYBOARD
static GUI_BUFFER_t KBBuffer;
static GUI_Byte_t KBBufferData[GUI_CFG_KEYBOARD_BUFFER_SIZE * sizeof(gui_keyboarddata_t) + 1];
#endif /* GUI_CFG_USE_KEYBOARD */

#if GUI_CFG_USE_TOUCH || __DOXYGEN__

/**
 * \brief           Add new touch data to internal buffer for further processing
 * \param[in]       ts: Pointer to \ref gui_touchdata_t touch data with valid input
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_input_touchadd(gui_touchdata_t* ts) {
    uint8_t ret;
    __GUI_ASSERTPARAMS(ts);                         /* Check input parameters */
    ts->Time = gui_sys_now();                       /* Set event time */
    ret = gui_buffer_write(&TSBuffer, ts, sizeof(*ts)) ? 1 : 0; /* Write data to buffer */
    
#if GUI_CFG_OS
    /*
     * Notify OS to wakeup thread
     */
    static gui_mbox_msg_t gui_touch_value = {GUI_SYS_MBOX_TYPE_TOUCH};  /* Enter some value, don't care about */
    gui_sys_mbox_putnow(&GUI.OS.mbox, &gui_touch_value);    /* Notify stack about new key added */
#endif /* GUI_CFG_OS */
    return ret;
}

/**
 * \brief           Reads new touch entry
 * \param[out]      ts: Pointer to \ref gui_touchdata_t structure to save touch into to
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_input_touchread(gui_touchdata_t* ts) {
    if (gui_buffer_getfull(&TSBuffer) >= sizeof(*ts)) {
        return (uint8_t)gui_buffer_read(&TSBuffer, ts, sizeof(*ts)); /* Read data fro mbuffer */
    }
    return 0;
}

/**
 * \brief           Checks if anything available for touch inputs
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_input_touchavailable(void) {
    return gui_buffer_getfull(&TSBuffer) > 0;       /* Check if any available touch */
}

#endif /* GUI_CFG_USE_TOUCH || __DOXYGEN__ */

#if GUI_CFG_USE_KEYBOARD || __DOXYGEN__

/**
 * \brief           Add new key data to internal buffer for further processing
 * \param[in]       kb: Pointer to \ref gui_keyboarddata_t key data
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_input_keyadd(gui_keyboarddata_t* kb) {
    uint8_t ret;
    __GUI_ASSERTPARAMS(kb);                         /* Check input parameters */
    kb->Time = gui_sys_now();                       /* Set event time */
    ret = gui_buffer_write(&KBBuffer, kb, sizeof(*kb)) ? 1 : 0; /* Write data to buffer */
    
#if GUI_CFG_OS
    /*
     * Notify OS to wakeup thread
     */
    static gui_mbox_msg_t gui_kbd_value = {GUI_SYS_MBOX_TYPE_KEYBOARD}; /* Enter some value, don't care about */
    gui_sys_mbox_putnow(&GUI.OS.mbox, &gui_kbd_value);   /* Notify stack about new key added */
#endif /* GUI_CFG_OS */
    return ret;
}

/**
 * \brief           Read keyboard entry from buffer
 * \param[out]      kb: Pointer to \ref gui_keyboarddata_t to save entry to
 * \return          `1` on success, `0` otherwise
 */
uint8_t
gui_input_keyread(gui_keyboarddata_t* kb) {
    if (gui_buffer_getfull(&KBBuffer) >= sizeof(*kb)) {
        return (uint8_t)gui_buffer_read(&KBBuffer, kb, sizeof(*kb)); /* Read data fro mbuffer */
    }
    return 0;
}
#endif /* GUI_CFG_USE_KEYBOARD || __DOXYGEN__ */

/**
 * \brief           Initialize input manager for GUI
 */
void
gui_input_init(void) {
#if GUI_CFG_USE_TOUCH
    gui_buffer_init(&TSBuffer, sizeof(TSBufferData), TSBufferData);
#endif /* GUI_CFG_USE_TOUCH */
#if GUI_CFG_USE_KEYBOARD
    gui_buffer_init(&KBBuffer, sizeof(KBBufferData), KBBufferData);
#endif /* GUI_CFG_USE_KEYBOARD */
}
