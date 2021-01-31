#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#define ARRSIZE 500
#define FILL_CHAR 43
#define CHAR_e 101
#define CHAR_g 103
#define CHAR_a 97
#define CHAR_z 122
#define SPACE 32
#define CHAR_ZERO 48
#define CHAR_NINE 57
#define NO_COLORS_BEZ_GREEN 6
#define MAX_PLAYERS 6
#define MAX_CARD_COLOR 20
#define MAX_COLORS 6
#define LARGE_NUMBER 1000
#define SMALL_NUMBER 0
#define WIDTH 2
#define HAND_SIZE 174

using namespace std;

typedef enum color {
    GREEN = 101,
    BLUE = 117,
    RED = 100,
    VIOLET = 111,
    YELLOW = 108,
    WHITE = 105,
    BLACK = 97,
};
#define MAX_LENGTH 7
typedef struct card
{
    int value;
    int card_color_val;
    const char* card_color;
};

typedef struct player
{
    card* hand;
    card* face_down;
};

typedef struct game
{
    int active_player = 0;
    int player_no = 0;
    int explosion_threshold = 0;
};


void check_vals(int p, int k, int g, int gv, int o, double total_cards)
{
    if ((p<2 || p>total_cards) || (k < 1 || k>6) || (g < 1 || g>9) || (gv < 1 || gv>10) || (o < 1 || o>20))
    {
        printf("Error, entered values not within stipulated range");
        exit(0);
    }
}


int* create_dynamic_array(int no_of_other_cards)
{
    // int* arr_set = (int*)malloc((no_of_other_cards)* sizeof(int));
    int* arr_set = (int*)calloc((no_of_other_cards), sizeof(int));
    if (arr_set == NULL)
    {
        printf("Memory not allocated");
        exit(0);
    }
    /*else
    {
        for (int i = 0; i < no_of_other_cards; i++)
        {
            scanf_s("%d", &arr_set[i]);
        }
    }*/
    return arr_set;
}


bool* create_bool_dynamic_array(int no_of_other_cards)
{
    bool* arr_set = (bool*)calloc((no_of_other_cards), sizeof(bool));
    if (arr_set == NULL)
    {
        printf("Memory not allocated");
        exit(0);
    }
    return arr_set;
}


int** create_2d_dynamic_array(int max_colors_no, int player_no)
{
    int** arr_set = (int**)calloc(max_colors_no, sizeof(int*));
    for (int i = 0; i < max_colors_no; i++)
    {
        arr_set[i] = (int*)calloc((player_no), sizeof(int));
    }

    if (arr_set == NULL)
    {
        printf("Memory not allocated");
        exit(0);
    }

    return arr_set;
}

//Creates an empty array of deck of cards
card* create_deck(int total_cards)
{
    card* deck = (card*)malloc(total_cards * sizeof(card));

    if (deck == NULL)
    {
        printf("Memory not allocated");
        exit(0);
    }
    return deck;
}

//Adds the cards from arr_set to deck
void add_cards_deck(card* deck, int* arr_set, int no_cauldrons, int total_greens, int val_greens, int no_of_other_cards)
{
    //Adding greens to deck
    for (int i = 0; i < total_greens; i++)
    {
        deck[i].value = val_greens;
        //deck[i].card_color_val = GREEN;
        deck[i].card_color = "green";
    }

    //Adding the other cards

    int k, j = total_greens;
    for (int i = 1; i <= no_cauldrons; i++)
    {
        for (j, k = 0; (j < (j + no_of_other_cards) && k < (no_of_other_cards)); j++, k++)
        {
            deck[j].value = arr_set[k];
            deck[j].card_color_val = i;
            switch (i)
            {
            case 1: deck[j].card_color = "blue";
                break;
            case 2: deck[j].card_color = "red";
                break;
            case 3: deck[j].card_color = "violet";
                break;
            case 4: deck[j].card_color = "yellow";
                break;
            case 5: deck[j].card_color = "white";
                break;
            case 6: deck[j].card_color = "black";
                break;
            case 7: deck[j].card_color = "brown";
                break;
            default: printf("There is an error");
                break;
            }
        }
    }
}

//prints deck of cards
void print_deck(card* deck, int total_cards)
{
    for (int i = 0; i < total_cards; i++)
    {
        printf("%d %s ", deck[i].value, deck[i].card_color);
    }
    printf("\n");
}

//creates an array of players
player* create_players(int no_of_players)
{
    player* players = (player*)malloc(no_of_players * sizeof(player));

    if (players == NULL)
    {
        printf("Memory not allocated for players");
        exit(0);
    }
    return players;
}

//creates a players hand according to hand size
void create_players_hand(double hand_size, int no_of_players, player* players)
{
    for (int i = 0; i < no_of_players; i++)
    {
        double hand_size_temp = hand_size;

        players[i].hand = create_deck(hand_size_temp);
    }
}


double check_if_last_odd(double hand_size, int i, int no_of_players, int total_cards)
{
    double hand_size_temp = hand_size;
    if (((int)total_cards % (int)no_of_players != 0) && i == no_of_players - 1)
    {
        hand_size_temp = hand_size - 1;
    }
    return hand_size_temp;
}


void distribute_cards_to_hand(double hand_size, int no_of_players, int total_cards, player* players, card* deck)
{
    int k = 0, j;
    for (int i = 0; i < no_of_players; i++)
    {
        double  hand_size_temp = check_if_last_odd(hand_size, i, no_of_players, total_cards);
        for (j = 0, k = i; j < hand_size_temp && k < total_cards; j++, k = k + no_of_players)
        {
            players[i].hand[j] = deck[k];
        }
    }
}


void assign_cards_to_players(double hand_size, int no_of_players, int total_cards, player* players)
{
    for (int i = 0; i < no_of_players; i++)
    {
        double hand_size_temp = check_if_last_odd(hand_size, i, no_of_players, total_cards);
        printf("%d player hand cards: ", i + 1);
        for (int j = 0; j < hand_size_temp; j++)
        {
            if (players[i].hand[j].value == NULL)
                continue;
            printf("%d %s ", players[i].hand[j].value, players[i].hand[j].card_color);
            //printf("%d %s ",players[i].face_down[j].value,players[i].face_down[j].card_color);
        }
        printf("\n");
        printf("%d player deck cards: ", i + 1);
        printf("\n");
    }
}


void count_deck_cards(char(*str), int* counter, int* counter_temp, int* temp, float test_even_odd)
{
    //cout<<counter<<" "<<*counter<<endl;
    for (int j = 1; j < ARRSIZE; j++)
    {
        if ((int)str[j] >= 48 && (int)str[j] <= 57 && ((int)str[j + 1] >= 48 && (int)str[j + 1] <= 57))
            (*counter)++;
        if ((int)str[j] >= 48 && (int)str[j] <= 57)
            (*counter_temp)++;
        //cout <<str[j]<<endl;
    }
    if (test_even_odd != 0.0)
        cout << (*temp) << " player has " << (*counter_temp) - (*counter) << " cards on hand\n";
    else
    {
        cout << (*temp) << " player has " << (*counter_temp) - (*counter) << " cards in front of him\n";
        (*temp)++;
    }
    *counter = 0;
    *counter_temp = 0;
}


void  assign_char_val(char(*str))
{
    for (int k = 0; k < ARRSIZE; k++)
    {
        str[k] = '+';
    }
}


void load_game_3a()
{
    char active[ARRSIZE], players[ARRSIZE];
    char sign;
    int player_no, counter = 0, counter_temp = 0;
    for (int i = 0; i < 2; i++)
    {
        cin >> active >> players >> sign >> player_no;
        //cout<<active<<players<<sign<<player_no;
    }
    cin.ignore();
    //cout<<player_no;
    for (int i = 1, temp = 1; i <= 2 * player_no; i++)
    {
        char str[ARRSIZE];
        assign_char_val(str);
        cin.getline(str, ARRSIZE);
        float test_even_odd = i % 2;
        count_deck_cards(str, &counter, &counter_temp, &temp, test_even_odd);
    }
}


void load_game_3b()
{
    char line[ARRSIZE];
    int cnt = 0, pile = 1;
    cin.ignore();
    while (cin.getline(line, ARRSIZE))
    {
        if ((int)line[1] == FILL_CHAR)
            break;
        for (int i = 1; i < ARRSIZE; i++)
        {
            if ((int)line[i] >= CHAR_ZERO && (int)line[i] <= CHAR_NINE && (int)line[i + 1] == SPACE && (int)line[i + 2] >= CHAR_a && (int)line[i + 2] <= CHAR_z)
            {
                cnt++;
            }

        }
        cout << "there are " << cnt << " cards on " << pile << " pile" << endl;
        cnt = 0;
        assign_char_val(line);
        pile++;
    }
}


void load_game_4a(char(*str), char(*hold))
{
    int determiner = 0, counter = 0, j = 0;
    while (cin.getline(str, ARRSIZE))
    {
        if ((int)str[1] == FILL_CHAR)
            break;
        for (int i = 1; i < ARRSIZE; i++)
        {
            if ((int)str[i + 2] == CHAR_g && (int)str[i] >= CHAR_ZERO && (int)str[i] <= CHAR_NINE && (int)str[i + 1] == SPACE)
            {
                hold[j] = str[i];
                j++;
                counter++;
                i = i + 6;
                //cout<<"counter is "<<counter;
            }
            else if ((int)str[i + 3] == CHAR_g && (int)str[i] >= CHAR_ZERO && (int)str[i] <= CHAR_NINE && (int)str[i + 1] >= CHAR_ZERO && (int)str[i + 1] <= CHAR_NINE && (int)str[i + 2] == SPACE)
            {
                hold[j] = str[i];
                hold[j + 1] = str[i + 1];
                j = j + 2;
                counter++;
                i = i + 7;
            }
        }
        for (int k = 0; k < j; k++)
        {
            if ((int)hold[k + 2] == FILL_CHAR)// 4 4 4 4 + + 1 0 1 0 1 0 + +
            {
                break;
            }
            determiner = (hold[k] == hold[k + 2]) ? 1 : 2;
            //cout<<"det is "<<determiner;
        }

        assign_char_val(str);
    }
    switch (determiner)
    {
    case 0: printf("Green cards does not exist");
        break;
    case 1: if (hold[0] == hold[1]) {
        printf("Found %d green cards, all with %c value", counter, hold[0]);
    }
          else {
        printf("Found %d green cards, all with %c%c value", counter, hold[0], hold[1]);
    }
          break;
    case 2: printf("Different green cards values occured");
        break;
    default: printf("Error with either code or input");
    }
}


void count_colors(int(*colors), int str_val)
{
    switch (str_val)
    {
    case BLUE: colors[0]++;
        break;
    case RED: colors[1]++;
        break;
    case VIOLET: colors[2]++;
        break;
    case YELLOW: colors[3]++;
        break;
    case WHITE: colors[4]++;
        break;
    case BLACK: colors[5]++;
        break;
    default:break;
    }
}

int index(int str_val)
{
    int index_type = 0;
    switch (str_val)
    {
    case BLUE: index_type = 0;
        break;
    case RED: index_type = 1;
        break;
    case VIOLET: index_type = 2;
        break;
    case YELLOW: index_type = 3;
        break;
    case WHITE: index_type = 4;
        break;
    case BLACK: index_type = 5;
        break;
    default:break;
    }
    return index_type;
}

void load_game_4b(char(*str))
{
    int colors[NO_COLORS_BEZ_GREEN] = { 0 };
    while (cin.getline(str, ARRSIZE))
    {
        if ((int)str[1] == FILL_CHAR)
            break;
        for (int i = 1; i < ARRSIZE; i++)
        {
            if ((int)str[i] >= CHAR_ZERO && (int)str[i] <= CHAR_NINE && (int)str[i + 1] == SPACE && (int)str[i + 4] >= CHAR_a && (int)str[i + 4] <= CHAR_z)
            {
                count_colors(colors, (int)str[i + 4]);
            }
        }
        assign_char_val(str);
    }
    bool alltrue = 0;
    int store = 0;
    for (int i = 0; i < NO_COLORS_BEZ_GREEN - 1; i++)
    {
        if (colors[i] != colors[i + 1] && colors[i] > 0 && colors[i + 1] > 0)
        {
            printf("At least two colors with a different number of cards were found:\n");
            for (int j = 0; j < NO_COLORS_BEZ_GREEN; j++)
            {
                switch (j)
                {
                case 0: if (colors[j] > 0) {
                    printf("blue cards are %d \n", colors[j]);
                }
                      break;
                case 1: if (colors[j] > 0) {
                    printf("red cards are %d \n", colors[j]);
                }
                      break;
                case 2: if (colors[j] > 0) {
                    printf("violet cards are %d \n", colors[j]);
                }
                      break;
                case 3: if (colors[j] > 0) {
                    printf("yellow cards are %d \n", colors[j]);
                }
                      break;
                case 4: if (colors[j] > 0) {
                    printf("white cards are %d \n", colors[j]);
                }
                      break;
                case 5: if (colors[j] > 0) {
                    printf("black cards are %d \n", colors[j]);
                }
                      break;
                default: printf("Error with colors in 4b code or input");
                }
            }
            break;
        }
        else if (colors[i] == colors[i + 1] && colors[i] > 0 && colors[i + 1] > 0)
        {
            alltrue = 1;
            store = colors[i];
        }
    }
    if (alltrue == 1)
    {
        printf("The number cards of all colors is equal: %d", store);
    }
}

int read_2_numbers(char* str, int position)
{
    if ((int)str[position] >= CHAR_ZERO && (int)str[position] <= CHAR_NINE && !((int)str[position + 1] >= CHAR_ZERO && (int)str[position + 1] <= CHAR_NINE))
    {
        if ((int)str[position - 1] >= CHAR_ZERO && (int)str[position - 1] <= CHAR_NINE)
        {
            //cout<<str[position]-'0' +((str[position-1]-'0')*10)<<" ";
            return (str[position] - '0' + ((str[position - 1] - '0') * 10));
        }
        else {
            //cout<<str[position]-'0' <<" ";
            return str[position] - '0';
        }
    }
    return 0;
}

int read_2_numbers_file(char* str[ARRSIZE], int i, int position)
{
    if ((int)str[i][position] >= CHAR_ZERO && (int)str[i][position] <= CHAR_NINE && !((int)str[i][position + 1] >= CHAR_ZERO && (int)str[i][position + 1] <= CHAR_NINE))
    {
        if ((int)str[i][position - 1] >= CHAR_ZERO && (int)str[i][position - 1] <= CHAR_NINE)
        {

            return (str[i][position] - '0' + ((str[i][position - 1] - '0') * 10));
        }
        else {
            //cout<<str[position]-'0' <<" ";
            return str[i][position] - '0';
        }
    }
    return 0;
}


//reads the values from a char array
void read_values(char* str, int** position, player* where_to, double** hand_size, int** colors_count_players, bool first_read, int* results, bool* color_appeared)
{
    //cout<<position<<endl<<endl;
    for (int j = 1, m = 0; j < ARRSIZE && m < **hand_size; j++)
    {
        if ((int)str[j] >= CHAR_ZERO && (int)str[j] <= CHAR_NINE && (int)str[j + 1] == SPACE &&/*(int)str[j+4]!=CHAR_e&&*/(int)str[j + 4] >= CHAR_a && (int)str[j + 4] <= CHAR_z)
        {
            //cout<<position<<endl<<endl;
            where_to[**position].hand[m].value = read_2_numbers(str, j);
            where_to[**position].hand[m].card_color_val = (int)str[j + 4];

            int pos_player = round(((**position) * 2) / 5.0);//To get the true position of deck of player
            if (first_read == true && (int)str[j + 4] != CHAR_e)//To only perform on player decks and  not piles, without greens
            {
                results[pos_player]++;
                // cout<<pos_player<<" ";
                colors_count_players[index((int)str[j + 4])][pos_player]++;
                //cout<<index((int)str[j + 4])<<" "<<pos_player;
            }
            if (first_read == true && (int)str[j + 4] == CHAR_e)//Each green cards is worth 2 points
            {
                results[pos_player] = results[pos_player] + 2;
            }
            if (first_read == false)
            {
                color_appeared[index((int)str[j + 4])] = 1;
            }
            m++;
        }
    }

}


void initialize_first_column(int size_of_arr, player* players)
{
    for (int i = 0; i < size_of_arr; i++)
    {
        for (int j = 0; j < HAND_SIZE; j++)
        {
            players[i].hand[j].value = 0;
            players[i].hand[j].card_color_val = 0;
        }
    }
}

void sort_by_color(int size_arr, player* players, int colors[][MAX_CARD_COLOR])
{
    for (int i = 0; i < size_arr; i++)
    {
        if (players[i].hand[0].value == 0)//to skip empty hands/decks
        {
            continue;
        }
        int kolor = index(players[i].hand[0].card_color_val);
        int temp = kolor;
        for (int j = 0, m = 0; j < 50; j++)
        {
            kolor = index(players[i].hand[j].card_color_val);
            if (temp != kolor)
            {
                m = 0;
            }
            while (colors[kolor][m] > 0)
            {
                temp = kolor;
                m++;
            }

            colors[kolor][m] = players[i].hand[j].value;
        }
    }
}

int compare(const void* ptr_a, const void* ptr_b)
{
    const int* a = (int*)ptr_a;
    const int* b = (int*)ptr_b;

    return *a - *b;
}

void print_cards_all(int colors[][MAX_CARD_COLOR], int position_i)
{
    for (int j = 0; j < MAX_CARD_COLOR; j++)
    {
        if (colors[position_i][j] > 0)
            cout << colors[position_i][j] << " ";
    }
    cout << endl;
}

void print_location(int position)
{
    switch (position)
    {
    case 0: cout << "blue ";
        break;
    case 1: cout << "red ";
        break;
    case 2: cout << "violet ";
        break;
    case 3: cout << "yellow ";
        break;
    case 4: cout << "white ";
        break;
    case 5: cout << "black ";
        break;
    }
}

int change_from_index_to_color(int position)
{
    switch (position)
    {
    case 0: return BLUE;
        break;
    case 1: return RED;
        break;
    case 2: return VIOLET;
        break;
    case 3: return YELLOW;
        break;
    case 4: return WHITE;
        break;
    case 5: return BLACK;
        break;
    }
    return 0;
}

bool compare_color_values(int colors[][MAX_CARD_COLOR], bool test)
{
    for (int i = 1; i < 6; i++)
    {

        for (int j = MAX_CARD_COLOR - 1; j > 0; j--)
        {
            if (colors[i][j] == 0)
                continue;
            if (colors[i - 1][j] == colors[i][j])
                test = true;
            else {
                test = false;
                break;
            }
        }
    }
    return test;
}


void print_cards_quantity(bool test, int colors[][MAX_CARD_COLOR])
{
    if (test == true)
    {
        cout << "The values of cards of all colors are identical: " << endl;
        for (int i = 0; i < 1; i++)
        {
            print_cards_all(colors, i);
        }
    }
    else {
        cout << "The values of cards of all colors are not identical: " << endl;
        for (int i = 0; i < 6; i++)
        {
            print_location(i); //dont forget to add " cards values:" for this question, removed it for question 8
            print_cards_all(colors, i);
        }
    }
}


void sort_small_to_large(int colors[][MAX_CARD_COLOR])
{
    for (int i = 0; i < MAX_COLORS; i++)
    {
        if (colors[i][0] == 0)//to skip empty hands/decks
        {
            continue;
        }
        qsort(colors[i], MAX_CARD_COLOR, sizeof(int), compare);
    }
}


void colors_no_5(int total_player_size, player* players, player* cauldrons)
{
    int colors[MAX_COLORS][MAX_CARD_COLOR] = { 0 };

    sort_by_color(total_player_size, players, colors);
    sort_by_color(MAX_PLAYERS, cauldrons, colors);

    sort_small_to_large(colors);


    bool test = 1;
    test = compare_color_values(colors, test);
    print_cards_quantity(test, colors);
}


void check_player_hand_and_pile(int player_no, int cauldron_size, int hand_size, player* players, player* cauldrons)
{
    double counter = 0, counter_cauldron = 0;
    const int player_hand_size = player_no;
    const int pile_error_size = cauldron_size;
    int* player_hand_no = create_dynamic_array(player_hand_size);
    int* pile_error = create_dynamic_array(pile_error_size);

    //int player_hand_no[player_hand_size]={0};
    //int pile_error[pile_error_size]={0};
    bool checker = 1;

    for (int i = 0, m = 0; i < 2 * player_no; i++)
    {
        if (i % 2 != 0)
        {
            m++;
        }
        for (int j = 0; j < hand_size; j++)
        {
            //cout<<players[i].hand[j].value<<" ";
            if (players[i].hand[j].value > 0)
            {
                // cout<<players[i].hand[j].value<<" ";
                counter++;
            }
            if ((i % 2 == 0) && (players[i].hand[j].value > 0))
            {
                //cout<<players[i].hand[j].value<<" ";
                player_hand_no[m]++;
            }
        }
        // cout<<endl;
    }
    //cout<<endl;
    for (int m = 0; m < pile_error_size; m++)
    {
        for (int j = 0; j < hand_size; j++)
        {
            if (cauldrons[m].hand[j].value > 0)
            {
                counter++;
                counter_cauldron++;
            }
        }
    }

    int max_cards = round((counter - counter_cauldron) / (double)player_no);

    //cout<<max_cards<<endl;
    for (int i = 0; i < player_no; i++)
    {

        //cout<< player_hand_no[i]<<endl;
        if (player_hand_no[i] == (max_cards) || player_hand_no[i] == (max_cards - 1) || player_hand_no[i] == (max_cards + 1))
        {
            //cout<< player_hand_no[i]<<endl;
            checker = 1;
        }
        else
        {
            //cout<< player_hand_no[i]<<endl;
            cout << "The number of players cards on hand is wrong" << endl;
            checker = 0;
            break;
        }
    }

    for (int m = 0; m < pile_error_size; m++)
    {
        int cauldron_color_checker = cauldrons[m].hand[0].card_color_val;
        for (int j = 0; j < hand_size; j++)
        {
            if (cauldrons[m].hand[j].card_color_val > 0 && cauldrons[m].hand[j].card_color_val != CHAR_e && cauldrons[m].hand[j].card_color_val != cauldron_color_checker)
            {

                cout << "Two different colors were found on the " << m + 1 << " pile" << endl;
                checker = 0;
                break;
            }
            if (cauldrons[m].hand[j].card_color_val > 0)
            {
                pile_error[m] = pile_error[m] + cauldrons[m].hand[j].value;
            }
        }
    }


    for (int i = 0; i < pile_error_size; i++)
    {
        if (pile_error[i] > 13) {
            cout << "Pile number " << i + 1 << " should explode earlier" << endl;
            checker = 0;
        }
    }
    if (checker == 1)
        cout << "Current state of the game is ok";

    free(player_hand_no);
    free(pile_error);

}

void print_colors(int str_val, FILE* fp)
{
    switch (str_val)
    {
    case GREEN: fprintf(fp, " green");
        break;
    case BLUE:fprintf(fp, " blue");
        break;
    case RED: fprintf(fp, " red");
        break;
    case VIOLET: fprintf(fp, " violet");
        break;
    case YELLOW: fprintf(fp, " yellow");
        break;
    case WHITE: fprintf(fp, " white");
        break;
    case BLACK: fprintf(fp, " black");
        break;
    case 1: fprintf(fp, " player hand cards: ");
        break;
    case 2: fprintf(fp, " player deck cards: ");
        break;
    case 3: fprintf(fp, " pile cards: ");
        break;
    default:break;
    }
}

//checks if sum of pile is greater than threshold
bool explosion_checker(int explosion_threshold, player* cauldrons, int position_cauldron, int hand_size)
{
    bool checker = 0;
    int sum = 0;
    for (int i = 0; i < hand_size; i++)
    {
        sum = sum + cauldrons[position_cauldron].hand[i].value;
    }
    if (sum > explosion_threshold)
        checker = 1;
    return checker;
}

//returns the position of the first empty part of player deck
int first_zero_pos(player* players, int active_player_deck_pos, int hand_size)
{
    int first_zero = 0;
    for (int i = 0; i < hand_size; i++)
    {
        if (players[active_player_deck_pos].hand[i].value == 0)
        {
            first_zero = i;
            break;
        }
    }
    return first_zero;
}

//Moves cards from pile to the appropriate player deck
void pile_to_face_down(player* cauldrons, bool explosion, player* players, int position_cauldron, int hand_size, int active_player_deck_pos)
{
    if (explosion == true)
    {
        int first_zero = first_zero_pos(players, active_player_deck_pos, hand_size);

        for (int i = 0; i < hand_size; i++)
        {
            if (cauldrons[position_cauldron].hand[i].value > 0)
            {
                //copies the values from the exploded cauldron to player deck
                players[active_player_deck_pos].hand[first_zero].value = cauldrons[position_cauldron].hand[i].value;
                players[active_player_deck_pos].hand[first_zero].card_color_val = cauldrons[position_cauldron].hand[i].card_color_val;
                first_zero++;
                //Resets the copied values to zero, basicalyy the pile is reset to zero
                cauldrons[position_cauldron].hand[i].value = 0;
                cauldrons[position_cauldron].hand[i].card_color_val = 0;
            }
        }
    }
}


void check_repeating_high_val(int player_no, int** colors_count_players, int* results)
{
    int highest_player = 0, count_each_player = 0;
    bool repeating = false;
    for (int i = 0; i < MAX_COLORS; i++)
    {
        for (int j = 0; j < player_no; j++)
        {
            if (colors_count_players[i][j] == count_each_player)
            {
                repeating = true;
            }
            if (colors_count_players[i][j] > count_each_player)
            {
                highest_player = j + 1;
                count_each_player = colors_count_players[i][j];//current highest value
                repeating = false;
            }

        }
        // cout<<"highest player has "<<count_each_player<<endl;
        if (repeating == false)
        {
            cout << "Na kolor "; print_location(i); cout << "odporny jest gracz " << highest_player << endl;
            //Minuses the cards of a specific color from a player with highest card color
            results[highest_player - 1] = results[highest_player - 1] - colors_count_players[i][highest_player - 1];
        }
        highest_player = 0;
        count_each_player = 0;
    }

    //print the results of players
    for (int i = 0; i < player_no; i++)
    {
        cout << "Wynik gracza " << i + 1 << " = " << results[i] << endl;

    }
}


void move_first_card_to_pile(player* players, player* cauldrons, int card_copy_val, int card_copy_color_val, int active_player_pos, int active_player_deck_pos, int hand_size, int explosion_threshold, int cauldron_size)
{
    card_copy_val = players[active_player_pos].hand[0].value;
    card_copy_color_val = players[active_player_pos].hand[0].card_color_val;
    players[active_player_pos].hand[0].value = 0;
    players[active_player_pos].hand[0].card_color_val = 0;
    bool we_did_not_fill = true;

    for (int i = 0; i < cauldron_size; i++)
    {
        for (int j = 0; j < hand_size; j++)
        {
            //makes sure we are looking at the last card
            if ((cauldrons[i].hand[j].card_color_val == card_copy_color_val && cauldrons[i].hand[j + 1].value == 0) || (card_copy_color_val == GREEN && cauldrons[i].hand[j + 1].value == 0) || (cauldrons[i].hand[j].card_color_val == GREEN && cauldrons[i].hand[j + 1].value == 0 && card_copy_color_val == cauldrons[i].hand[0].card_color_val))
            {
                cauldrons[i].hand[j + 1].value = card_copy_val;
                cauldrons[i].hand[j + 1].card_color_val = card_copy_color_val;
                bool explosion = explosion_checker(explosion_threshold, cauldrons, i, hand_size);
                pile_to_face_down(cauldrons, explosion, players, i, hand_size, active_player_deck_pos);
                we_did_not_fill = false;
                break;
            }
        }
        if (card_copy_color_val == GREEN)//prevents putting the green card in multiple cauldrons
        {
            break;
        }

    }
    //Situaution where color appears first time in Cauldron
    if (we_did_not_fill == true && card_copy_color_val != GREEN)
    {
        cauldrons[0].hand[0].value = card_copy_val;
        cauldrons[0].hand[0].card_color_val = card_copy_color_val;
        bool explosion = explosion_checker(explosion_threshold, cauldrons, 0, hand_size);
        pile_to_face_down(cauldrons, explosion, players, 0, hand_size, active_player_deck_pos);
    }

}


void print_basic_three_file(game game_state, FILE* fp)
{
    fprintf(fp, "active player =");
    fprintf(fp, " %d\n", game_state.active_player);
    fprintf(fp, "players number =");
    fprintf(fp, " %d\n", game_state.player_no);
    fprintf(fp, "explosion threshold =");
    fprintf(fp, " %d\n", game_state.explosion_threshold);
}


void print_det_to_file(player* where_from, int size, int hand_size, FILE* fp, bool printing_players)
{
    int printer_det = 0;

    for (int i = 0, m = 1; i < size; i++)
    {
        if (printing_players == true)
        {
            fprintf(fp, "%d", m);
            if (i % 2 != 0)//increases only after passing both player hand and deck
            {
                m++;
            }
            printer_det = (i % 2 == 0) ? 1 : 2;//helps print 2 things alternatively
            print_colors(printer_det, fp);
        }
        else {
            int pile_cards = 3;//prints pile cards:
            fprintf(fp, "%d", i + 1);
            print_colors(pile_cards, fp);
        }

        for (int j = 0; j < hand_size; j++)
        {
            if (where_from[i].hand[j].value > 0)
            {
                fprintf(fp, "%d", where_from[i].hand[j].value); print_colors(where_from[i].hand[j].card_color_val, fp); fprintf(fp, " ");
            }
        }
        fprintf(fp, "\n");
    }
}

void print_pile(player* cauldrons, int cauldron_size, int hand_size, FILE* fp)
{
    for (int i = 0; i < cauldron_size; i++)
    {
        int pile_cards = 3;
        fprintf(fp, "%d", i + 1);
        print_colors(pile_cards, fp);
        for (int j = 0; j < hand_size; j++)
        {
            if (cauldrons[i].hand[j].value > 0)
            {
                fprintf(fp, "%d", cauldrons[i].hand[j].value); print_colors(cauldrons[i].hand[j].card_color_val, fp); fprintf(fp, " ");
            }
        }
        fprintf(fp, "\n");
    }
}


void delete_from_hand(int smallest, int color_type, player* players, int hand_size, int active_player_pos)
{
    for (int i = 0; i < hand_size; i++)
    {
        if (players[active_player_pos].hand[i].value == smallest && players[active_player_pos].hand[i].card_color_val == color_type)
        {
            players[active_player_pos].hand[i].value = 0;
            break;
        }
    }
}


void initialize_large_small(int** colors_small)
{
    for (int i = 0; i < MAX_COLORS; i++)
    {
        colors_small[i][0] = LARGE_NUMBER; //Here we want to store the smallest value
        colors_small[i][1] = SMALL_NUMBER;//Here we wabt to store the sum
    }
}


void color_smallest_phand(player* players, int** colors_small, int hand_size, int active_player_pos)
{
    for (int i = 0; i < hand_size; i++)
    {
        if (players[active_player_pos].hand[i].value > 0)
        {

            int index_color = index(players[active_player_pos].hand[i].card_color_val);//This tells us which index we store th value, according to color
            if (colors_small[index_color][0] > players[active_player_pos].hand[i].value)
            {
                colors_small[index_color][0] = players[active_player_pos].hand[i].value;//Stores smallest value of each color in player hand
            }
        }
    }
}

void find_empty_cauldron(player* cauldrons,  bool* found, int* index_empty_cauldron, int position)
{
    if (cauldrons[position].hand[0].value == 0 && *found == false)//to find the first empty cauldron
    {
        *found = true;
        *index_empty_cauldron = position;//gets the index of the empty cauldron
    }
}

void color_smallest_sum_cauldron(player* cauldrons, int cauldron_size, int** colors_small, int hand_size, int* pos_small_cauldron, int* cauldron_pos, int* index_empty_cauldron, bool* found)
{
    int smallest_cauldron = LARGE_NUMBER;

    for (int i = 0; i < cauldron_size; i++)
    {
        int index_color = index(cauldrons[i].hand[0].card_color_val);
        find_empty_cauldron(cauldrons, found, index_empty_cauldron, i);
        for (int j = 0; j < hand_size; j++)
        {
            if (cauldrons[i].hand[j].value > 0)
            {
                colors_small[index_color][1] = colors_small[index_color][1] + cauldrons[i].hand[j].value;
            }
        }
        if (colors_small[index_color][1] < smallest_cauldron)
        {
            smallest_cauldron = colors_small[index_color][1];//stores sum of the smallest cauldron
            *pos_small_cauldron = index_color;//index of colors_small[index][] of the smallest sum
            *cauldron_pos = i;//stores the pos from where the smallest sum came from cauldrons[cauldron_pos].hand.....
        }
    }
}


void smallest_val_repeating(int** colors_small, bool* similar, int* smallest, int* smallest_pos)
{
    for (int i = 0; i < MAX_COLORS - 2; i++)
    {

        if (colors_small[i + 2][0] < colors_small[i][0])//finds the smallest value of all colors
        {
            *smallest = colors_small[i + 1][0];
            *smallest_pos = i + 1;
            *similar = false;
        }
        if (colors_small[i + 2][0] == colors_small[i][0])
        {
            *similar = true;
        }

    }
}


void card_mover(player* cauldrons, int smallest, int position_of_smallest, int pos_hand, int cauldron_pos, int color_type, player* players, int hand_size, int active_player_pos)
{
    color_type = change_from_index_to_color(position_of_smallest);
    cauldrons[cauldron_pos].hand[pos_hand].value = smallest;
    cauldrons[cauldron_pos].hand[pos_hand].card_color_val = color_type;
    delete_from_hand(smallest, color_type, players, hand_size, active_player_pos);
}



game read_input_first_three(game game_state, int first_three[3], bool file_input)
{
    if (file_input == false)
    {
        char active[ARRSIZE], active_char[ARRSIZE], players_char[ARRSIZE], explosion_char[ARRSIZE];
        char sign;
        //int active_player, player_no, explosion_threshold;
        double counter = 0, determiner = 0, counter_cauldron = 0;

        cin >> active >> active_char >> sign >> game_state.active_player;
        cin >> active >> players_char >> sign >> game_state.player_no;
        cin >> active >> explosion_char >> sign >> game_state.explosion_threshold;

        cin.ignore();
    }
    else
    {
        game_state.active_player = first_three[0];
        game_state.player_no = first_three[1];
        game_state.explosion_threshold = first_three[2];
    }
    return game_state;
}

void read_hand_pile(game(*game_state), player* players, double* hand_size, player* cauldrons, int* cauldron_size, char* str, int** colors_count_players, int* results, bool* color_appeared)
{

    int i = 0;
    int* ptr = &i;
    bool first_read = 1; //used to make sure the color highest count occurs for players only and not pile
    for (int i = 0; i < 2 * (game_state->player_no); i++)
    {
        cin.getline(str, ARRSIZE);
        if ((int)str[0] == '\0')
            continue;
        if ((int)str[0] == NULL)
            i = i - 1;
        read_values(str, &ptr, players, &hand_size, colors_count_players, first_read, results, color_appeared);
        players++;
        assign_char_val(str);

    }

    first_read = 0;
    cout << cauldron_size << endl;

    while (cin.getline(str, ARRSIZE))
    {
        if ((int)str[0] == '\0')
            break;
        read_values(str, &cauldron_size, cauldrons, &hand_size, colors_count_players, first_read, results, color_appeared);
        (*cauldron_size)++;
        assign_char_val(str);//fills the str char array with a + sign
    }
}


void read_hand_pile_file(game(*game_state), player* players, double* hand_size, player* cauldrons, int cauldron_size, char* str[ARRSIZE], int** colors_count_players, int* results, bool* color_appeared)
{
    bool first_read = 1; //used to make sure the color highest count occurs for players only and not pile
    for (int i = 0; i < 2 * (game_state->player_no); i++)
    {
        if (str[i][0] != NULL)
        {
            for (int j = 0, m = 1; j < (int)*hand_size && m < (strlen(str[i])); m++)
            {
                if ((int)str[i][m] >= CHAR_ZERO && (int)str[i][m] <= CHAR_NINE && (int)str[i][m + 1] == SPACE &&/*(int)str[j+4]!=CHAR_e&&*/(int)str[i][m + 2] >= CHAR_a && (int)str[i][m + 2] <= CHAR_z)
                {
                    players[i].hand[j].value = read_2_numbers_file(str, i, m);
                    players[i].hand[j].card_color_val = (int)str[i][m + 4];
                    j++;

                    int pos_player = round(((i) * 2) / 5.0);//To get the true position of deck of player
                    if (first_read == true && (int)str[i][m + 4] != CHAR_e)//To only perform on player decks and  not piles, without greens
                    {
                        results[pos_player]++;
                        // cout<<pos_player<<" ";
                        colors_count_players[index((int)str[i][m + 4])][pos_player]++;
                        //cout<<index((int)str[j + 4])<<" "<<pos_player;

                    }
                    if (first_read == true && (int)str[i][m + 4] == CHAR_e)//Each green cards is worth 2 points
                    {
                        results[pos_player] = results[pos_player] + 2;

                    }

                }

            }

        }
    }
    int i = 2 * (game_state->player_no);
    first_read = 0;
    for (int k = 0; k < cauldron_size; k++, i++)
    {
        if (str[i] != NULL)
        {

            for (int j = 0, m = 1; j < (int)hand_size && m < (strlen(str[i])); m++)
            {
                if ((int)str[i][m] >= CHAR_ZERO && (int)str[i][m] <= CHAR_NINE && (int)str[i][m + 1] == SPACE &&/*(int)str[j+4]!=CHAR_e&&*/(int)str[i][m + 2] >= CHAR_a && (int)str[i][m + 2] <= CHAR_z)
                {

                    cauldrons[k].hand[j].value = read_2_numbers_file(str, i, m);
                    cauldrons[k].hand[j].card_color_val = (int)str[i][m + 4];

                    j++;
                    if (first_read == false)
                    {
                        color_appeared[index((int)str[i][m + 4])] = 1;
                    }
                }
            }
        }
    }

}


void move_smallest_card(bool found, bool similar, bool* color_appeared, int** colors_small, player* players, player* cauldrons, int cauldron_pos, int hand_size, int active_player_pos, int pos_small_cauldron, int index_empty_cauldron, int smallest, int smallest_pos)
{
    int color_type = 0;
    //if the smallest card has a similar value, we assign the card from player to cauldron, according to sum
    if (found == true && similar == true)
    {
        int smallest_val = LARGE_NUMBER;
        for (int i = 0; i < MAX_COLORS; i++)
        {
            if (color_appeared[i] == 0 && smallest_val >= colors_small[i][0])
            {
                smallest_val = colors_small[i][0];
                int pos = i;
                card_mover(cauldrons, colors_small[i][0], i, 0, index_empty_cauldron, color_type, players, hand_size, active_player_pos);
                break;
            }
        }
    }
    if (similar == true && found == false)
    {

        for (int i = 0; i < hand_size; i++)
        {
            if (cauldrons[cauldron_pos].hand[i].value > 0 && cauldrons[cauldron_pos].hand[i + 1].value == 0)
            {
                int small = colors_small[pos_small_cauldron][0];
                int pos_hand = i + 1;
                card_mover(cauldrons, small, pos_small_cauldron, pos_hand, cauldron_pos, color_type, players, hand_size, active_player_pos);
                break;
            }
        }

    }
    else if (similar == false && found == false)
    {
        for (int i = 0; i < hand_size; i++)
        {
            if (cauldrons[cauldron_pos].hand[i].value > 0 && cauldrons[cauldron_pos].hand[i + 1].value == 0)
            {
                int pos_hand = i + 1;
                card_mover(cauldrons, smallest, smallest_pos, pos_hand, cauldron_pos, color_type, players, hand_size, active_player_pos);
                break;
            }
        }
    }
}


void read_from_file(char* str[ARRSIZE], int first_three[3], bool* file_input, int* counter_player_hand_deck)
{
    int counter_first_three = 0;

    FILE* fp;
    fopen_s(&fp, "game_state.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file!");
        exit(1);
    }
    char chunk[ARRSIZE];

    size_t len = sizeof(chunk);
    char* line = (char*)malloc(len);


    if (len == NULL)
    {
        printf("Unamble to allocate memory");
        exit(1);
    }
    line[0] = '\0';
    int i = 0;

    while (fgets(chunk, sizeof(chunk), fp) != NULL)
    {
        //Resizes the buffer if necessary
        if (len - strlen(line) < sizeof(chunk))
        {
            len *= 2;
            if ((line = (char*)realloc(line, len)) == NULL)
            {
                perror("Unable to reallocate memory for line buffer");
                free(line);
                exit(1);
            }
        }

        //Appenf the chunk to the end of the line buffer
        strcat(line, chunk);

        //check if line contains '\n'.If it does, process the line of text
        if (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == ':' || (line[strlen(line) - 1] >= 'a' && line[strlen(line) - 1] <= 'z'))
        {

            while (counter_first_three < 3)
            {
                cout << line << endl;
                first_three[counter_first_three] = read_2_numbers(line, strlen(line) - 2);
                counter_first_three++;
                break;
            }

            while (first_three[1] > 0 && counter_first_three >= 3 && (int)line[0] != CHAR_e)
            {
                cout << line << endl;
                str[i] = (char*)malloc(strlen(line));
                strcpy(str[i], line);
                i++;
                (*counter_player_hand_deck)++;
                break;
            }
            line[0] = '\0';

        }
    }

    fclose(fp);
    free(line);
}


void write_to_file(game game_state, player* players, int hand_size, player* cauldrons, int cauldron_size)
{
    FILE* fp;
    fopen_s(&fp, "game_state.txt", "w");
    if (fp == NULL)
    {
        perror("Unable to open file!");
        exit(1);
    }

    //Printing to file
    print_basic_three_file(game_state, fp);
    bool printing_players = true;
    print_det_to_file(players, 2 * game_state.player_no, hand_size, fp, printing_players);
    printing_players = false;
    print_det_to_file(cauldrons, cauldron_size, hand_size, fp, printing_players);

    fclose(fp);
}

int main()
{
    game  game_state;

    //load_game_3a();
     //load_game_3b();

    //game_state = read_input_first_three(game_state);
    char* str[ARRSIZE];
    int first_three[3] = { 0 };
    bool file_input = true;
    int counter_player_hand_deck = 0;
    //Here we read from a file
    read_from_file(str, first_three, &file_input, &counter_player_hand_deck);

    game_state = read_input_first_three(game_state, first_three, file_input);

    player* players = create_players(2 * game_state.player_no);
    player* cauldrons = create_players(MAX_PLAYERS);
    double hand_size = HAND_SIZE;
    int cauldron_size = 0;//Used to get size of cauldron
    cauldron_size = counter_player_hand_deck - (2 * game_state.player_no);//in the case of reading from a file, we know the size of cauldron, the -1 is bcoz of explosion threshold

    create_players_hand(hand_size, 2 * game_state.player_no, players);
    create_players_hand(hand_size, MAX_PLAYERS, cauldrons);
    //char str[ARRSIZE], 
   // char hold[ARRSIZE];

    //assign_char_val(hold);
    //load_game_4a(str,hold);
    //load_game_4b(str);

    initialize_first_column(2 * game_state.player_no, players);
    initialize_first_column(MAX_PLAYERS, cauldrons);

    int** colors_count_players = create_2d_dynamic_array(MAX_COLORS, game_state.player_no);


    int* results = create_dynamic_array(game_state.player_no);
    bool* color_appeared = create_bool_dynamic_array(MAX_COLORS); //checks if a color appeared on a cauldron

    read_hand_pile_file(&game_state, players, &hand_size, cauldrons, cauldron_size, str, colors_count_players, results, color_appeared);

    //colors_no_5(2*game_state.player_no, players, cauldrons); //You have to remove comment from read_values to stop reading greens for this to work


    int card_copy_val = 0, card_copy_color_val = 0;
    int active_player_pos = (game_state.active_player * 2) - 2;//gets accurate positon of player
    int active_player_deck_pos = active_player_pos + 1;//gets accurate positon of player deck


    int** colors_small = create_2d_dynamic_array(MAX_COLORS, WIDTH);

    //initialize values of 2d array to large number i.e 1000
    initialize_large_small(colors_small);


    //Assigns the smallest value of each color in player hand to first column of colors_small according to color, i.e index 0 is BLUE
    color_smallest_phand(players, colors_small, hand_size, active_player_pos);

    int pos_small_cauldron = 0; //used to store copy of index
    int cauldron_pos; //stores copy of position of smalles sum from cauldron
    int index_empty_cauldron; //stores the position of the first empty cauldron if it is there
    bool found = false; //checks for empty cauldron
    //Assigns the sum of the values of cauldrons according to color
    color_smallest_sum_cauldron(cauldrons, cauldron_size, colors_small, hand_size, &pos_small_cauldron, &cauldron_pos, &index_empty_cauldron, &found);



    bool similar = false;//checks if the smallest value is being repeated for other colours
    int smallest = colors_small[0][0];//we assume the first values is the smallest
    int smallest_pos = 0;

    //finds the smallest value of all colors  and checks if that smallest value is repeating
    smallest_val_repeating(colors_small, &similar, &smallest, &smallest_pos);

    //MENU
    int choice = 0;
    cout << "Choose according to number:" << endl;
    cout << "1: move the first card to pile with explosion checker active" << endl;
    cout << "2: move the smallest card with explosion checker active" << endl;
    cout << "3: Check the hands of players if hand is wrong or 2 different colors " << endl;
    cout << "4: Check the hands of players for odpornosc " << endl;
    cout << "WHAT DO YOU WANT TO DO?" << endl;
    cin >> choice;

    switch (choice)
    {
    case 1://we are moving the first card in player hand to pile
        move_first_card_to_pile(players, cauldrons, card_copy_val, card_copy_color_val, active_player_pos, active_player_deck_pos, hand_size, game_state.explosion_threshold, cauldron_size);
        break;
    case 2://Moves the smallest card to avoid explosion
        move_smallest_card(found, similar, color_appeared, colors_small, players, cauldrons, cauldron_pos, hand_size, active_player_pos, pos_small_cauldron, index_empty_cauldron, smallest, smallest_pos);
        break;
    case 3://We check the hands of players
        check_player_hand_and_pile(game_state.player_no, cauldron_size, hand_size, players, cauldrons);
        break;
    case 4://We check for odpornosc
        check_repeating_high_val(game_state.player_no, colors_count_players, results);
        break;
    default:
        break;
    }



    game_state.active_player++;//changes the active player to next player
    game_state.active_player = (game_state.active_player > game_state.player_no) ? 1 : game_state.active_player;

    write_to_file(game_state, players, hand_size, cauldrons, cauldron_size);


    //Clearing of dynamic memory allocated
    for (int i = 0; i < MAX_COLORS; i++)
    {
        free(colors_count_players[i]);
        free(colors_small[i]);
    }
    free(results);
    free(color_appeared);
    free(players);
    free(cauldrons);
    return 0;
}
