/*
 * permission_table.c
 *
 * Created: 6/8/2019 4:19:07 PM
 *  Author: XXXX
 */ 

#include "permission_table.h"
#include "hexconv.h"
#include <stdlib.h>
#include <string.h>

permission_table_t *permission_table; 

int add_permission_hex(char *hex, role_t role){
	can_id_t id;
	hex_to_bytes(hex,4,id);
	return add_permission(id,role);
}

int add_entry_hex(char *hex){
	can_id_t id;
	hex_to_bytes(hex, 4, id);
	add_entry(id);
	return 0;
}

int add_entry(can_id_t id){
	entry_t *new_entry; 
	new_entry = malloc(sizeof(entry_t));
	new_entry->permissions = NULL;
	new_entry->successor = NULL;
	memcpy(new_entry->id, id, sizeof(can_id_t));
	entry_t *curr = permission_table->head;
	if(curr != NULL){
		while(curr->successor != NULL){
			if(entrycmp(curr, id) == 0) return 1;
			curr = curr->successor;
		}
		if(entrycmp(curr, id) == 0) return 1;
		curr->successor = new_entry;
	}else{
		permission_table->head = new_entry;
	}
	return 0;
}

int add_permission(can_id_t id, role_t role){
	volatile permission_t *new_permission;
	new_permission = malloc(sizeof(permission_t));
	new_permission->successor = NULL;
	new_permission->role = role;
	entry_t *entry;
	if(find_entry(id, &entry)) return 1;
	if(entry->permissions == NULL){
		entry->permissions = new_permission;
		return 0;
	}else{
		permission_t *curr = entry->permissions;
		while(curr->successor != NULL){
			if(role == curr->role) return 2;
			curr = curr->successor;
		}
		if(role == curr->role) return 2;
		curr->successor = new_permission;		
	}
	return 0;
}
	

int entrycmp(entry_t *entry, can_id_t msg_id){
	volatile uint8_t res =  memcmp(entry->id, msg_id, 2);
	return res;
}

int find_entry(can_id_t msg_id, entry_t **dest){
	entry_t *curr = permission_table->head;
	while(curr != NULL){
		if(entrycmp(curr, msg_id) == 0){
			*dest = curr;
			return 0;
		}else{
			curr = curr->successor;
		}
	}
	return 1;
}

int find_permission(entry_t *entry, role_t role, permission_t **dest){
	permission_t *curr = entry->permissions;
	while(curr != NULL){
		if(role == curr->role){
			*dest = curr;
			return 0;
		}
		curr = curr->successor;
	}
	return 1;
}

int check_permission(can_id_t id, role_t role){
	entry_t *entry;
	if(find_entry(id, &entry)) return 2;
	permission_t *p;
	return find_permission(entry, role, &p);
}

void init_permissions_table(){
	permission_table = malloc(sizeof(permission_table_t));
	permission_table->head = NULL;
	add_entry_hex("0201");
	add_permission_hex("0201", ADMIN_ROLE);
	add_permission_hex("0201", OEM_ROLE);
	add_permission_hex("0201", POLICEMAN_ROLE);
	add_permission_hex("0201", REPAIRMAN_ROLE);
	add_permission_hex("0201", OWNER_ROLE);
	add_entry_hex("0402");
	add_permission_hex("0402", ADMIN_ROLE);
	add_permission_hex("0402", OEM_ROLE);
	add_permission_hex("0402", REPAIRMAN_ROLE);
	add_entry_hex("0000");
	add_permission_hex("0000", ADMIN_ROLE);
	add_permission_hex("0000", OEM_ROLE);
}