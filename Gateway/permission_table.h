/*
 * permission_table.h
 *
 * Created: 6/8/2018 4:46:25 PM
 *  Author: XXXX
 */ 


#ifndef PERMISSION_TABLE_H_
#define PERMISSION_TABLE_H_

#include <stdint.h>
#include "types.h"

typedef uint8_t role_t;

typedef struct permission {
	role_t role;
	struct permission *successor;
	} permission_t;
	
typedef struct entry {
	can_id_t id;
	permission_t *permissions;
	struct entry *successor;
} entry_t;

typedef struct permission_table_struct{
	entry_t *head;
} permission_table_t;

/* init_permissions_table() function.
Initialize the permission table with the example specified in thesis.pdf.
*/
void init_permissions_table();

/* add_entry() function.
Add a new permissions table entry.
	
	Inputs:
		id		- The CAN identifier of the new entry.

	Returns:	
				- 0 if the entry was successfully added.
				- 1 if the entry was already added to the table.	
*/
int add_entry(can_id_t id);

/* add_permission() function.
Add a new permission to an existing table entry.

	Inputs:
		id		- The CAN identifier of the intended entry.
		role	- The byte presentation of the newly added permission.

	Returns:	
				- 0 if the permission was successfully added.
				- 1 if the intended entry was not found.
				- 2 if the permission was already added to the intended entry.
*/
int add_permission(can_id_t id, role_t role);

/* check_permission() function.
Check the permission table to verify whether the given role has permission to send a message with the given ID.

	Inputs:
		id		- The CAN identifier of the intended entry.
		role	- The byte representation of the role that is to be checked.
		
	Returns:
				- 0 if the permission is granted.
				- 1 if the permission is not granted.
				- 2 if the corresponding entry was not found in the table.
*/ 
int check_permission(can_id_t id, role_t role);

#endif /* PERMISSION_TABLE_H_ */