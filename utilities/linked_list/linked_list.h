/*
 * Copyright 2019 Cypress Semiconductor Corporation
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "aws_error.h"
#include "stdint.h"
/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

typedef struct linked_list_node linked_list_node_t;

/******************************************************
 *                    Structures
 ******************************************************/

#pragma pack(1)
struct linked_list_node
{
    void*               data;
    linked_list_node_t* next;
    linked_list_node_t* prev;
};

typedef struct
{
    uint32_t            count;
    linked_list_node_t* front;
    linked_list_node_t* rear;
} linked_list_t;
#pragma pack()

typedef wiced_bool_t (*linked_list_compare_callback_t)( linked_list_node_t* node_to_compare, void* user_data );

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

wiced_result_t linked_list_init( linked_list_t* list );

wiced_result_t linked_list_deinit( linked_list_t* list );

wiced_result_t linked_list_get_count( linked_list_t* list, uint32_t* count );

wiced_result_t linked_list_set_node_data( linked_list_node_t* node, const void* data );

wiced_result_t linked_list_get_front_node( linked_list_t* list, linked_list_node_t** front_node );

wiced_result_t linked_list_get_rear_node( linked_list_t* list, linked_list_node_t** rear_node );

wiced_result_t linked_list_find_node( linked_list_t* list, linked_list_compare_callback_t callback, void* user_data, linked_list_node_t** node_found );

wiced_result_t linked_list_insert_node_at_front( linked_list_t* list, linked_list_node_t* node );

wiced_result_t linked_list_insert_node_at_rear( linked_list_t* list, linked_list_node_t* node );

wiced_result_t linked_list_insert_node_before( linked_list_t* list, linked_list_node_t* reference_node, linked_list_node_t* node_to_insert );

wiced_result_t linked_list_insert_node_after( linked_list_t* list, linked_list_node_t* reference_node, linked_list_node_t* node_to_insert );

wiced_result_t linked_list_remove_node( linked_list_t* list, linked_list_node_t* node );

wiced_result_t linked_list_remove_node_from_front( linked_list_t* list, linked_list_node_t** removed_node );

wiced_result_t linked_list_remove_node_from_rear( linked_list_t* list, linked_list_node_t** removed_node );

#ifdef __cplusplus
} /* extern "C" */
#endif
