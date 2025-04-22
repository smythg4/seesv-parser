#pragma once

typedef enum Gender {
    MALE,
    FEMALE,
} gender_t;

typedef struct Person {
    int id;
    int age;
    char *last_name;
    char *first_name;
    int height;
    int weight;
    gender_t gender;
} person_t;

char *get_gender_output(person_t pers) {
    switch( pers.gender ) {
        case MALE:
            return "Male";
            break;
        case FEMALE:
            return "Female";
            break;
        default:
            return "Unspecified";
    }
}

void print_person(person_t pers) {
    printf("%d: %s, %s.\t\tGender: %s.\tAge: %d.\tHeight: %d cm.\tWeight %d kg.\n",pers.id, pers.last_name, pers.first_name, get_gender_output(pers),pers.age, pers.height, pers.weight);
}

void free_person(person_t *person) {
    free(person->first_name);
    free(person->last_name);
}

// helper functions for the linkedlist module
void free_person_data(void *data) {
    person_t *person = (person_t *)data;
    free(person->first_name);
    free(person->last_name);
    free(person);
}

void print_person_data(void *data) {
    person_t *person = (person_t *)data;
    print_person(*person);
}

int compare_person_by_id(void *data1, void *data2) {
    person_t *p1 = (person_t *)data1;
    person_t *p2 = (person_t *)data2;

    return (p1->id)-(p2->id);
}

int compare_person_by_age(void *data1, void *data2) {
    person_t *p1 = (person_t *)data1;
    person_t *p2 = (person_t *)data2;

    return (p1->age)-(p2->age);
}