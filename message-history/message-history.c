#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct message
{
    char date[11]; 
    char time[9]; 
    char chatClient[22];
    char protocol[22];
    char userName[50];
    char message[250];
};
typedef struct message message;

struct node
{
    message msg;
    struct node *next;
};
typedef struct node node;

struct list
{
    node *root;
    int count;
};
typedef struct list list;

void print_list(list *l)
{
    node *temp = l->root;
    while(temp != NULL)
    {
        printf("[%s %s] [%s.%s] %s: %s\n", 
            temp->msg.date,
            temp->msg.time,
            temp->msg.chatClient,
            temp->msg.protocol,
            temp->msg.userName,
            temp->msg.message);

        temp = temp->next;
    }
}

/*
    print_specif(list *l, char *curentDate)
    {
        node *temp = l->root;
        while(temp != NULL)
        {
            //check date
            int dateOk = 1;
            int indexs[6] = {6, 9, 3, 4, 0, 1};
            //dd.mm.yyyy
            for(int j = 0; j < 6; j+=2)
                for(int i = indexs[j]; i <= indexs[j+1]; i++)                
                    if(temp->val.date[i] < currentDate[i])
                        printf("...\n", ...);

            temp = temp->next;
        }
    }
*/

void remove_unwanted_chars(char *str)
{
    int len = strlen(str);

    for(int i = 0; i < len; i++)
    {
        if(str[i] == '[' || str[i] == ']' )
            str[i] = ' ';
    }
}

void segmentate(char *str, message *msg)
{    
    remove_unwanted_chars(str);

    sscanf(str, "%s %s %[^.] %*1s %s %[^:] %*1s %250[^\n]",
        msg->date, 
        msg->time,
        msg->chatClient, 
        msg->protocol, 
        msg->userName, 
        msg->message);
}

list *readTXT()
{
    FILE *fp;
    fp = fopen("chat.txt", "r");
    if(fp == NULL)
    {
        perror("Cannot open file");
        exit(1);
    }

    list *history = (list*) malloc(sizeof(list));

    node *bef = NULL;
    node *temp = NULL;
    int count = 0;
    char buffer[370];                       

    while(fgets(buffer, 370, fp)) 
    {
        if(count == 0)
        {    
            history->root = (node*) malloc(sizeof(node));
            segmentate(buffer, &history->root->msg);
            bef = history->root;
            count++;
            continue;
        }

        temp = (node*) malloc(sizeof(node));
        segmentate(buffer, &temp->msg);

        bef->next = temp;
        bef = bef->next;
        count++;
    }

    fclose(fp);

    history->count = count;
    return history;
}

list *readBIN()
{
    FILE *fp;
    fp = fopen("history.db", "rb");
    if(fp == NULL)
    {
        perror("Cannot open file");
        exit(1);
    }

    message m;    
    list *history = (list*) malloc(sizeof(list));
    node *temp = NULL;
    node *bef = NULL;
    int count = 0;    
    while(fread(&m, sizeof(message), 1, fp))
    {        
        if(count == 0)
        {
            history->root = (node*) malloc(sizeof(node));
            history->root->msg = m;
            bef = history->root;
            count++;
            continue;
        }

        temp = (node*) malloc(sizeof(node));
        temp->msg = m;

        bef->next = temp;
        bef = bef->next;
        count++;
    }

    fclose(fp);

    history->count = count;
    return history;
}

void writeBIN(list *l)
{    
    FILE *fp;
    fp = fopen("history.db", "wb");
    if(fp == NULL)
    {
        perror("Cannot open file");
        exit(1);
    }

    node *temp = l->root;
    int count = 0;    
    while(temp != NULL)
    {
        if(fwrite(&temp->msg, sizeof(message), 1, fp) != 1)
        {
            perror("Cannot write to file");
            exit(1);
        }
        temp = temp->next;
    }

    fclose(fp);
}

int delete_user_history(list *l, char *userName)
{
    node *temp = l->root;
    node *bef = NULL;
    int del = 0;    
    while(temp != NULL)
    {
        if(strcmp(temp->msg.userName, userName) == 0)
        {
            del++;
            
            if(temp == l->root)
            {                
                l->root = temp->next;
                
                free(temp);                
                temp = l->root;
                
                continue;
            }
                        
            bef->next = temp->next;            
            free(temp);            
            temp = bef->next;
        }        
        else
        {
            bef = temp;
            temp = temp->next;
        }
    }
    l->count -= del;
    
    if(del == 0)
        printf("No such user\n");
    return del;
}

void guess_mood(list *l, char *userName)
{
    node *temp = l->root;    
    int userExist = 0;
    float happy = 0, sad = 0;    
    while(temp != NULL)
    {        
        if(strcmp(temp->msg.userName, userName) == 0)
        {            
            userExist = 1;            
            char c[250];
            strcpy(c, temp->msg.message);            
            for(int i = 0; i < strlen(c); i++)
            {                
                if(c[i] == ':' || c[i] == '=')
                {                    
                    if(c[i+1] == ')' || c[i+1] == 'D' ||
                    c[i+1] == ']' || c[i+1] == 'P')
                        happy++;
                                            
                    if(c[i+1] == '(')
                        sad++;
                                            
                    if(c[i+1] == '\'' && c[i+2] == '(')
                        sad++;
                    
                }
            }
        }
        temp = temp->next;
    }
    
    if(userExist)
    {
        char mood[10];
    
        if(sad == 0)
        {
            sad = 1;
            happy++;
        }    
        
        float coef =  happy / sad;
        if(coef < 0.5)            
            strcpy(mood, "sad");
        else if(coef >= 0.5 && coef <= 1.5)            
            strcpy(mood, "neutral");
        else if(coef > 1.5)            
            strcpy(mood, "happy");
        
        printf("User %s is %s\n", userName, mood);
    }    
    else
        printf("No such user\n");
}

void most_messages(list *l)
{
    int count = 0;    
    char **users = NULL; //users[l->count][50];

    node *temp = l->root;    
    while(temp != NULL)
    {        
        int isNew = 1;
        
        for(int i = 0; i < count; i++)
        {            
            if(strcmp(temp->msg.userName, users[i]) == 0)
            {
                isNew = 0;
                break;
            }
        }        
        if(isNew)
        {            
            // ненужно v
            char **curr = (char**) realloc(users, (count+1)*sizeof(char*));
            if(curr != NULL)
                users = curr;            
            users[count] = (char*) malloc(50*sizeof(char)); 
            //при статик users е ненужно ^           
            strcpy(users[count], temp->msg.userName);
            count++;
        }
        temp = temp->next;
    }
    
    // ненужно v
    int *messagesUser = (int*) calloc(count, sizeof(int));
    for(int i = 0; i < count; i++)
    {
        temp = l->root;
        while(temp != NULL)
        {
            if(strcmp(users[i], temp->msg.userName) == 0)
                messagesUser[i]++;
            temp = temp->next;
        }
    }
    
    int max = 0;
    for(int i = 0; i < count; i++)
        if(max < messagesUser[i])
            max = messagesUser[i];

    for(int i = 0; i < count; i++)
        if(messagesUser[i] == max)
            printf("%s wrote %d messages\n", users[i], messagesUser[i]);
    //ненужно ^
    /*
        //намираме maх duration
        int max = 0;
        for(int i = 0; i < count; i++)
            if(max < users[count].<каквото и да е >)
                max = users[count].<...>
        
        //print user with max 
        for(int i = 0; i < count; i++)
            if(max == users[count].<каквото и да е >)
            {
                printf("%s\n", users[count].name);
                break;
            }
        
    */
    free(users);
    free(messagesUser);
}

int longest_message(list *l)
{
    int max = 0;
    node *temp = l->root;
    
    
    while(temp != NULL)
    {
        int len = strlen(temp->msg.message);
        if(len > max)
            max = len;
        temp = temp->next;
    }

    temp = l->root;
    
    while(temp != NULL)
    {
        int len = strlen(temp->msg.message);
        if(len == max)
        {
            printf("%s: %s -> %d characters\n", 
                temp->msg.userName, 
                temp->msg.message,
                len);
        }
        temp = temp->next;
    }
}



int main()
{
    list *l = readTXT();

    if(l->root == NULL)
    {
        printf("No chat history data.\n");
        return 0;
    }
    
    writeBIN(l);
    free(l);

    list *l2 = readBIN();

    int option = 0;
    
    while(1)
    {
        
        printf("1. Print chat history\n");
        printf("2. Delete chat hostory for user\n");
        printf("3. Find user's mood\n");
        printf("4. Most messages\n");
        printf("5. Longest message\n");
        printf("6. Exit\n\n");

        printf("Choose option: ");
        scanf("%d", &option);

        char userName[50];
        
        switch(option)
        {
            case 1:
                printf("\n");
                
                print_list(l2);
                printf("\n");
                break;
            case 2:
                printf("\n");
                printf("Enter username to delete his char history: ");
                scanf("%s", userName);
                
                int del = delete_user_history(l2, userName);
                
                if(del != 0)
                {
                    printf("%d messages deleted\n", del);
                    print_list(l2);
                }
                printf("\n");
                
                writeBIN(l2);
                break;
            case 3:
                printf("\n");
                printf("Enter username to find his mood: ");
                scanf("%s", userName);
                
                guess_mood(l2, userName);
                printf("\n");
                break;
            case 4:
                
                most_messages(l2);
                printf("\n");
                break;
            case 5:
                
                longest_message(l2);
                printf("\n");
                break;
            case 6:
                
                return 0;
            default:
                printf("Choose between 1 and 5\n");
                break;
        }
    }

    return 0;
}