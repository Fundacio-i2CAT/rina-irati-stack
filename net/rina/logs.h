/*
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

#define __LOG(PREFIX, LEVEL, FMT, ARGS...)                              \
	do { printk(LEVEL "rina-" PREFIX ": " FMT, ##ARGS); } while (0)

/* Sorted by "urgency" (high to low) */
#define LOG_EMERG(FMT, ARGS...) __LOG(RINA_PREFIX, KERN_EMERG,   FMT, ##ARGS)
#define LOG_ALERT(FMT, ARGS...) __LOG(RINA_PREFIX, KERN_ALERT,   FMT, ##ARGS)
#define LOG_CRIT(FMT,  ARGS...) __LOG(RINA_PREFIX, KERN_CRIT,    FMT, ##ARGS)
#define LOG_ERR(FMT,   ARGS...) __LOG(RINA_PREFIX, KERN_ERR,     FMT, ##ARGS)
#define LOG_WARN(FMT,  ARGS...) __LOG(RINA_PREFIX, KERN_WARNING, FMT, ##ARGS)
#define LOG_NOTE(FMT,  ARGS...) __LOG(RINA_PREFIX, KERN_NOTICE,  FMT, ##ARGS)
#define LOG_INFO(FMT,  ARGS...) __LOG(RINA_PREFIX, KERN_INFO,    FMT, ##ARGS)
#define LOG_DBG(FMT,  ARGS...)  __LOG(RINA_PREFIX, KERN_DEBUG,   FMT, ##ARGS)

#endif
