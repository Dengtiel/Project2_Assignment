/*
 * Project 1: Dynamic Student Record Engine
 * Student Email: d.akol@alustudent.com
 * Analysis Function: Grade Stability Score
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure Definition */
typedef struct {
    int   id;
    char  name[50];
    float grade;
} Student;

/* Global State */
Student *students = NULL;   /* dynamically allocated array */
int      count    = 0;      /* current number of students  */
int      capacity = 0;      /* allocated slots             */

/* Add a student (grows array dynamically) */
void add_student(void) {
    if (count == capacity) {
        capacity = (capacity == 0) ? 4 : capacity * 2;
        Student *tmp = realloc(students, capacity * sizeof(Student));
        if (!tmp) { fprintf(stderr, "Memory allocation failed.\n"); return; }
        students = tmp;
    }

    Student *s = &students[count];      /* pointer to new slot */
    s->id = count + 1;

    printf("  Enter name  : ");
    scanf(" %49[^\n]", s->name);
    printf("  Enter grade (0-100): ");
    scanf("%f", &s->grade);

    if (s->grade < 0 || s->grade > 100) {
        printf("  Invalid grade. Student not added.\n");
        return;
    }
    count++;
    printf("  Student added (ID %d).\n", s->id);
}

/* Display all students using pointer traversal */
void display_students(void) {
    if (count == 0) { printf("  No students on record.\n"); return; }

    printf("\n  %-4s  %-25s  %s\n", "ID", "Name", "Grade");
    printf("  %s\n", "----  -------------------------  -----");

    Student *p = students;   /* pointer traversal */
    for (int i = 0; i < count; i++, p++) {
        printf("  %-4d  %-25s  %.2f\n", p->id, p->name, p->grade);
    }
    printf("\n");
}

/* Bubble-sort students by grade (descending) */
void sort_students(void) {
    if (count < 2) { printf("  Not enough students to sort.\n"); return; }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (students[j].grade < students[j + 1].grade) {
                Student tmp     = students[j];
                students[j]     = students[j + 1];
                students[j + 1] = tmp;
            }
        }
    }
    /* re-assign sequential IDs after sort */
    for (int i = 0; i < count; i++) students[i].id = i + 1;
    printf("  Students sorted by grade (highest first).\n");
}

/* Calculate and print average grade */
void calculate_average(void) {
    if (count == 0) { printf("  No students on record.\n"); return; }

    float sum = 0;
    Student *p = students;
    for (int i = 0; i < count; i++, p++) sum += p->grade;

    printf("  Average grade: %.2f\n", sum / count);
}

/* Find and display the top student */
void find_top_student(void) {
    if (count == 0) { printf("  No students on record.\n"); return; }

    Student *top = students;
    Student *p   = students + 1;
    for (int i = 1; i < count; i++, p++) {
        if (p->grade > top->grade) top = p;
    }
    printf("  Top student: %s  (Grade: %.2f)\n", top->name, top->grade);
}

/*
 * Custom Analysis: Grade Stability Score
 * Measures how evenly spread the grades are.
 * Score = 100 - standard deviation.
 * Higher score means grades are tightly clustered (stable class).
 * Lower score means grades are widely spread.
 */
void grade_stability_score(void) {
    if (count < 2) { printf("  Need at least 2 students.\n"); return; }

    /* calculate mean */
    float sum = 0;
    Student *p = students;
    for (int i = 0; i < count; i++, p++) sum += p->grade;
    float mean = sum / count;

    /* calculate variance */
    float var = 0;
    p = students;
    for (int i = 0; i < count; i++, p++) {
        float diff = p->grade - mean;
        var += diff * diff;
    }
    var /= count;

    /* square root approximation without math.h */
    float std_dev = 0;
    float x = var, x1 = var / 2.0f;
    if (var > 0) {
        while (x - x1 > 0.0001f) { x = x1; x1 = (x + var / x) / 2.0f; }
        std_dev = x1;
    }

    float stability = 100.0f - std_dev;
    if (stability < 0) stability = 0;

    printf("\n  --- Grade Stability Score ---\n");
    printf("  Mean grade      : %.2f\n", mean);
    printf("  Std Deviation   : %.2f\n", std_dev);
    printf("  Stability Score : %.2f / 100\n", stability);
    if      (stability >= 85) printf("  Status: Highly Stable Class\n");
    else if (stability >= 65) printf("  Status: Moderately Stable Class\n");
    else                       printf("  Status: High Grade Variation\n");
    printf("\n");
}

/* Function-pointer type and menu table */
typedef void (*Operation)(void);

typedef struct {
    const char *label;
    Operation   fn;
} MenuItem;

/* Pre-load sample dataset */
void load_sample_data(void) {
    const char *names[] = {
        "Alice Mwangi", "Brian Odhiambo", "Clara Mayen", "David Thon",
        "Achol Achieng",  "Frank David",    "Grace Wanjiku","Henry Majok",
        "Akol Chol", "James Jok",    "Karen Nduta",  "Liam Otieno",
        "Mary Deng",  "Noel Simiyu",    "Olivia Kariuki"
    };
    float grades[] = {78, 85, 62, 91, 74, 55, 88, 67, 93, 80, 71, 84, 59, 76, 95};
    int n = 15;

    capacity = n;
    students = malloc(capacity * sizeof(Student));
    if (!students) { fprintf(stderr, "Memory error.\n"); exit(1); }

    for (int i = 0; i < n; i++) {
        students[i].id    = i + 1;
        strcpy(students[i].name, names[i]);   /* strcpy demonstrated */
        students[i].grade = grades[i];
    }
    count = n;
    printf("  Sample dataset of %d students loaded.\n\n", count);
}

int main(void) {
    printf("========================================\n");
    printf("  Dynamic Student Record Engine\n");
    printf("  Student ID: 1\n");
    printf("  Analysis  : Grade Stability Score\n");
    printf("========================================\n\n");

    load_sample_data();

    /* build function-pointer menu table */
    MenuItem menu[] = {
        {"Add Student",           add_student          },
        {"Display Students",      display_students      },
        {"Sort Students",         sort_students         },
        {"Calculate Average",     calculate_average     },
        {"Find Top Student",      find_top_student      },
        {"Grade Stability Score", grade_stability_score },
    };
    int n_items = sizeof(menu) / sizeof(menu[0]);

    int choice;
    do {
        printf("--- MENU ---\n");
        for (int i = 0; i < n_items; i++)
            printf("  %d. %s\n", i + 1, menu[i].label);
        printf("  0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= n_items) {
            printf("\n");
            menu[choice - 1].fn();   /* call via function pointer */
        } else if (choice != 0) {
            printf("  Invalid option.\n");
        }
    } while (choice != 0);

    free(students);   /* release dynamic memory */
    printf("Goodbye.\n");
    return 0;
}


