#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stud_type
{
    int matnum;
    char vorname[20];
    char nachname[20];
    struct stud_type *next;
} stud_type;

typedef struct sort_data
{
    struct stud_type *value;
    struct sort_data *next;
} sort_data;

/* Ist die Datenbank leer?      */
bool is_empty(stud_type const *liste)
{
    return !liste;
}

bool enqueueOrdered0(stud_type **studenten_liste, stud_type *student, int (*compare)(stud_type *, stud_type *))
{
    if (is_empty(*studenten_liste))
    {
        *studenten_liste = student;
    }
    else
    {

        if (compare(*studenten_liste, student) == 1) // das erste element ist größer als das neue - ersetzen
        {
            student->next = *studenten_liste;
            *studenten_liste = student;
            return true;
        }
        stud_type *current = *studenten_liste;

        while (current)
        {
            if (current->next)
            {
                if (compare(current->next, student) == 1)
                { // Zwischen den Elementen einfügen - Das nächste Element ist upper bound
                    student->next = current->next;
                    current->next = student;
                    return true;
                }
            }
            else
            { // Letztes Element der Liste
                current->next = student;
                return true;
            }
            current = current->next;
        }
    }

    return false;
}
// Because I'm notoriously lazy, we'll just copy the entire method again
bool enqueueOrderedDataPoint(sort_data **list, sort_data *new_datapoint, int (*compare)(stud_type *, stud_type *))
{
    if (!(*list))
    {
        *list = new_datapoint;
    }
    else
    {

        if (compare((*list)->value, new_datapoint->value) == 1) // das erste element ist größer als das neue - ersetzen
        {
            new_datapoint->next = *list;
            *list = new_datapoint;
            return true;
        }
        sort_data *current = *list;

        while (current)
        {
            if (current->next)
            {
                if (compare(current->next->value, new_datapoint->value) == 1)
                { // Zwischen den Elementen einfügen - Das nächste Element ist upper bound
                    new_datapoint->next = current->next;
                    current->next = new_datapoint;
                    return true;
                }
            }
            else
            { // Letztes Element der Liste
                current->next = new_datapoint;
                return true;
            }
            current = current->next;
        }
    }

    return false;
}

static int compareStudentsByMatNum(stud_type *student1, stud_type *student2)
{
    if (student1->matnum > student2->matnum)
    {
        return 1;
    }
    else
    {
        return student1->matnum == student2->matnum ? 0 : -1;
    }
}

static int compareStudentsByFirstName(stud_type *student1, stud_type *student2)
{
    return strcmp(student1->vorname, student2->vorname);
}

static int compareStudentsByLastName(stud_type *student1, stud_type *student2)
{
    return strcmp(student1->nachname, student2->nachname);
}

/* Einfuegen eines Elementes
 * 
 * Bekommt einen Zeiger auf einen Zeiger der auf das 1. Element der Liste zeigt
 * Bekommt MatNr, Vorname und Nachname des Studenten der eingefügt werden soll
 *
 * Gibt true für Erfolg zurück
 * Gibt false für einen Fehler zurück
 */
bool enqueue(stud_type **studenten_liste, int matnum, char const vorname[20], char const nachname[20])
{
    /* Hole dynamischen Speicher für den neuen Listen Eintrag */
    stud_type *student = malloc(sizeof(stud_type));

    /* Befülle den Speicher (vorsicht vor buffer overflows!) */
    student->matnum = matnum;
    strcpy(student->vorname, vorname);
    strcpy(student->nachname, nachname);
    student->next = NULL;

    bool result = enqueueOrdered0(studenten_liste, student, &compareStudentsByMatNum);

    return result;

    /* Füge den neuen Eintrag in die Liste ein */

    /* Ist die Liste leer ? */

    /* Sortier den Studenten aufsteigend nach Matrikelnummer ein (matrikelnummern sind einzigartig) */
}

/* Löschen eines Elementes. 
 * 
 * Bekommt einen Zeiger auf einen Zeiger der auf das 1. Element der Liste zeigt
 * Bekommt die MatNr des Studenten der zu löschen ist
 *
 * Gibt true für Erfolg zurück
 * Gibt false für einen Fehler zurück
 */
bool dequeue(stud_type **studenten_liste, int matnum)
{
    /* Prüfe Randbedingungen */
    if (is_empty(*studenten_liste))
        return false;

    stud_type *head = *studenten_liste;

    if (!head->next && head->matnum == matnum)
    {
        *studenten_liste = NULL;
        return true;
    }

    if (head->matnum == matnum)
    {
        *studenten_liste = head->next;
        return true; // erstes element
    }

    stud_type *current = head;

    while (current)
    {
        if (current->next)
        {
            if (current->next->matnum == matnum)
            {
                stud_type *obj = current->next;
                current->next = current->next->next;

                free(obj);
                return true;
            }
            else
            {
                current = current->next;
            }
        }
        else
        {
            return false;
        }
    }

    return false;

    /* Finde den Studenten */

    /* Lösche den Studenten und gibt den Speicher frei */

    /* Was muss passieren wenn das 1. Element gelöscht wird? */
    /* Was ist wenn es nicht in der Liste ist? */
    /* ... */
}

/* Auslesen eines Elementes 
 *
 * Bekommt pointer auf den Listenstart
 * Bekommt MatNr des Studenten der ausgelesen werden soll
 *
 * Schreibt Vorname und Nachname in vorname und nachname
 */
bool get_student(stud_type const *studenten_liste, int matnum, char vorname[20], char nachname[20])
{
    /* Durchsuchen der DB */
    stud_type const *curr = studenten_liste;
    while (curr && curr->matnum < matnum)
    {
        curr = curr->next;
    }

    if (!curr || curr->matnum != matnum)
    {
        /* Rückgabewert: Fehler */
        return false;
    }
    else
    {
        strncpy(vorname, curr->vorname, 19);
        vorname[19] = '\0';
        strncpy(nachname, curr->nachname, 19);
        nachname[19] = '\0';
        /* Rückgabewert: OK */
        return true;
    }
}

static void test_empty(stud_type const *liste)
{
    printf(">>> Test ob die Studentenliste leer ist ...\n");

    if (is_empty(liste))
    {
        printf("    Die Studentenliste ist leer \n");
    }
    else
    {
        printf("    Die Studentenliste ist nicht leer \n");
    }
}

static void test_get(stud_type const *liste, int matnum)
{
    printf(">>> Test, ob die Matrikelnummer %4i bereits erfasst wurde ...\n", matnum);

    char vorname[20];
    char nachname[20];
    if (get_student(liste, matnum, vorname, nachname))
    {
        printf("    Matrikelnummer %4i: %s %s\n", matnum, vorname, nachname);
    }
    else
    {
        printf("    Matrikelnummer %4i ist unbekannt\n", matnum);
    }
}

static void test_enqueue(stud_type **liste, int matnum, char const *vorname, char const *nachname)
{
    printf(">>> Fuege neuen Studenten in die Liste ein: %s %s [%4i] ...\n", vorname, nachname, matnum);
    if (enqueue(liste, matnum, vorname, nachname))
    {
        printf("    Matrikelnummer %4i eingefügt\n", matnum);
    }
    else
    {
        printf("    Matrikelnummer %4i konnte nicht eingefügt werden\n", matnum);
    }
}

static void test_dequeue(stud_type **liste, int matnum)
{
    printf(">>> Loesche die Matrikelnummer %4i ...\n", matnum);

    if (dequeue(liste, matnum))
    {
        printf("    Matrikelnummer %4i geloescht\n", matnum);
    }
    else
    {
        printf("    Matrikelnummer %4i war nicht erfasst\n", matnum);
    }
}

static void test_dump(stud_type const *liste)
{
    printf(">>> Gebe alle erfassten Studenten aus ...\n");
    for (stud_type const *curr = liste; curr; curr = curr->next)
    {
        printf("    Matrikelnummer %4i: %s %s\n", curr->matnum, curr->vorname, curr->nachname);
    }
}

static void test_dump_ordered(sort_data const *liste)
{
    printf(">>> Gebe alle erfassten Studenten aus ...\n");
    for (sort_data const *curr = liste; curr; curr = curr->next)
    {
        printf("    Matrikelnummer %4i: %s %s\n", curr->value->matnum, curr->value->vorname, curr->value->nachname);
    }
}

static sort_data *replicateOrdered(stud_type *origin_list, int (*compare)(stud_type *, stud_type *))
{

    if (is_empty(origin_list))
        return NULL;

    sort_data *list_head = NULL;

    for (stud_type *curr = origin_list; curr; curr = curr->next)
    {
        sort_data *data_point = malloc(sizeof(sort_data));
        data_point->value = curr;
        data_point->next = NULL;

        enqueueOrderedDataPoint(&list_head, data_point, compare);
    }

    return list_head;
}

/* Test der Listenfunktionen  */
int main(void)
{
    /* Initialisierung der Datenbank */
    stud_type *studenten_liste = NULL;

    test_empty(studenten_liste);
    test_enqueue(&studenten_liste, 1234, "Eddard", "Stark");
    test_get(studenten_liste, 1234);
    test_dump(studenten_liste);
    test_enqueue(&studenten_liste, 5678, "Jon", "Snow");
    test_get(studenten_liste, 1234);
    test_enqueue(&studenten_liste, 9999, "Tyrion", "Lannister");
    test_get(studenten_liste, 1235);
    test_enqueue(&studenten_liste, 1289, "Daenerys", "Targaryen");
    test_dequeue(&studenten_liste, 1234);
    test_empty(studenten_liste);
    test_get(studenten_liste, 5678);
    test_dequeue(&studenten_liste, 9998);
    test_enqueue(&studenten_liste, 1289, "Viserys", "Targaryen");
    test_dequeue(&studenten_liste, 5678);
    test_enqueue(&studenten_liste, 1, "Tywin", "Lannister");
    test_enqueue(&studenten_liste, 2, "ABC", "Z");
    test_dump(studenten_liste);

    {
        sort_data *firstNameOrdered = replicateOrdered(studenten_liste, &compareStudentsByFirstName);
        test_dump_ordered(firstNameOrdered);
        /* Erzeuge sortierte Liste nach Vorname */
        /* Gebe Liste aus */
        /* Räume erzeugte Liste auf */
    }

    {
        sort_data *lastNameOrdered = replicateOrdered(studenten_liste, &compareStudentsByLastName);
        test_dump_ordered(lastNameOrdered);
        
        /* Erzeuge sortierte Liste nach Nachname */
        /* Gebe Liste aus */
        /* Räume erzeugte Liste auf */
    }

    /* Räume studenten_liste auf */

    return 0;
}
