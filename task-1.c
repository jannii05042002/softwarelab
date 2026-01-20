#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SUBJECTS 5
#define MAX 100

/* Structure Definition */
struct Student
{
    char id[20];
    char name[50];
    int marks[SUBJECTS];
    int total;
    float percentage;
    float cgpa;
    char grade[3];
};

/* Function Prototypes */
int isValidID(char id[], struct Student s[], int count);
int isValidName(char name[]);
int isValidMarks(int marks[]);
void calculateResult(struct Student *s);
void assignGrade(struct Student *s);
void writeHeader(FILE *fp);
void writeStudent(FILE *fp, struct Student s);
void calculateStatistics(struct Student s[], int n, FILE *fp);

int main()
{
    FILE *fin, *fout;
    struct Student s[MAX];
    int n, i;

    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    if (fin == NULL || fout == NULL)
    {
        printf("File error!\n");
        return 1;
    }

    fscanf(fin, "%d", &n);

    for (i = 0; i < n; i++)
    {
        fscanf(fin, "%s %s", s[i].id, s[i].name);

        if (!isValidID(s[i].id, s, i))
        {
            printf("Invalid or duplicate ID: %s\n", s[i].id);
            return 1;
        }

        if (!isValidName(s[i].name))
        {
            printf("Invalid Name: %s\n", s[i].name);
            return 1;
        }

        for (int j = 0; j < SUBJECTS; j++)
            fscanf(fin, "%d", &s[i].marks[j]);

        if (!isValidMarks(s[i].marks))
        {
            printf("Invalid marks for student %s\n", s[i].id);
            return 1;
        }

        calculateResult(&s[i]);
        assignGrade(&s[i]);
    }

    writeHeader(fout);

    for (i = 0; i < n; i++)
        writeStudent(fout, s[i]);

    calculateStatistics(s, n, fout);

    fclose(fin);
    fclose(fout);

    
    return 0;
}

/* ---------- Validation Functions ---------- */

int isValidID(char id[], struct Student s[], int count)
{
    for (int i = 0; id[i]; i++)
        if (!isalnum(id[i]))
            return 0;

    for (int i = 0; i < count; i++)
        if (strcmp(id, s[i].id) == 0)
            return 0;

    return 1;
}

int isValidName(char name[])
{
    for (int i = 0; name[i]; i++)
        if (!isalpha(name[i]))
            return 0;
    return 1;
}

int isValidMarks(int marks[])
{
    for (int i = 0; i < SUBJECTS; i++)
        if (marks[i] < 0 || marks[i] > 100)
            return 0;
    return 1;
}

/* ---------- Computation ---------- */

void calculateResult(struct Student *s)
{
    s->total = 0;
    for (int i = 0; i < SUBJECTS; i++)
        s->total += s->marks[i];

    s->percentage = (float)s->total / SUBJECTS;
    s->cgpa = s->percentage / 10;
}

void assignGrade(struct Student *s)
{
    float p = s->percentage;

    if (p >= 90) strcpy(s->grade, "O");
    else if (p >= 85) strcpy(s->grade, "A+");
    else if (p >= 75) strcpy(s->grade, "A");
    else if (p >= 65) strcpy(s->grade, "B+");
    else if (p >= 60) strcpy(s->grade, "B");
    else if (p >= 55) strcpy(s->grade, "C");
    else if (p >= 50) strcpy(s->grade, "D");
    else strcpy(s->grade, "F");
}

/* ---------- Output ---------- */

void writeHeader(FILE *fp)
{
    fprintf(fp, "-------------------------------------------------------------------------------------------------\n");
    fprintf(fp, "ID       Name       S1   S2   S3   S4   S5   Total  Percent  Grade  CGPA\n");
    fprintf(fp, "-------------------------------------------------------------------------------------------------\n");
}

void writeStudent(FILE *fp, struct Student s)
{
    fprintf(fp, "%-8s %-10s ", s.id, s.name);
    for (int i = 0; i < SUBJECTS; i++)
        fprintf(fp, "%-4d ", s.marks[i]);

    fprintf(fp, "%-6d %-8.2f %-5s %.2f\n",
            s.total, s.percentage, s.grade, s.cgpa);
}

/* ---------- Statistics ---------- */

void calculateStatistics(struct Student s[], int n, FILE *fp)
{
    float sum = 0, max = s[0].percentage, min = s[0].percentage;
    int gradeCount[8] = {0};

    for (int i = 0; i < n; i++)
    {
        sum += s[i].percentage;
        if (s[i].percentage > max) max = s[i].percentage;
        if (s[i].percentage < min) min = s[i].percentage;

        if (!strcmp(s[i].grade, "O")) gradeCount[0]++;
        else if (!strcmp(s[i].grade, "A+")) gradeCount[1]++;
        else if (!strcmp(s[i].grade, "A")) gradeCount[2]++;
        else if (!strcmp(s[i].grade, "B+")) gradeCount[3]++;
        else if (!strcmp(s[i].grade, "B")) gradeCount[4]++;
        else if (!strcmp(s[i].grade, "C")) gradeCount[5]++;
        else if (!strcmp(s[i].grade, "D")) gradeCount[6]++;
        else gradeCount[7]++;
    }

    fprintf(fp, "\nCLASS STATISTICS\n");
    fprintf(fp, "Class Average Percentage : %.2f\n", sum / n);
    fprintf(fp, "Highest Percentage       : %.2f\n", max);
    fprintf(fp, "Lowest Percentage        : %.2f\n", min);

    fprintf(fp, "\nGrade Distribution:\n");
    fprintf(fp, "O  : %d\nA+ : %d\nA  : %d\nB+ : %d\nB  : %d\nC  : %d\nD  : %d\nF  : %d\n",
            gradeCount[0], gradeCount[1], gradeCount[2], gradeCount[3],
            gradeCount[4], gradeCount[5], gradeCount[6], gradeCount[7]);
}

