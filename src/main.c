#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "person.h"
#include "linkedlist.h"
#include "BST.h"
#include "rbtree.h"

void _init_data(char* data[7]) {
    // takes an array that holds strings of data items and initializes each entry to one character
    for(int i=0; i<7;i++) {
        data[i] = malloc(sizeof(char));
        data[i][0] = '\0';
    }
}

gender_t gen_str_to_enum(char* gender) {
    // takes a string literal and converts it to a gender_t enum value. Defaults to 3 - unspecified.
    if (strcmp(gender, "M") == 0) {
        return MALE;
    }
    else if (strcmp(gender, "F") == 0) {
        return FEMALE;
    }
    else {
        return 3;
    }
}

person_t make_person_from_data(char* data[7]) {
    // takes a data array from parse_line and generates a person object
    person_t new_person;
    new_person.id = atoi(data[0]);
    new_person.age = atoi(data[4]);
    new_person.weight = atoi(data[6]);
    new_person.height = atoi(data[5]);
    new_person.gender = gen_str_to_enum(data[3]);
    new_person.first_name = strdup(data[2]);
    new_person.last_name = strdup(data[1]);
    return new_person;
}

void free_data(char *data[7]) {
    for(int i=0;i<7;i++) {
        free(data[i]);
    }
}

person_t parse_line(char *csvline) {
    // parses a line from a csv file and returns a person struct
    // id,last_name,first_name,gender,age,height,weight
    char* data[7]; // 7 items of data in each line
    _init_data(data); // allocate memory for each entry

    int curr_item = 0;
    int buffer_size = 25 * sizeof(char); // allow 25 characters for each entry initially (this can grow later)
    char *buffer = malloc(buffer_size); // generate a buffer to hold current item before adding it to data
    int buffer_pos = 0;

    //printf("\n\nLine for parsing: %s\n\n", csvline);

    for(int i = 0; csvline[i] != '\0'; i++) {
        // iterate through each character of the line
        if (curr_item > 6) {
            // dumb way to stop the loop if we go past 7 items on a line.
            break;
        }
        else if ( csvline[i] == ',' ) {
            // end of the entry. Add a null char and add the line to the data array
            buffer[buffer_pos] = '\0';
            data[curr_item] = realloc(data[curr_item], buffer_pos + 1); // reallocate the memory to make space
            strcpy(data[curr_item], buffer); // copy the buffer into the data array

            //printf("Adding %s to data array in the %d position.\n", data[curr_item], curr_item);

            buffer_pos = 0;
            curr_item++;
        }
        else {
            buffer[buffer_pos] = csvline[i];
            buffer_pos++;

            if (buffer_pos >= buffer_size) {
                buffer_size *= 2; // double the buffer size
                buffer = realloc(buffer, buffer_size); // reallocate memory for the new buffer size
            }
        }
    }
/*     printf("End of line. Buffer value is: %s\n", buffer);
    printf("Buffer position: %d\n", buffer_pos);
    printf("Current item count: %d\n", curr_item);
    if(curr_item < 7 && buffer_pos > 0) {
        // add the last element to the data array
        buffer[buffer_pos] = '\0';
        data[curr_item] = realloc(data[curr_item], buffer_pos + 1);
        strcpy(data[curr_item], buffer);
        printf("Current Item: %s", data[curr_item]);
    } */

    person_t result = make_person_from_data(data);
    free(buffer);
    free_data(data);
    return result;
}

void read_file_into_rbt(char *filepath, redblack_tree_t *tree) {
    // begin file reading logic
    FILE *file_ptr = fopen(filepath, "r");
    if (file_ptr == NULL ) {
        printf("Error openining file: %s", filepath);
        return;
    }
    char c = getc(file_ptr);
    int buffer_size = 256 * sizeof(char); // default to 256 character buffer
    char *buffer = malloc(buffer_size);
    int buffer_pos = 0;

    //logic to skip the header line
    bool header_skipped = false;

    while( c != EOF ) {
        if (c == '\n') {
            if(!header_skipped) {
                header_skipped = true;
                buffer_pos = 0;
                free(buffer);
                buffer = calloc(1, buffer_size);
            }
            else {
                person_t temp_person = parse_line(buffer);
                person_t *new_person = malloc(sizeof(person_t));
                new_person->id = temp_person.id;
                new_person->age = temp_person.age;
                new_person->height = temp_person.height;
                new_person->weight = temp_person.weight;
                new_person->gender = temp_person.gender;
                // copy the pointers to the name strings
                new_person->first_name = temp_person.first_name;
                new_person->last_name = temp_person.last_name;
                // nullify the name string pointers in the temp person
                temp_person.first_name = NULL;
                temp_person.last_name = NULL;
                //print_person(person);
                
                rbt_insert(tree, new_person, free_person_data, print_person_data);
                
                // free the memory of the temp_person
                free_person(&temp_person);
                buffer_pos = 0;
                free(buffer);
                buffer = calloc(1, buffer_size);
            }
        }
        if (buffer_pos >= buffer_size) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);
        }
        buffer[buffer_pos] = c;
        c = getc(file_ptr);
        buffer_pos++;
    }
    fclose(file_ptr);
    free(buffer);
    // end file reading logic
}

void read_file_into_bst(char *filepath, bst_t *tree) {
    // begin file reading logic
    FILE *file_ptr = fopen(filepath, "r");
    if (file_ptr == NULL ) {
        printf("Error openining file: %s", filepath);
        return;
    }
    char c = getc(file_ptr);
    int buffer_size = 256 * sizeof(char); // default to 256 character buffer
    char *buffer = malloc(buffer_size);
    int buffer_pos = 0;

    //logic to skip the header line
    bool header_skipped = false;

    while( c != EOF ) {
        if (c == '\n') {
            if(!header_skipped) {
                header_skipped = true;
                buffer_pos = 0;
                free(buffer);
                buffer = calloc(1, buffer_size);
            }
            else {
                person_t temp_person = parse_line(buffer);
                person_t *new_person = malloc(sizeof(person_t));
                new_person->id = temp_person.id;
                new_person->age = temp_person.age;
                new_person->height = temp_person.height;
                new_person->weight = temp_person.weight;
                new_person->gender = temp_person.gender;
                // copy the pointers to the name strings
                new_person->first_name = temp_person.first_name;
                new_person->last_name = temp_person.last_name;
                // nullify the name string pointers in the temp person
                temp_person.first_name = NULL;
                temp_person.last_name = NULL;
                //print_person(person);
                
                bst_insert(tree, new_person, free_person_data, print_person_data);
                
                // free the memory of the temp_person
                free_person(&temp_person);
                buffer_pos = 0;
                free(buffer);
                buffer = calloc(1, buffer_size);
            }
        }
        if (buffer_pos >= buffer_size) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);
        }
        buffer[buffer_pos] = c;
        c = getc(file_ptr);
        buffer_pos++;
    }
    fclose(file_ptr);
    free(buffer);
    // end file reading logic
}

void read_file_into_list(char *filepath, linked_list_t *list) {
    // begin file reading logic
    FILE *file_ptr = fopen(filepath, "r");
    if (file_ptr == NULL ) {
        printf("Error openining file: %s", filepath);
        return;
    }
    char c = getc(file_ptr);
    int buffer_size = 256 * sizeof(char); // default to 256 character buffer
    char *buffer = malloc(buffer_size);
    int buffer_pos = 0;

    //logic to skip the header line
    bool header_skipped = false;

    while( c != EOF ) {
        if (c == '\n') {
            if(!header_skipped) {
                header_skipped = true;
                buffer_pos = 0;
            }
            else {
                person_t temp_person = parse_line(buffer);
                person_t *new_person = malloc(sizeof(person_t));
                new_person->id = temp_person.id;
                new_person->age = temp_person.age;
                new_person->height = temp_person.height;
                new_person->weight = temp_person.weight;
                new_person->gender = temp_person.gender;
                // copy the pointers to the name strings
                new_person->first_name = temp_person.first_name;
                new_person->last_name = temp_person.last_name;
                // nullify the name string pointers in the temp person
                temp_person.first_name = NULL;
                temp_person.last_name = NULL;
                //print_person(person);
                
                add_item(list, new_person, free_person_data, print_person_data);
                
                // free the memory of the temp_person
                free_person(&temp_person);
                buffer_pos = 0;
                //buffer = realloc(buffer,256*sizeof(char)); // reset the buffer memory
            }
        }
        if (buffer_pos >= buffer_size) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);
        }
        buffer[buffer_pos] = c;
        c = getc(file_ptr);
        buffer_pos++;
    }
    fclose(file_ptr);
    free(buffer);
    // end file reading logic
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s file1.csv file2.csv...\n", argv[0]);
    }

    printf("You provided %d CSV file(s):\n", argc-1);

    linked_list_t *people_list = init_list();
    bst_t *people_bst = init_bst(compare_person_by_id);
    redblack_tree_t *people_rbt = init_rbt(compare_person_by_id);

    for(int i = 1; i < argc; i++) {
        char *filepath = argv[i];

        char *extension = strrchr(filepath,'.');
        if (extension && strcmp(extension,".csv") == 0) {
            printf("Processing file %d: %s\n",i,filepath);
            //read_file_into_list(filepath, people_list);
            read_file_into_bst(filepath, people_bst);
            read_file_into_rbt(filepath, people_rbt);
        }
        else {
            printf("Skipping file %d: %s not a .csv file.\n", i, filepath);
        }
    }

    printf("Here's the list I came up with: \n");
    print_rbt(people_rbt);

    int id_num = 1452;
    printf("I'm going to remove person with id number %d\n",id_num);
    bst_remove(people_bst, &id_num);
    print_bst(people_bst);

    id_num = 3900;
    printf("I'm going to remove person with id number %d\n",id_num);
    bst_remove(people_bst, &id_num);
    print_bst(people_bst);

    free_bst(people_bst);

 /*    printf("Here's the list I came up with: \n");
    print_list(people_list);

    printf("Now I'm gonna sort the list by age.\n");
    merge_sort(people_list, compare_person_by_age);

    printf("Here's the sorted list I came up with: \n");
    print_list(people_list);

    printf("Now I'm gonna sort the list by id.\n");
    merge_sort(people_list, compare_person_by_id);

    printf("Here's the sorted list I came up with: \n");
    print_list(people_list);

    free_list(people_list); */

    return 0;
}