#include <stdio.h>
#include <stdlib.h>

//структура за точка от координатна система
//една точка се състои от (x, y)
struct point
{
    int x;
    int y;
};
//пишем typedef за улеснение
//вместо всеки път да пишем 'struct point'
//пишем само 'point_t'
typedef struct point point_t;

//структура за отсечка от координатната система
//отсечка се състои от начална точка и крайна точка
struct line
{
    point_t start;
    point_t end;
};
//тук пак за улеснение
typedef struct line line_t;

//структура за ДВОЙКА отсечки (т.е. 2 отсечки)
//правим го за да ни е по-лесно
struct pair
{
    line_t first;
    line_t second;
};
//тук пак за улеснение
typedef struct pair pair_t;

//помощна функция за четене на записаните двойки отсечки от двоичен файл
//lines.bin
pair_t *read_file(int *k)
{
    FILE *fp;
    //отваряме файла за двоично писане
    //ако файла вече съществува, информацията в него се изтрива
    //и се записва тази, която ще въведем сега
    fp = fopen("lines.bin", "rb");
    //ако не сме отворили благополучно или файла не съществува
    if(fp == NULL)
    {
        //изхвърли грешка
        printf("Cannot open file");
        //и излез от функцията, NULL показва че няма файл
        return NULL;
    }

    //брояч за отброяване на броя прочетени двойки отсечки
    int count = 0;
    //структура тип двойка отсечки (pair_t) 
    //която се използва за четене от файла
    //т.е. прочетеното от файла се записва в нея
    pair_t current;
    //временен помощен масив от двойки
    pair_t *temp = NULL;
    //самият масив от двойки, който ще return-нем
    pair_t *pairs = NULL;

    //четем докато не стигнем края на файла
    //и то четем елемент по елемент
    //като всеки елемент се присвоява на current
    while(fread(&current, sizeof(pair_t), 1, fp))
    {
        //увеличаваме броя елементи в temp с 1
        //използваме temp за да предотвратим грешки или загуби
        //ако ползваме pairs, т.е. да не изгубим директен достъп до pairs
        temp = (pair_t*) realloc(pairs, (count+1)*sizeof(pair_t));
        //ако няма памет temp да се увеличава още
        if(temp == NULL)
        {
            perror("Not enough space");
            exit(1);
        }
        //присвояваме temp на pairs
        pairs = temp;
        //
        pairs[count] = current;
        //увеличаваме брояча
        count++;
    }

    fclose(fp);

    //присвояваме стойността на k да е равна на брояча count
    //k = count няма да сработи защото k е указател (pointer)
    //a *k е самата стойност, т.е. ние директно променяме стойността поадена като аргуемнт
    *k = count;
    //можем да го return-нем, тъй като се пази в heap (динамичната памет)
    //а в heap се пази докато не я free-нем ние самите
    //ако я бяхме дефинирали pair_t pairs,
    //тя щеше да се пази в stack (статична памет) и автоматично щеше да бъе изтрита
    //след края на функцията
    return pairs;
}

//помощна функция за записване на двойките отсечки в двоичен файл
//lines.bin
void write_file(pair_t *pairs, int k)
{
    FILE *fp;
    //отваряме файла за двоично писане
    //ако файла вече съществува, информацията в него се изтрива
    //и се записва тази, която ще въведем сега
    fp = fopen("lines.bin", "wb");
    //ако не сме отворили благополучно
    if(fp == NULL)
    {
        //изхвърли грешка и прекрати прогрмата
        perror("Cannot open file");
        exit(1);
    }

    //записваме всяка двойка отсечки от масива с двойки отсечки pairs,
    //който сме подали като аргумент на функцията
    //k ни е броят ДВОЙКИ отсечки
    for(int i = 0; i < k; i++)
    {
        fwrite(&pairs[i], sizeof(pair_t), 1, fp);
    }

    //затваряме файла
    fclose(fp);
}

//функция за въвеждане от потребителя дойките отсечки
pair_t *input_pair_line(int k)
{
    //помощни променливи за съответните координати

    //стартовата точка за първата отсечка
    int firstStartX, firstStartY;
    //стартова точка за втората отсечка
    int secondStartX, secondStartY;
    //крайна точка за първата отсечка
    int firstEndX, firstEndY;
    //крайна точка за втората променлива
    int secondEndX, secondEndY;

    //създаваме динамичен масив от тип двойки отсечки (pair_t)
    //със брой на елементите k
    //правим го така, за да се запази в динамичната памет
    //и а можем да го return-нем от функцията
    //в противен случай, ако го дефинираме така:
    //pair_t pairts;
    //то ще се запази в статчната памет
    //и ще се изтрие след прекратяването на функцията
    pair_t *pairs = (pair_t*) malloc(k*sizeof(pair_t));

    //въртим цикъл k пъти
    //т.е. въвеждаме k двойки отсечки
    for(int i = 0; i < k; i++)
    {
        printf("pair %d FIRST line\n", i+1);
        printf("| STARTING point coordinates (x, y): ");
        //въвеждаме x и y за стартовата точка на първата отсека
        scanf("%d %d", &firstStartX, &firstStartY);
        printf("| ENDIND point coordinates (x, y): ");
        //въвеждаме х и у за крайната точка на първата отсечка
        scanf("%d %d", &firstEndX, &firstEndY);
        printf("\n");
        //присвояваме стойностите на
        //i-тата двойка отсечки ->
        //ПЪРВАТА отсечка ->
        //началната точка
        pairs[i].first.start.x = firstStartX; // х на началната точка
        pairs[i].first.start.y = firstStartY; // у на началната точка
        //крайната точка
        pairs[i].first.end.x = firstEndX; // х на крайната точка
        pairs[i].first.end.y = firstEndY; // у на крайната точка

        printf("pair %d SECOND line\n", i+1);
        printf("| STARTING point coordinates (x, y): ");
        //въвеждаме x и y за стартовата точка на втората отсека
        scanf("%d %d", &secondStartX, &secondStartY);
        printf("| ENDIND point coordinates (x, y): ");
        //въвеждаме х и у за крайната точка на втората отсечка
        scanf("%d %d", &secondEndX, &secondEndY);
        printf("\n");
        //присвояваме стойностите на
        //i-тата двойка отсечки ->
        //ВТОРАТА отсечка ->
        //началната точка
        pairs[i].second.start.x = secondStartX; // х на началната точка
        pairs[i].second.start.y = secondStartY; // у на началната точка
        //крайната точка
        pairs[i].second.end.x = secondEndX; // х на крайната точка
        pairs[i].second.end.y = secondEndY; // у на крайната точка
    }

    //можем да го return-нем, тъй като се пази в heap (динамичната памет)
    //а в heap се пази докато не я free-нем ние самите
    //ако я бяхме дефинирали pair_t pairs,
    //тя щеше да се пази в stack (статична памет) и автоматично щеше да бъе изтрита
    //след края на функцията
    return pairs;
}

//функция за извежане на всички двойки отсечки
void print_lines(pair_t *pairs, int k)
{
    //минаваме през всяка двойка
    for(int i = 0; i < k; i++)
    {
        printf("pair %d:\n", i+1);
        printf("  FIRST line: start(%d, %d), end(%d, %d)\n",
            pairs[i].first.start.x,
            pairs[i].first.start.y,
            pairs[i].first.end.x,
            pairs[i].first.end.y);
        printf("  SECOND line: start(%d, %d), end(%d, %d)\n",
            pairs[i].second.start.x,
            pairs[i].second.start.y,
            pairs[i].second.end.x,
            pairs[i].second.end.y);
    }
}

//функция за извеждане на двойките отсечки, лежащи на една и съща права
void coincident_lines(pair_t *pairs, int k)
{
    //флаг показващ али има поне една двойка отсечки, които да лежат
    //на една и съща права
    // ако е 1 значи НЯМА двойка отсечки лежащи на една права
    // ако е 0 значи има
    int notCoincident = 1;
    for(int i = 0; i < k; i++)
    {
        //как да разберем дали 2 отсечки лежат на една права
        //или по точно: как да разберем че 4 точки лежат на една и съща права?
        //т.е. са колинеарни?
        //Колинеарност (Collinearity): https://www.bbc.co.uk/bitesize/guides/zctppv4/revision/5#:~:text=Three%20or%20more%20points%20are,and%20C%20are%20collinear%20then&text=If%20you%20want%20to%20show,two%20line%20segments%2C%20for%20example
        //4 точки колинеарни?: https://www.quora.com/How-can-I-prove-that-4-points-are-collinear
        float m1 = (pairs[i].first.end.y - pairs[i].first.start.y)/
           (float) (pairs[i].first.end.x - pairs[i].first.start.x);
        float m2 = (pairs[i].second.start.y - pairs[i].first.start.y)/
           (float) (pairs[i].second.start.x - pairs[i].first.start.x);
        float m3 = (pairs[i].second.end.y - pairs[i].first.start.y)/
           (float) (pairs[i].second.end.x - pairs[i].first.start.x);

        //ако математическите условия от линковете са изпълнени
        if(m1 == m2 && m1 == m3)
        {
            printf("pair %d is coincidental\n", i+1);
            printf("  FIRST line: start(%d, %d), end(%d, %d)\n",
                pairs[i].first.start.x,
                pairs[i].first.start.y,
                pairs[i].first.end.x,
                pairs[i].first.end.y);
            printf("  SECOND line: start(%d, %d), end(%d, %d)\n",
                pairs[i].second.start.x,
                pairs[i].second.start.y,
                pairs[i].second.end.x,
                pairs[i].second.end.y);
            //слеователно съществуват двойка отечки лежащи на ена права
            notCoincident = 0;
        }
    }
    //принтирай ако няма двойка отсечки, лежажи на една права
    if(notCoincident)
    {
        printf("None.\n");
    }
}

//функция за извеждане на двойките отсечки с обща начална или крайна точка
void collinear_points(pair_t *pairs, int k)
{
    //флаг показващ али има поне една двойка отсечки, чиито точки
    //да съвпадат
    // ако е 1 значи НЯМА двойка отсечки със съвпадащи точки
    // ако е 0 значи има
    int notCollinear = 1;
    for(int i = 0; i < k; i++)
    {
        //ако x и у на началните точки на двойката отсечки съвпада
        if( pairs[i].first.start.x == pairs[i].second.start.x &&
            pairs[i].first.start.y == pairs[i].second.start.y)
        {
            printf("Common starting point on pair %d:\n", i+1);
            printf("  FIRST line: start(%d, %d), end(%d, %d)\n",
                pairs[i].first.start.x,
                pairs[i].first.start.y,
                pairs[i].first.end.x,
                pairs[i].first.end.y);
            printf("  SECOND line: start(%d, %d), end(%d, %d)\n",
                pairs[i].second.start.x,
                pairs[i].second.start.y,
                pairs[i].second.end.x,
                pairs[i].second.end.y);
            //значи все пак съществува двойка отсечки със съвпадащи точки
            notCollinear = 0;
        }
        //ако x и у на крайните точки на двойката отсечки съвпада
        if( pairs[i].first.end.x == pairs[i].second.end.x &&
            pairs[i].first.end.y == pairs[i].second.end.y)
        {
            printf("Common ending point on pair %d:\n", i+1);
            printf("  FIRST line: start(%d, %d), end(%d, %d)\n",
                pairs[i].first.start.x,
                pairs[i].first.start.y,
                pairs[i].first.end.x,
                pairs[i].first.end.y);
            printf("  SECOND line: start(%d, %d), end(%d, %d)\n",
                pairs[i].second.start.x,
                pairs[i].second.start.y,
                pairs[i].second.end.x,
                pairs[i].second.end.y);
            //значи все пак съществува двойка отсечки със съвпадащи точки
            notCollinear = 0;
        }
        //ако началната точка на ПЪРВАТА отсечка съвпада с крайната на ВТОРАТА
        //или ако крайната точка на ПЪРВАТА отсечка съвпада с началната на ВТОРАТА
        if((pairs[i].first.start.x == pairs[i].second.end.x &&
            pairs[i].first.start.y == pairs[i].second.end.y) ||
           (pairs[i].first.end.x == pairs[i].second.start.x &&
            pairs[i].first.end.y == pairs[i].second.start.y))
        {
            printf("Common point on pair %d:\n", i+1);
            printf("  FIRST line: start(%d, %d), end(%d, %d)\n",
                pairs[i].first.start.x,
                pairs[i].first.start.y,
                pairs[i].first.end.x,
                pairs[i].first.end.y);
            printf("  SECOND line: start(%d, %d), end(%d, %d)\n",
                pairs[i].second.start.x,
                pairs[i].second.start.y,
                pairs[i].second.end.x,
                pairs[i].second.end.y);
            //значи все пак съществува двойка отсечки със съвпадащи точки
            notCollinear = 0;
        }
    }
    //ако няма двойка отсечки със съвпадащи точки
    if(notCollinear)
    {
        printf("None.\n");
    }
}

int main()
{
    //задължително инфо
    printf("Task number: 2018\n");    
    printf("Maximilian Mario Pena, 22 group, Fac.N.: 261220837\n");

    //променливата в която ще съграняваме двойките отсечики
    //динамичен масив
    pair_t *pairs_lines = NULL;
    int k = 0;

    //безкраен цикъл, въртим постоянно докато не въведем 5 (виж case 5)
    while(1)
    {
        //самото меню
        printf("\n");
        printf("1) Input pairs of lines\n");
        printf("2) Print all pairs of lines\n");
        printf("3) Print coincidental lines\n");
        printf("4) Print all lines with common starting or ending points\n");
        printf("5) Exit\n");

        //въвеждане на опция
        int option;
        printf("\nOption: ");
        scanf("%d", &option);

        //при различна опция се извършва различно действие
        switch (option)
        {
            //при option = 1 => Въвеждане на двойките отсечки
            case 1:
                //въвежаме К
                printf("Number of pairs (k): ");
                scanf("%d", &k);
                //записваме дадените от потребителя двойки отсечки в pairs_lines
                //pair_lines e динамичен масив от тип pair_t(struct pair)
                //т.е. можем да му променяме дължината
                //и тъй като се пази в динамичната памет можем да го return-нем от функцияцята
                pairs_lines = input_pair_line(k);
                //записваме двойките отсечки във двоичен файл
                write_file(pairs_lines, k);
                //освобождаваме паметта, за а можем като принтираме двойките отсечки
                //да ги прочетем от двоичния файл
                //т.е. да не пазим данните в променлива
                free(pairs_lines);
                break;

            //при option = 2 => Извеждане на въведените данни
            case 2:
                //четем записаните двойки отсечки в воичния файл
                //правим &k за да можем директно на k да се запише 
                //броя прочетени двойки отсечки от двоичния файл
                //и да го използваме в долната функция
                pairs_lines = read_file(&k);
                //ако НЕ съществуват данни във двоичен файл за двойки отсечки
                //или самият той НЕ съществува
                //т.е. ако не сме прочели нищо то файла
                if(pairs_lines == NULL)
                {
                    printf("\nNo data in binary file\n");
                }
                //в противен случай
                else
                {
                    //извеждаме ги всичките
                    print_lines(pairs_lines, k);
                    //освобождаваме се от паметта
                    free(pairs_lines);
                }   
                break;

            //при option = 3 => Извеждане на двойките отсечки, лежащи на една и съща права
            case 3:
                pairs_lines = read_file(&k);
                if(pairs_lines == NULL)
                {
                    printf("\nNo data in binary file\n");
                }
                else
                {
                    //извеждаме ги 
                    coincident_lines(pairs_lines, k);
                    free(pairs_lines);
                }
                break;

            //при option = 4 => Извеждане на двойките отсечки с обща начална или крайна точка
            case 4:
                pairs_lines = read_file(&k);
                if(pairs_lines == NULL)
                {
                    printf("\nNo data in binary file\n");
                }
                else
                {
                    //извеждаме ги 
                    collinear_points(pairs_lines, k);
                    free(pairs_lines);
                }
                break;

            //при option = 5 => Край (или изход от програмата)
            case 5:
                printf("Exit program...\n");
                //директно тук прекратяваме програмата благополучно
                return 0;
                
            //ако option е различно от 1,2,3,4,5 
            default:
                printf("Choose between the given options!\n");
                break;
        }
    }

    return 0;
}