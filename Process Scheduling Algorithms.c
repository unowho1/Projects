#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
struct node
{
    char name[5];
    int intime;
    int burst;
    int init_burst;
    struct node *next;
    struct node *prev;
} *l, *r;
struct prio_node
{
    char name[5];
    int intime;
    int burst;
    int init_burst;
    int priority;
    struct prio_node *next;
    struct prio_node *prev;
} *lp, *rp;
struct process
{
    char name[20];
    int arrival;
    int burst;
    struct process *next;
} *head = NULL, *temp, *prev;
void insert(struct node *new)
{
    if (l == NULL)
    {
        l = new;
        r = new;
        l->next = NULL;
        l->prev = NULL;
    }
    else if (new->intime < l->intime)
    {
        l->prev = new;
        new->next = l;
        new->prev = NULL;
        l = new;
    }
    else
    {
        struct node *temp = l;
        bool flag = false;
        while (temp != NULL)
        {
            if (temp->intime > new->intime)
            {
                new->prev = temp->prev;
                new->next = temp;
                temp->prev->next = new;
                temp->prev = new;
                flag = true;
                break;
            }
            temp = temp->next;
        }
        if (flag == 0)
        {
            r->next = new;
            new->next = NULL;
            new->prev = r;
            r = new;
        }
    }
}
void insert_fcfs(struct process *p)
{
    if (head == NULL)
    {
        head = p;
        head->next = NULL;
    }
    else
    {
        temp = head;
        prev = NULL;
        while (p->arrival > temp->arrival && temp->next != NULL)
        {
            prev = temp;
            temp = temp->next;
        }
        if (p->arrival >= temp->arrival)
        {
            p->next = temp->next;
            temp->next = p;
        }
        else
        {
            if (temp == head)
                p->next = temp;
            else
            {
                prev->next = p;
                p->next = temp;
            }
        }
    }
}
void insert_prio(struct prio_node *new)
{
    if (lp == NULL)
    {
        lp = new;
        rp = new;
        lp->next = NULL;
        lp->prev = NULL;
    }
    else if (new->intime < lp->intime)
    {
        lp->prev = new;
        new->next = lp;
        new->prev = NULL;
        lp = new;
    }
    else
    {
        struct prio_node *temp = lp;
        bool flag = false;
        while (temp != NULL)
        {
            if (temp->intime > new->intime)
            {
                new->prev = temp->prev;
                new->next = temp;
                temp->prev->next = new;
                temp->prev = new;
                flag = true;
                break;
            }
            temp = temp->next;
        }
        if (flag == 0)
        {
            rp->next = new;
            new->next = NULL;
            new->prev = rp;
            rp = new;
        }
    }
}
struct node *check_srtf(int *time, int *next_time)
{
    struct node *min = NULL;
    struct node *temp = l;
    *next_time = INT_MAX;
    int m = INT_MAX;
    while (temp != NULL)
    {
        if (temp->intime <= *time && temp->burst < m)
        {
            m = temp->burst;
            min = temp;
        }
        if (temp->intime - (*time) > 0 && temp->intime - (*time) < (*next_time))
            (*next_time) = temp->intime;
        temp = temp->next;
    }
    return min;
}
struct node *check_sjf(int *time)
{
    struct node *min = NULL;
    struct node *temp = l;
    int m = INT_MAX;
    while (temp != NULL)
    {
        if (temp->intime <= *time && temp->burst < m)
        {
            m = temp->burst;
            min = temp;
        }
        temp = temp->next;
    }
    return min;
}
struct node *check_lrtf(int *time)
{
    struct node *max = NULL;
    struct node *temp = l;
    int m = INT_MIN;
    while (temp != NULL)
    {
        if (temp->intime <= *time && temp->burst > m)
        {
            m = temp->burst;
            max = temp;
        }
        temp = temp->next;
    }
    return max;
}
struct prio_node *check_prio(int *time, int *next_time)
{
    struct prio_node *max = NULL;
    struct prio_node *temp = lp;
    *next_time = INT_MAX;
    int m = INT_MIN;
    while (temp != NULL)
    {
        if (temp->intime <= *time && temp->priority > m)
        {
            m = temp->priority;
            max = temp;
        }
        if (temp->intime - (*time) > 0 && temp->intime - (*time) < (*next_time))
            (*next_time) = temp->intime;
        temp = temp->next;
    }
    return max;
}
struct node *next_rr(int *time, struct node *running)
{
    struct node *temp = running;
    if (running == NULL)
        return l;
    temp = temp->next;
    do
    {
        if (temp == NULL)
            temp = l;
        if (temp->intime <= *time)
        {
            return temp;
        }
        temp = temp->next;
    } while (temp != running->next);
    return NULL;
}
void update_prio(struct prio_node *temp, int *tt, int *count, int *wt, int *cyc)
{
    temp->burst -= 1;
    if (temp->burst == 0)
    {
        (*count)++;
        (*tt) += (*cyc) - temp->intime;
        (*wt) += (*cyc) - temp->intime - temp->init_burst;
        if (lp == rp)
        {
            lp = NULL;
            rp = NULL;
        }
        else if (temp == lp)
        {
            lp = temp->next;
            lp->prev = NULL;
        }
        else if (temp == rp)
        {
            rp = rp->prev;
            rp->next = NULL;
        }
        else
            temp->prev->next = temp->next;
    }
}
void update(struct node *temp, int *tt, int *count, int *wt, int *cyc)
{
    temp->burst -= 1;
    if (temp->burst == 0)
    {
        (*count)++;
        (*tt) += (*cyc) - temp->intime;
        (*wt) += (*cyc) - temp->intime - temp->init_burst;
        if (l == r)
        {
            l = NULL;
            r = NULL;
        }
        else if (temp == l)
        {
            l = temp->next;
            if(l!=NULL)l->prev = NULL;
        }
        else if (temp == r)
        {
            r = r->prev;
            if(r!=NULL)r->next = NULL;
        }
        else
            temp->prev->next = temp->next;
    }
}
void update_rr(struct node *temp, int *tt, int *count, int *wt, int *cyc, int *fixedtime)
{
    temp->burst -= *fixedtime;
    if (temp->burst < 0)
        temp->burst = 0;
    if (temp->burst == 0)
    {
        // printf("%s\n",temp->name);
        (*count)++;
        (*tt) += (*cyc) - temp->intime;
        (*wt) += (*cyc) - temp->intime - temp->init_burst;
        if (l == r)
        {
            l = NULL;
            r = NULL;
        }
        else if (temp == l)
        {
            l = temp->next;
            if(l!=NULL)l->prev = NULL;
        }
        else if (temp == r)
        {
            r = r->prev;
            if(r!=NULL)r->next = NULL;
        }
        else
            temp->prev->next = temp->next;
    }
}
int fcfs()
{
    FILE *f1, *f2;
    int arr, bur;
    struct process *new;
    int count = 0;
    char n[20];
    f1 = fopen("processes.txt", "r");
    f2 = fopen("Result.txt", "a+");
    fprintf(f2, "\nFCFS :\nGantt Chart : ");
    while (fscanf(f1, "%d %d %s", &arr, &bur, n) != EOF)
    {
        new = (struct process *)malloc(sizeof(struct process));
        strcpy(new->name, n);
        new->arrival = arr;
        new->burst = bur;
        insert_fcfs(new);
        count++;
    }
    fclose(f1);
    temp = head;
    float tt = 0, turnaround = 0, waiting = 0;
    while (temp != NULL)
    {
        tt += temp->burst;
        for (int i = 0; i < temp->burst; i++)
        {
            printf("%s ", temp->name);
            fprintf(f2, "%s ", temp->name);
        }
        turnaround += tt - temp->arrival;
        waiting += tt - temp->arrival - temp->burst;
        temp = temp->next;
    }
printf("\nAverage turnaround time: %.2f\nAverage waiting time: %.2f\n",turnaround / count, waiting / count);
 fprintf(f2,"\nAverage turnaround time: %.2f\nAverage waiting time: %.2f\n",turnaround/count,waiting/count);
 fclose(f2);
}
int sjf()
{
 FILE *f1;
 int in, bur, cyc = 0, count = 0, tt = 0, wt = 0;
 struct node *new, *running = NULL;
 char n[5];
 char **gant;
 f1 = fopen("processes.txt", "r");
 while (fscanf(f1, "%d %d %s", &in, &bur, n) != EOF)
 {
     new = (struct node *)malloc(sizeof(struct node));
     new->intime = in;
     new->burst = bur;
     new->init_burst = bur;
     strcpy(new->name, n);
     insert(new);
 }
 fclose(f1);
 gant = (char **)malloc(sizeof(char *));
 gant[cyc] = (char *)malloc(sizeof(char) * 5);
 running = check_sjf(&cyc);
 while (l != NULL)
 {
     gant = realloc(gant, sizeof(char *) * (cyc + 1));
     gant[cyc] = (char *)malloc(sizeof(char) * 5);
     if (running == NULL || running->burst == 0)
     {
         running = check_sjf(&cyc);
     }
     if (running != NULL)
     {
         strcpy(gant[cyc], running->name);
         update(running, &tt, &count, &wt, &cyc);
     }
     else
         strcpy(gant[cyc], "\0");
     cyc++;
 }
 for (int i = 0; i < cyc; i++)
 {
     printf("%s ", gant[i]);
 }
 printf("\nAverage turnaround time : %.2f", ((float)tt / count));
 printf("\nAverage waiting time : %.2f", ((float)wt / count));
 f1 = fopen("Result.txt", "a+");
 fprintf(f1, "\nSJF :\nGantt Chart : ");
 for (int i = 0; i < cyc; i++)
 {
     fprintf(f1, "%s ", gant[i]);
 }
 fprintf(f1, "\n");
 fprintf(f1, "Average turnaround time : %.2f\n", ((float)tt / count));
 fprintf(f1, "Average waiting time : %.2f\n", ((float)wt / count));
 fclose(f1);
 return 0;
}
int rr()
{
 FILE *f1;
 int in, bur, cyc = 0, count = 0, tt = 0, wt = 0, fixedtime, ctr = 0;
 struct node *new, *running = NULL;
 char n[5];
 char **gant;
 f1 = fopen("processes.txt", "r");
 while (fscanf(f1, "%d %d %s", &in, &bur, n) != EOF)
 {
     new = (struct node *)malloc(sizeof(struct node));
     new->intime = in;
     new->burst = bur;
     new->init_burst = bur;
     strcpy(new->name, n);
     insert(new);
 }
 printf("Enter a fixed time cycle for each process : ");
 scanf("%d", &fixedtime);
 fclose(f1);
 gant = (char **)malloc(sizeof(char *));
 gant[ctr] = (char *)malloc(sizeof(char) * 5);
 while (l != NULL)
 {
     gant = realloc(gant, sizeof(char *) * (ctr + 1));
     gant[ctr] = (char *)malloc(sizeof(char) * 5);
     running = next_rr(&cyc, running);
     if (running != NULL)
         strcpy(gant[ctr], running->name);
     else
         strcpy(gant[ctr], "\0");
     update_rr(running, &tt, &count, &wt, &cyc, &fixedtime);
     cyc += fixedtime;
     ctr++;
 }
 for (int i = 0; i < ctr; i++)
 {
     printf("%s ", gant[i]);
 }
 printf("\nAverage turnaround time : %.2f", ((float)tt / count));
 printf("\nAverage waiting time : %.2f", ((float)wt / count));
 f1 = fopen("Result.txt", "a+");
 fprintf(f1, "\nRR :\nGantt Chart : ");
 printf("%d\n", cyc);
 for (int i = 0; i < ctr; i++)
 {
     fprintf(f1, "%s ", gant[i]);
     printf("%d ", i);
 }
 fprintf(f1, "\n");
 fprintf(f1, "Average turnaround time : %.2f\n", ((float)tt / count));
 fprintf(f1, "Average waiting time : %.2f\n", ((float)wt / count));
 fclose(f1);
 return 0;
}
int srtf()
{
 FILE *f1;
 int in, bur, cyc = 0, next_time = INT_MAX, count = 0, tt = 0, wt = 0;
 struct node *new, *running = NULL;
 char n[5];
 char **gant;
 f1 = fopen("processes.txt", "r");
 while (fscanf(f1, "%d %d %s", &in, &bur, n) != EOF)
 {
     new = (struct node *)malloc(sizeof(struct node));
     new->intime = in;
     new->burst = bur;
     new->init_burst = bur;
     if (next_time > in)
         next_time = in;
     strcpy(new->name, n);
     insert(new);
 }
 fclose(f1);
 gant = (char **)malloc(sizeof(char *));
 gant[cyc] = (char *)malloc(sizeof(char) * 5);
 while (l != NULL)
 {
     gant = realloc(gant, sizeof(char *) * (cyc + 1));
     gant[cyc] = (char *)malloc(sizeof(char) * 5);
     if (next_time == cyc || (running != NULL && running->burst == 0))
     {
         running = check_srtf(&cyc, &next_time);
     }
     if (running != NULL)
     {
         strcpy(gant[cyc], running->name);
         update(running, &tt, &count, &wt, &cyc);
     }
     else
         strcpy(gant[cyc], "\0");
     cyc++;
 }
 for (int i = 0; i < cyc; i++)
 {
     printf("%s ", gant[i]);
 }
 printf("\nAverage turnaround time : %.2f", ((float)tt / count));
 printf("\nAverage waiting time : %.2f", ((float)wt / count));

 f1 = fopen("Result.txt", "a+");
 fprintf(f1, "\nSRTF :\nGantt Chart : ");
 for (int i = 0; i < cyc; i++)
 {
     fprintf(f1, "%s ", gant[i]);
 }
 fprintf(f1, "\n");
 fprintf(f1, "Average turnaround time : %.2f\n", ((float)tt / count));
 fprintf(f1, "Average waiting time : %.2f\n", ((float)wt / count));
 fclose(f1);
 return 0;
}
int ps()
{
 FILE *f1;
 int in, bur, p, cyc = 0, next_time = INT_MAX, count = 0, tt = 0, wt = 0;
 struct prio_node *new, *running;
 char n[5];
 char **gant;
 f1 = fopen("processes_p.txt", "r");
 while (fscanf(f1, "%d %d %d %s", &in, &bur, &p, n) != EOF)
 {
     new = (struct prio_node *)malloc(sizeof(struct prio_node));
     new->intime = in;
     new->burst = bur;
     new->init_burst = bur;
     new->priority = p;
     if (next_time > in)
         next_time = in;
     strcpy(new->name, n);
     insert_prio(new);
 }
 fclose(f1);
 gant = (char **)malloc(sizeof(char *));
 gant[cyc] = (char *)malloc(sizeof(char) * 5);
 running = check_prio(&cyc, &next_time);
 while (lp != NULL)
 {
     gant = realloc(gant, sizeof(char *) * (cyc + 1));
     gant[cyc] = (char *)malloc(sizeof(char) * 5);
     if (next_time == cyc || (running != NULL && running->burst == 0))
     {
         running = check_prio(&cyc, &next_time);
     }
     if (running != NULL)
     {
         strcpy(gant[cyc], running->name);
         update_prio(running, &tt, &count, &wt, &cyc);
     }
     else
         strcpy(gant[cyc], "\0");
     cyc++;
 }
 for (int i = 0; i < cyc; i++)
 {
     printf("%s ", gant[i]);
 }
 printf("\nAverage turnaround time : %.2f", ((float)tt / count));
 printf("\nAverage waiting time : %.2f", ((float)wt / count));
 f1 = fopen("Result.txt", "a+");
 fprintf(f1, "\nPS :\nGantt Chart : ");
 for (int i = 0; i < cyc; i++)
 {
     fprintf(f1, "%s ", gant[i]);
 }
 fprintf(f1, "\n");
 fprintf(f1, "Average turnaround time : %.2f\n", ((float)tt / count));
 fprintf(f1, "Average waiting time : %.2f\n", ((float)wt / count));
 fclose(f1);
 return 0;
}
int lrtf()
{
 FILE *f1;
 int in, bur, cyc = 0, count = 0, tt = 0, wt = 0;
 struct node *new, *running;
 char n[5];
 char **gant;
 f1 = fopen("processes.txt", "r");
 while (fscanf(f1, "%d %d %s", &in, &bur, n) != EOF)
 {
     new = (struct node *)malloc(sizeof(struct node));
     new->intime = in;
     new->burst = bur;
     new->init_burst = bur;
     strcpy(new->name, n);
     insert(new);
 }
 fclose(f1);
 gant = (char **)malloc(sizeof(char *));
 gant[cyc] = (char *)malloc(sizeof(char) * 5);
 while (l != NULL)
 {
     gant = realloc(gant, sizeof(char *) * (cyc + 1));
     gant[cyc] = (char *)malloc(sizeof(char) * 5);
     running = check_lrtf(&cyc);
     if (running != NULL)
     {
         strcpy(gant[cyc], running->name);
         update(running, &tt, &count, &wt, &cyc);
     }
     else
         strcpy(gant[cyc], "\0");
     cyc++;
 }
 for (int i = 0; i < cyc; i++)
 {
     printf("%s ", gant[i]);
 }
 printf("\nAverage turnaround time : %.2f", ((float)tt / count));
 printf("\nAverage waiting time : %.2f", ((float)wt / count));
 f1 = fopen("Result.txt", "a+");
 fprintf(f1, "\nLRTF :\nGantt Chart : ");
 for (int i = 0; i < cyc; i++)
 {
     fprintf(f1, "%s ", gant[i]);
 }
 fprintf(f1, "\n");
 fprintf(f1, "Average turnaround time : %.2f\n", ((float)tt / count));
 fprintf(f1, "Average waiting time : %.2f\n", ((float)wt / count));
 fclose(f1);
 return 0;
}
int main()
{
 int choice;
 FILE *f1 = fopen("Result.txt", "w");
 fclose(f1);
 while (1)
 {
     printf("\n1.FCFS 2.SJF 3.RR 4.PS 5.SRTF 6.LRTF 7.Exit\n");
     printf("Enter choice : ");
     scanf("%d", &choice);
     if (choice == 1)
     {
         printf("\n");
         fcfs();
         printf("\n");
     }
     else if (choice == 2)
     {
         printf("\n");
         sjf();
         printf("\n");
     }
     else if (choice == 3)
     {
         printf("\n");
         rr();
         printf("\n");
     }
     else if (choice == 4)
     {
         printf("\n");
         ps();
         printf("\n");
     }
     else if (choice == 5)
     {
         printf("\n");
         srtf();
         printf("\n");
     }
     else if (choice == 6)
     {
         printf("\n");
         lrtf();
         printf("\n");
     }
     else if (choice == 7)
     {
         break;
     }
     else
     {
         printf("\nINVALID INPUT");
     }
 }
}