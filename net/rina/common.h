/*
 *  Common definition placeholder
 *
 *    Francesco Salvestrini <f.salvestrini@nextworks.it>
 *    Leonardo Bergesio <leonardo.bergesio@i2cat.net> 
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

#ifndef RINA_COMMON_H
#define RINA_COMMON_H

typedef int ipc_process_id_t;
typedef unsigned int ipc_process_address_t;
typedef utf8_t string_t;
typedef uint16_t port_id_t;


/*-----------------------------------------------------------------------------
 * The value should be interpreted as false if the value is 0 or true otherwise.
 * This typedef should be interpreted as ISO C99 bool/_Bool and could be
 * replaced by the inclusion of stdbool.h where/when possible.
 *-----------------------------------------------------------------------------*/
typedef int bool_t;

/* This structure represents raw data */
struct buffer_t{
	char *data;
	size_t size;
};

/* An SDU */
struct sdu_t{
	buffer_t *buffer;
};

struct uint_range_t{
	/*-----------------------------------------------------------------------------
	 *  This structure represents a range of integer values
	 *-----------------------------------------------------------------------------*/

	/* Minimum value */
	uint_t min_value;

	/* Maximum value */
	uint_t max_value;
};


struct name_t{
	/*-----------------------------------------------------------------------------
	 * The process_name identifies an application process within the
	 * application process namespace. This value is required, it
	 * cannot be NULL. This name has global scope (it is defined by
	 * the chain of IDD databases that are linked together), and is
	 * assigned by an authority that manages the namespace that
	 * particular application name belongs to.
	 *-----------------------------------------------------------------------------*/
	string_t *process_name;
	

	/*-----------------------------------------------------------------------------
	 * The process_instance identifies a particular instance of the
	 * process. This value is optional, it may be NULL.
	 *-----------------------------------------------------------------------------*/
	string_t *process_instance;

	
	/*-----------------------------------------------------------------------------
	 * The entity_name identifies an application entity within the
	 * application process. This value is optional, it may be NULL.
	 *-----------------------------------------------------------------------------*/
	string_t *entity_name;

	
	/*-----------------------------------------------------------------------------
	 * The entity_name identifies a particular instance of an entity within
	 * the application process. This value is optional, it may be NULL.
	 *-----------------------------------------------------------------------------*/
	string_t *entity_instance;

};



#endif
