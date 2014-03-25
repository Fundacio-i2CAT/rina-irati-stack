/*
 * Logs
 *
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef RINA_LOGS_H
#define RINA_LOGS_H

#ifndef RINA_PREFIX
#error You must define RINA_PREFIX before including this file
#endif

#include <linux/kernel.h>

/* The global logs prefix */
#define __GPFX "rina-"

#ifdef CONFIG_RINA_UNFILTERED_LOGS
#define __LOG(PFX, LVL, FMT, ARGS...)                                   \
        do { printk(KERN_NOTICE __GPFX PFX ": " FMT "\n", ##ARGS); } while (0)
#else
#define __LOG(PFX, LVL, FMT, ARGS...)                                   \
        do { printk(LVL __GPFX PFX ": " FMT "\n", ##ARGS); } while (0)
#endif

/* Sorted by "urgency" (high to low) */
#define LOG_EMERG(FMT, ARGS...) __LOG(RINA_PREFIX, KERN_EMERG,   FMT, ##ARGS)
#define LOG_ALERT(FMT, ARGS...) __LOG(RINA_PREFIX, KERN_ALERT,   FMT, ##ARGS)
#define LOG_CRIT(FMT,  ARGS...) __LOG(RINA_PREFIX, KERN_CRIT,    FMT, ##ARGS)
#define LOG_ERR(FMT,   ARGS...) __LOG(RINA_PREFIX, KERN_ERR,     FMT, ##ARGS)
#define LOG_WARN(FMT,  ARGS...) __LOG(RINA_PREFIX, KERN_WARNING, FMT, ##ARGS)
#define LOG_NOTE(FMT,  ARGS...) __LOG(RINA_PREFIX, KERN_NOTICE,  FMT, ##ARGS)
#define LOG_INFO(FMT,  ARGS...) __LOG(RINA_PREFIX, KERN_INFO,    FMT, ##ARGS)
#define LOG_DBG(FMT,  ARGS...)  __LOG(RINA_PREFIX, KERN_DEBUG,   FMT, ##ARGS)

#ifdef RINA_DEBUG_HEARTBEATS
#define LOG_HBEAT LOG_DBG("I'm in %s (%s:%d)",                  \
                          __FUNCTION__, __FILE__, __LINE__)
#else
#define LOG_HBEAT
#endif

#define LOG_OBSOLETE_FUNC LOG_ERR("Function %s is obsolete and it will be " \
                                  "removed soon, do not use",           \
                                  __FUNCTION__)
#define LOG_MISSING       LOG_ERR("Missing code in %s:%d", __FILE__, __LINE__)
#define LOG_UNSUPPORTED   LOG_WARN("Unsupported feature in %s:%d",  \
                                   __FILE__, __LINE__)

#endif
