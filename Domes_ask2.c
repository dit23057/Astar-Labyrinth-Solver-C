// Domes_ask2.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define MIN 10
#define MAX 20

struct square{
    int row;
    int col;
    
    int f;
    
    int g;
    int d;
    
    struct square* next;
    struct square* prev;
};

void  delete_list(struct square* head){
    struct square* node = NULL;
    
    if (head == NULL) return;
    
    while (head != NULL){
        node = head->next;
        free(head);
        head = node;
    }
}

void print(char** labyrinth, int rows, int cols){
    int i, j;
    
    printf("\n\nThe labyrinth is:\n");
    
    for (i = 0 ; i < rows ; i++){
        for (j = 0 ; j < cols ; j++){
            printf("%c", labyrinth[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

struct square* create(int row, int col, int g, int d){
    struct square* newSquare = (struct square*)malloc(sizeof(struct square));
    if (newSquare == NULL){
        printf("malloc error\n");
        exit(1);
    }
    
    newSquare->row = row;
    newSquare->col = col;
    newSquare->g   = g;
    newSquare->d   = d;
    newSquare->f   = g + d;
    
    newSquare->next = NULL;
    newSquare->prev = NULL;
    
    
    return newSquare;
}

// αφαιρώ (χωρίς να σβήσω) τον πρώτο κόμβο από την λίστα
// απλώς κόβω τους δείκτες
struct square* cut_head(struct square* head){
    struct square* newHead = NULL;
    
	// αν δεν υπάρχει λίστα eπιστρέφω NULL
    if (head == NULL) return NULL;
    
	// βρίσκω το επόμενο κεφάλι
    newHead = head->next;
    
	// αν δεν υπάρχει επόμενος κόμβος, επιστρέφω NULL
    if (newHead == NULL) return NULL;
    
	// αλλιώς αν υπάρχει δεύτερος κόμβος
	// κόβω τους δείκτες
    newHead->prev = NULL;
    head->next    = NULL;
    
	// επιστρέφω τον δεύτερο κόμβο 
	// ως νέα κεφαλή
    return newHead;
}


// εισάγω τον νέο κόμβο στην ταξινομημένη του θέση στη λίστα
struct square* insert(struct square* head, struct square* newSquare){
    struct square* node = head;
    struct square* prevnode = NULL;
    
	// αν η λίστα είναι κενή, ο νέος κόμβος 
	// είναι η κεφαλή της λίστας
    if (head == NULL) return newSquare;
    
	// αλλιώς πρέπει να βρω που θα προσθέσω τον νέο κόμβο
	
	// θέλω να βρω το node που θα έχει μεγαλύτερη τιμή 
	// από τον νέο κόμβο και το prevnode που θα έχει 
	// μικρότερη τιμή από τον νέο κόμβο. Ο νέος κόμβος
	// θέλω να μπεί μετά το prevnode και πριν το node
    while (node != NULL && node->f < newSquare->f){
        prevnode = node;
        node     = node->next;
    }
    
	// αν τελικά το node δεν υπάρχει, ο νέος κόμβος
	// πρέπει να προστεθεί στην ουρά της λίστας
    if (node == NULL){
        prevnode->next = newSquare;
        newSquare->prev = prevnode;
        
    }
	
    // όταν έχω μόνο την κεφαλή, δεν υπάρχει προηγούμενος κόμβος
	// άρα θα πρέπει να ισχύει ότι node->prev == NULL και αν ο 
	// νέος κόμβος είναι ίσος ή μικρότερος της κεφαλής, θα πρέπει 
	// ο νέος κόμβος να γίνει κεφαλή. 
	// Άρα πρέπει ο νέος κόμβος να μπει πριν την κεφαλή
    else if (node->prev == NULL && newSquare->f <= node->f){
        newSquare->next = node;
        node->prev = newSquare;
        head = newSquare;
    }
	
	// αλλιώς αν υπάρχει ο επόμενος κόμβος και ο προηγούμενος
	// ο νέος κόμβος θα πρέπει να μπει ανάμεσά τους
    else if (node != NULL && node->f >= newSquare->f &&
            prevnode != NULL && prevnode->f <= newSquare->f)
    {
        prevnode->next  = newSquare;
        newSquare->next = node;
        node->prev      = newSquare;
        newSquare->prev = prevnode;
    }
	
	// επιστρέφω την κεφαλή της ταξινομημένης λίστας
    return head;
}

int calculate_Manhattan_distance(int current_row, int current_col,
                            int target_row, int target_col)
{
    int row_distance = abs(current_row - target_row);
    int col_distance = abs(current_col - target_col);
    
    return row_distance + col_distance;
}

// δημιουργεί έναν νέο κόμβο για τη θέση που 
// βρίσκεται πάνω από την τρέχουσα θέση
struct square* get_north(struct square* currentSquare, char** labyrinth,
                            int rows, int cols, int target_row)
{
    int current_row = currentSquare->row;
    int upper_row, col, d;
    struct square* n = NULL;
    
	// αν η τρέχουσα γραμμή είναι η πρώτη, 
	// δεν υπάρχει άλλη γραμμή από πάνω της
	// επομένως επιστρέφω NULL
    if (current_row == 0) return NULL;
    
    upper_row = current_row - 1;
    col = currentSquare->col;
    
	// αν βόρεια υπάρχει εμπόδιο, επιστρέφω NULL
    if (labyrinth[upper_row][col] == '*') return NULL;
    
    d = calculate_Manhattan_distance(upper_row, col, target_row, cols -1);
    
	// δημιουργώ νέο κόμβο με τα νέα στοιχεία
    n = create(upper_row, col, currentSquare->g + 1, d);
                            
    return n;
}

struct square* get_south(struct square* currentSquare, char** labyrinth,
                            int rows, int cols, int target_row)
{
    int current_row = currentSquare->row;
    int lower_row, col, d;
    struct square* s = NULL;
    
    if (current_row == rows - 1) return NULL;
    
    lower_row = current_row + 1;
    col = currentSquare->col;
    
    if (labyrinth[lower_row][col] == '*') return NULL;
    
    d = calculate_Manhattan_distance(lower_row, col, target_row, cols -1);
    
    s = create(lower_row, col, currentSquare->g + 1, d);
                            
    return s;
}

struct square* get_east(struct square* currentSquare, char** labyrinth,
                            int rows, int cols, int target_row)
{
    int current_col = currentSquare->col;
    int right_col, row, d;
    struct square* e = NULL;
    
    if (current_col == cols - 1) return NULL;
    
    right_col = current_col + 1;
    row = currentSquare->row;
    
    if (labyrinth[row][right_col] == '*') return NULL;
    
    d = calculate_Manhattan_distance(row, right_col, target_row, cols -1);
    
    e = create(row, right_col, currentSquare->g + 1, d);
                            
    return e;
}

struct square* get_west(struct square* currentSquare, char** labyrinth,
                            int rows, int cols, int target_row)
{
    int current_col = currentSquare->col;
    int left_col, row, d;
    struct square* w = NULL;
    
    if (current_col == 0) return NULL;
    
    left_col = current_col - 1;
    row = currentSquare->row;
    
    if (labyrinth[row][left_col] == '*') return NULL;
    
    d = calculate_Manhattan_distance(row, left_col, target_row, cols -1);
    
    w = create(row, left_col, currentSquare->g + 1, d);
                            
    return w;
}
  
  
// αυτή η συνάρτηση εμφανίζει την ταξινομημένη ουρά/λίστα
// την χρησιμοποίησα για να διορθώσω τον αλγόριθμο
void printQueue(struct square* head){
    struct square *node = NULL;
    
    printf("The queue is: ");
    
    if (head == NULL){
        printf("empty queue!\n");
        return;
    }
    
    node = head;
    
    while (node != NULL){
        printf("(%d,%d:%d) ", node->row, node->col, node->f);
        node = node->next;
    }
    printf("\n");
}

// εμφανίζει την βέλτιστη διαδρομή
void print_g_score(int** g_scores, char** labyrinth, int rows, int cols, 
														int entrance, int target)
{
    int i, j, current_row, current_col, current_g, next_g;
	
	// σβήνω το 'R' από όλες τις θέσης, εκτός από την είσοδο
    for (i = 0 ; i < rows ; i++){
        for (j = 0 ; j < cols ; j++){
			if (i == entrance && j == 0) continue;
			if (labyrinth[i][j] == 'E')  continue;
			
			if (labyrinth[i][j] == 'R') labyrinth[i][j] = '-';
		}
	}
	
	// ξεκινάω από την έξοδο
	current_row = target;
	current_col = cols - 1;
	current_g   = g_scores[current_row][current_col];
	
	// αυτή είναι η επόμενη τιμή που θα πρέπει 
	// να βρω σε κάποια γειτονική θέση
	next_g		= current_g - 1;
	
	// όσο δεν έχω φτάσει μέχρι την είσοδο
	while (g_scores[current_row][current_col] > 0){
		
		// υπάρχει βόρεια θέση, που έχει τιμή next_g
		// θα πρέπει να κινηθώ βόρεια
		if (current_row > 0 && 
				g_scores[current_row - 1][current_col] == next_g)
		{
			current_row--;
			labyrinth[current_row][current_col] = 'R';
		}
		else if (current_row < rows - 1 && 
				g_scores[current_row + 1][current_col] == next_g)
		{
			current_row++;
			labyrinth[current_row][current_col] = 'R';
		}
		else if (current_col < cols - 1 && 
				g_scores[current_row][current_col + 1] == next_g)
		{
			current_col++;
			labyrinth[current_row][current_col] = 'R';
		}
		else if (current_col > 0 && 
				g_scores[current_row][current_col - 1] == next_g)
		{
			current_col--;
			labyrinth[current_row][current_col] = 'R';
		}
		
		// πλέον βρίσκομαι στην επόμενη θέση
		current_g   = g_scores[current_row][current_col];
		
		// και θα ψάχνω για την επόμενη τιμή
		next_g		= current_g - 1;
	}
	
	
	printf("Backtracking!\n");
	print(labyrinth, rows, cols);
}

void erotima1(char** labyrinth, int rows, int cols, int entrance, int target, int** g_scores)
{
    int round = 1, current_row, current_col;
    int d = calculate_Manhattan_distance(entrance, 0, target, cols-1);
    struct square* head = create(entrance, 0, 0, d);
    struct square* minSquare = NULL;
    struct square* n = NULL;
    struct square* s = NULL;
    struct square* e = NULL;
    struct square* w = NULL;
    
    // όσο υπάρχουν στοιχεία στην ταξινομημένη λίστα/ουρά
    while (head != NULL){
		// κρατάω τον πρώτο/ελάχιστο κόμβο
        minSquare = head;
        current_row = minSquare->row;
        current_col = minSquare->col;
        
		// κρατάω την τιμή της θέσεις και στον πίνακα που 
		// θα χρησιμοποιήσω στο τέλος για το backtrack
        if (minSquare->g < g_scores[current_row][current_col]) {
            g_scores[current_row][current_col] = minSquare->g;
        }
        
        /*
        if (round % 10 == 0) {
            printQueue(head);
            scanf("%d", &a);
        }
        */
        
		// ο κόμβος φεύγει από τη λίστα
        head = cut_head(head);
        
		// αν βρέθηκα στην έξοδο, τερματισμός!
        if (minSquare->d == 0) break;
        
		// σημειώνω ότι έχω επισκευτεί την θέση
        labyrinth[current_row][current_col] = 'R';
        
        printf("\nRound %d:", round++);
        print(labyrinth, rows, cols);
        
		// βρίσκω γειτονικές θέσεις
        n = get_north(minSquare, labyrinth, rows, cols, target);
        s = get_south(minSquare, labyrinth, rows, cols, target);
        e = get_east(minSquare, labyrinth, rows, cols, target);
        w = get_west(minSquare, labyrinth, rows, cols, target);
        
		// αν υπάρχει γειτονική θέση που δεν έχω επισκευτεί, 
		// τις προσθέτω στην ταξινομημένη λίστα
		// https://en.wikipedia.org/wiki/A*_search_algorithm
        if (n != NULL && labyrinth[current_row-1][current_col] != 'R') 
														head = insert(head, n);
        if (s != NULL && labyrinth[current_row+1][current_col] != 'R') 
														head = insert(head, s);
        if (e != NULL && labyrinth[current_row][current_col+1] != 'R') 
														head = insert(head, e);
        if (w != NULL && labyrinth[current_row][current_col-1] != 'R') 
														head = insert(head, w);
        
		// διαγραφή κόμβου που βγάλαμε από την λίστα
        if (minSquare != NULL) free(minSquare);
    }
    
    
    // εμφάνιση αποτελεσμάτων
    if (minSquare->d == 0){
        printf("Exit found!\n\n");
		print_g_score(g_scores, labyrinth, rows, cols, entrance, target);
    }
    else {
        printf("no way out!\n\n");
    }
	
    if (head != NULL){
        delete_list(head);
    }
}

char** create_labyrinth(int *rows, int* cols, int *entrance, int *target){
    int random_col, random_row;
    int i, j, numberOfObstacles, percentageOfObstacles;
    char **labyrinth = NULL;
    
    // αποφασίζω στην τύχη τις διαστάσεις
    *rows = rand() % (MAX - MIN + 1) + MIN;
    *cols = rand() % (MAX - MIN + 1) + MIN;
    
	// κατασκευή λαβύρινθου
    labyrinth = (char**)malloc(sizeof(char*) * (*rows));
    if (labyrinth == NULL){
        printf("malloc error\n");
        exit(1);
    }
    
    
    for (i = 0 ; i < (*rows) ; i++){
        labyrinth[i] = (char*)malloc(sizeof(char) * (*cols));
        if (labyrinth[i] == NULL){
            printf("malloc error\n");
            exit(1);
        }
        for (j = 0 ; j < (*cols) ; j++){
            labyrinth[i][j] = '-';
        }
    }
    
	// βρίσκω στην τύχη την είσοδο
    *entrance = -1;
    while (*entrance == -1){
        random_row = rand() % *rows;
        if (labyrinth[random_row][0] == '-'){
            *entrance = random_row;
            labyrinth[random_row][0] = 'R';
        }
    }
    
	
	// βρίσκω στην τύχη την έξοδο
    *target = -1;
    while (*target == -1){
        random_row = rand() % *rows;
        printf("random row: %d, ", random_row);
        if (labyrinth[random_row][(*cols) - 1] == '-'){
            *target = random_row;
            labyrinth[random_row][(*cols) - 1] = 'E';
        }
        else {
            printf("-> %c\n", labyrinth[random_row][(*cols) - 1]);
        }
    }
    
	//αποφασίζω ποσοστό εμποδίων
    percentageOfObstacles = rand() % 41 + 10;
    numberOfObstacles = *rows * *cols * percentageOfObstacles / 100;
    
    printf("Rows: %d, Cols: %d, Percentage: %d%%, Obstacles: %d\n",
        *rows, *cols, percentageOfObstacles, numberOfObstacles);
    
	// εισαγωγή εμποδίων σε τυχαίες θέσεις
    i = 0;
    while (i < numberOfObstacles){
        random_col = rand() % *cols;
        random_row = rand() % *rows;
        
        if (labyrinth[random_row][random_col] == '-'){
            labyrinth[random_row][random_col] = '*';
            i++;
        }
    }
    
    
    
    printf("Entrance from row: %d\n", (*entrance) + 1);
    
    
    return labyrinth;
}

void delete_labyrinth(char** labyrinth, int rows){
    int i;
    
    for (i = 0 ; i < rows ; i++){
        free(labyrinth[i]);
    }
    free(labyrinth);
}

void delete_g_scores(int** g_scores, int rows){
    int i;
    
    for (i = 0 ; i < rows ; i++){
        free(g_scores[i]);
    }
    free(g_scores);
}

// ο πίνακας αυτός θα χρησιμοποιηθεί στο backtrack
int** create_g_scores_array(int rows, int cols){
    int i, j;
    int** a = (int**)malloc(sizeof(int*) * rows);
    if (a == NULL){
        printf("malloc error\n");
        exit(1);
    }
    
    for (i = 0 ; i < rows ; i++){
        a[i] = (int*)malloc(sizeof(int) * cols);
        if (a[i] == NULL){
            printf("malloc error\n");
            exit(1);
        }
        // αρχικά όλες οι θέσεις έχουν άπειρη τιμή
		// και καθώς θα εκτελώ τον αλγόριθμο, η 
		// τιμή κάθε θέσης θα μειώνεται και θα γίνεται
		// η σωστή σύμφωνα με τους υπολογισμούς
        for (j = 0 ; j < cols ; j++) a[i][j] = rows * cols + 1000;
    }
    return a;
}

int main(){
    int rows, cols, entrance, target;
    char **labyrinth = NULL;
    int** g_scores = NULL;
    
    srand(time(NULL));
    
    labyrinth = create_labyrinth(&rows, &cols, &entrance, &target);
    
    g_scores = create_g_scores_array(rows, cols);
    
    erotima1(labyrinth, rows, cols, entrance, target, g_scores);
    
    delete_labyrinth(labyrinth, rows);
	delete_g_scores(g_scores, rows);
    
    return 0;
}
