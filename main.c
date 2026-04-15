#include "trielib.h"
#include "veclib.h"
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

int main(){
    TrieNode* root=create_node();
    
    //Open dictionary.txt in read mode and insert each word processing each line

    FILE *fin = (FILE *)fopen("dict.txt" , "r");
    char word[100];
    while(fgets(word , 99 , fin) != NULL){
        if(word[strlen(word)-1] == '\n'){
            word[strlen(word)-1] = '\0';
        }
        word[strlen(word)]='\0';

        insert(root , word);    
    }
    //All words loaded in trie
    fclose(fin);
    //close file pointer

    //ncurses references taken from https://github.com/mcdaniel/curses_tutorial
    
    initscr();
    
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    refresh();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // Measure the current physical screen

    WINDOW *bg = newwin((95*max_y)/100, (99*max_x)/100,0,0);
    box(bg, 0, 0);    
    wrefresh(bg);

    WINDOW *search_win;
    char buff[50] = {0};
    char buff_m1[50] = {0};
    char buff_m2[50] = {0};
    char buff_m3[50] = {0};
    int current_mode=4;

    int selected_index = 0;
    int scroll_offset = 0;

    // int ln_m3 = 3;
    // int ln_search_m3 = 3;

    int rno = 4;
    int cno = 1;

    while(1){

        if(current_mode == 3){
    
            mvwprintw(bg , 1 , 1 , "Search Mode\n");
            box(bg, 0, 0);
            box(search_win , 0 , 0);
            wrefresh(bg);
            touchwin(search_win);
            wrefresh(search_win);

        }
        else if(current_mode == 1){

            mvwprintw(bg , 1 , 1 , "Insert Mode\n");
            box(bg, 0, 0);
            wrefresh(bg);

        }
        else if(current_mode == 2){

            mvwprintw(bg , 1 , 1 , "Delete Mode\n");
            box(bg, 0, 0);
            wrefresh(bg);

        }
        else if(current_mode == 4){
            
            int h, w;
            getmaxyx(bg, h, w);

            mvwprintw(bg , 0.1*h , 0.1*w , "Dictionary Statistics\n");
            mvwprintw(bg , 0.1*h , 0.35*w , "F1: Insert\n");
            mvwprintw(bg , 0.1*h , 0.5*w , "F2: Delete\n");
            mvwprintw(bg , 0.1*h , 0.65*w , "F3: Search\n");
            mvwprintw(bg , 0.1*h , 0.8*w , "F4: Statistics\n");
            mvwprintw(bg, h * 0.2, w * 0.1, "Word Count:   %d", wordcount(root));
            mvwprintw(bg, h * 0.3, w * 0.1, "Max Word Length:    %d", max_depth(root)-1);
            mvwprintw(bg, h * 0.4, w * 0.1, "Data Occupied By Nodes:   %.3f MB", (float)node_count(root)*(float)sizeof(TrieNode)/(1024.0f * 1024.0f));
            mvwprintw(bg, h * 0.5, w * 0.1, "Alpabetical Distribution:");
            int distri[26];
            alpha_distri(root, distri);
            for (int i=0; i<26; i++) {
                int col = (i%3);
                int row = (i/3);
                int y = (h*0.6) + row;
                int x = (w*0.1) + (col*(w/3));
        
                mvwprintw(bg, y, x, "%c: %d", 'a' + i, distri[i]);
            }
            box(bg, 0, 0);
            wrefresh(bg);

        }
        int ch = getch();
        if(ch == KEY_RESIZE){
            delwin(bg);
            getmaxyx(stdscr, max_y, max_x);
            bg = newwin((95*max_y)/100, (99*max_x)/100,0,0);
            box(bg, 0, 0);
            wrefresh(bg);
        }
        else if(ch == KEY_F(1)){
            // inserting a new word to the trie
            current_mode = 1;
            
            if(search_win!=NULL){
                delwin(search_win);
                search_win=NULL;
            }
            werase(bg);
            box(bg, 0, 0);
            wrefresh(bg);

            mvwprintw(bg , 3 , 1 , "The word to be inserted is:\n");
            box(bg, 0, 0);
            wrefresh(bg);          

        }
        else if(ch == KEY_F(2)){
            // deleting a word from trie
            current_mode = 2;

            if(search_win!=NULL){
                delwin(search_win);
                search_win=NULL;
            }
            werase(bg);
            box(bg, 0, 0);
            wrefresh(bg);

            mvwprintw(bg , 3 , 1 , "The word to be deleted is:\n");
            box(bg, 0, 0);
            wrefresh(bg);     
            
            rno = 4;
            cno = 1;
        }
        else if(ch == KEY_F(3)){
            //searching
            buff_m3[0]='\0';
            current_mode = 3;

            werase(bg);
            box(bg, 0, 0);
            wrefresh(bg);
            search_win = newwin(20, (max_x*39)/100, 3, (max_x*60)/100);
            touchwin(search_win);
            wrefresh(search_win);

            mvwprintw(bg , 3 , 1 , "Typing...\n");
            box(bg, 0, 0);
            wrefresh(bg); 
            touchwin(search_win);
            wrefresh(search_win); 
            
            mvwprintw(search_win , 1 , 1 , "Search Results:\n");
            box(search_win , 0 , 0);
            touchwin(search_win);
            wrefresh(search_win);
            
            rno = 4;
            cno = 1;

        }
        else if(ch == KEY_F(4)){
            current_mode = 4;
            
            if(search_win!=NULL){
                delwin(search_win);
                search_win=NULL;
            }
            werase(bg);
            box(bg, 0, 0);
            wrefresh(bg);      
        }
        else if(ch >= 'a' && ch <= 'z'){
            if(current_mode == 1){
                int temp_sz = strlen(buff_m1);
                buff_m1[temp_sz] = (char)ch;
                buff_m1[temp_sz + 1] = '\0';

                mvwprintw(bg , 4 , 1 , "%s\n" , buff_m1);
                box(bg, 0, 0);
                wrefresh(bg);  
                            
            }
            else if(current_mode == 2){
                int temp_sz = strlen(buff_m2);
                buff_m2[temp_sz] = (char)ch;
                buff_m2[temp_sz + 1] = '\0';

                mvwprintw(bg , 4 , 1 , "%s\n" , buff_m2);
                box(bg, 0, 0);
                wrefresh(bg);
            }
            if(current_mode == 3){
                int temp_sz = strlen(buff_m3);
                buff_m3[temp_sz] = (char)ch;
                buff_m3[temp_sz + 1] = '\0';

                // mvwprintw(bg , 4 , 1 , "%s\n" , buff_m3);
                // box(bg, 0, 0);
                // wrefresh(bg);

                vector *results = prefix_search(root , buff_m3);

                int n = vec_size(results);               
                
                
                mvwprintw(bg , 3 , 1 , "Typing...\n");
                box(bg, 0, 0);
                wrefresh(bg);
                touchwin(search_win);
                wrefresh(search_win);

                if(cno + strlen(buff_m3) >= (60*max_x)/100){
                    //clearing out the text at old location
                    for(int ii = 0 ; ii<strlen(buff_m3)-1 ; ii++){
                        mvwprintw(bg , rno , cno , " ");
                        cno++;
                        box(bg, 0, 0);
                        wrefresh(bg);
                        touchwin(search_win);
                        wrefresh(search_win);
                    }
                    rno++;
                    cno = 1;
                    mvwprintw(bg , rno , cno , "%s\n" , buff_m3);
                    box(bg, 0, 0);
                    wrefresh(bg);
                    touchwin(search_win);
                    wrefresh(search_win);
                }
                else{
                    mvwprintw(bg , rno , cno , "%s\n" , buff_m3);
                    box(bg, 0, 0);
                    wrefresh(bg);
                    touchwin(search_win);
                    wrefresh(search_win);
                }
                                
                box(bg, 0, 0);
                wrefresh(bg);
                touchwin(search_win);
                wrefresh(search_win);
                
                mvwprintw(search_win , 1 , 1 , "Search Results:\n");
                box(search_win , 0 , 0);
                touchwin(search_win);
                wrefresh(search_win);

                if(n==0){
                    //printf("NO RESULTS FOUND\n");
                    werase(search_win);
                    mvwprintw(search_win , 1 , 1 , "Search Results:\n");
                    box(search_win , 0 , 0);

                    touchwin(search_win);
                    wrefresh(search_win);

                    mvwprintw(search_win,2,1, "NO RESULTS FOUND\n");
                    box(search_win, 0, 0);
                    touchwin(search_win);
                    wrefresh(search_win);
                }
                else{
                    werase(search_win);
                    mvwprintw(search_win , 1 , 1 , "Search Results:\n");
                    box(search_win , 0 , 0);

                    touchwin(search_win);
                    wrefresh(search_win);

                    for(int i=0;i<n && i<15 ; i++){
                        mvwprintw(search_win , 2+i , 1 , vec_at(results , i));
                        box(search_win, 0, 0);
                        touchwin(search_win);
                        wrefresh(search_win);
                    }
                }     
            
                vec_free(&results);

            }
        }
        else if(ch == '\n' || ch == '\r' || ch == KEY_ENTER){
            if(current_mode == 1){
                insert(root , buff_m1);
                mvwprintw(bg ,5,1, "SUCCESSFULLY INSERTED: %s" , buff_m1);
                box(bg, 0, 0);
                
                wrefresh(bg);
                napms(5000);

                refresh();
                werase(bg);
                box(bg, 0, 0);
                wrefresh(bg);
                buff_m1[0] = '\0';
            }
            else if(current_mode == 2){
                mvwprintw(bg ,5,1, "Do you want to delete \" %s \": Y or N \n" , buff_m2);
                box(bg, 0, 0);
                wrefresh(bg);
                napms(3000);

                refresh();
            }
            else{
                werase(search_win);
                box(search_win, 0, 0);
                touchwin(search_win);
                wrefresh(search_win);

                if(current_mode == 3){
                    cno += strlen(buff_m3);
                    buff_m3[0] = '\0';
                    if(cno != 0){
                        mvwprintw(bg , rno , cno , "%c" , ch);
                        wrefresh(bg);
                        touchwin(search_win);
                        wrefresh(search_win);
                    }
                    else{
                        mvwprintw(bg , rno , 0 , "%c" , ch);
                        wrefresh(bg);
                        touchwin(search_win);
                        wrefresh(search_win);
                    }

                    if(ch == '\n'){
                        rno++;
                        cno = 1;
                    }
                    else if(ch == '\t'){
                        cno += 4;
                    }
                    else if(ch == ' '){
                        cno++;
                    }
                    else{
                        mvwprintw(bg , rno , cno , "%c" , ch);
                        cno++;
                    }
                    box(bg , 0 , 0);
                    wrefresh(bg);
                    touchwin(search_win);
                    wrefresh(search_win);
                }
            }
        }
        else if(ch == KEY_BACKSPACE || ch == 127 || ch == '\b'){
            

            if(current_mode == 1){
                int temp_sz = strlen(buff_m1);
                buff_m1[temp_sz-1] = '\0';
                mvwprintw(bg , 4 , 1 , "%s\n" , buff_m1);
                box(bg, 0, 0);
                wrefresh(bg);
            }
            else if(current_mode == 2){
                int temp_sz = strlen(buff_m2);
                buff_m2[temp_sz-1] = '\0';
                mvwprintw(bg , 4 , 1 , "%s\n" , buff_m2);
                box(bg, 0, 0);
                wrefresh(bg);
            }
            else if(current_mode == 3){
                int temp_sz = strlen(buff_m3);
                buff_m3[temp_sz-1] = '\0';
                // mvwprintw(bg , 4 , 1 , "%s\n" , buff_m3);
                // box(bg, 0, 0);
                // wrefresh(bg);

                vector *results = prefix_search(root , buff_m3);

                int n = vec_size(results);               
                
                
                mvwprintw(bg , 3 , 1 , "Typing...\n");
                box(bg, 0, 0);
                wrefresh(bg);
                touchwin(search_win);
                wrefresh(search_win);
                if(cno + strlen(buff_m3) >= (60*max_x)/100){
                    //clearing out the text at old location
                    for(int ii = 0 ; ii<strlen(buff_m3)-1 ; ii++){
                        mvwprintw(bg , rno , cno , " ");
                        cno++;
                        box(bg, 0, 0);
                        wrefresh(bg);
                        touchwin(search_win);
                        wrefresh(search_win);
                    }
                    rno++;
                    cno = 1;
                    mvwprintw(bg , rno , cno , "%s\n" , buff_m3);
                    box(bg, 0, 0);
                    wrefresh(bg);
                    touchwin(search_win);
                    wrefresh(search_win);
                }
                else{
                    mvwprintw(bg , rno , cno , "%s\n" , buff_m3);
                    box(bg, 0, 0);
                    wrefresh(bg);
                    touchwin(search_win);
                    wrefresh(search_win);
                }

                box(bg, 0, 0);
                wrefresh(bg);
                touchwin(search_win);
                wrefresh(search_win);
                
                mvwprintw(search_win , 1 , 1 , "Search Results:\n");
                box(search_win , 0 , 0);
                touchwin(search_win);
                wrefresh(search_win);

                if(n==0){
                    //printf("NO RESULTS FOUND\n");
                    werase(search_win);
                    mvwprintw(search_win , 1 , 1 , "Search Results:\n");
                    box(search_win , 0 , 0);

                    touchwin(search_win);
                    wrefresh(search_win);

                    mvwprintw(search_win,2,1, "NO RESULTS FOUND\n");
                    box(search_win, 0, 0);
                    touchwin(search_win);
                    wrefresh(search_win);
                }
                else{
                    werase(search_win);
                    mvwprintw(search_win , 1 , 1 , "Search Results:\n");
                    box(search_win , 0 , 0);

                    touchwin(search_win);
                    wrefresh(search_win);
                    
                    for(int i=0;i<n && i<15 ; i++){
                        mvwprintw(search_win , 2+i , 1 , vec_at(results , i));
                        box(search_win, 0, 0);
                        touchwin(search_win);
                        wrefresh(search_win);
                    }
                }     
            
                vec_free(&results);

            }
        }
        else if(ch == 'Y'){
            if(current_mode == 2){
                if(search(root , buff_m2) == 1){
                    delete(root , buff_m2);
                    mvwprintw(bg , 6 , 1, "%s\n" , "Y");
                    box(bg, 0, 0);
                    wrefresh(bg);
                    mvwprintw(bg ,6+1,1, "Deleted \" %s \"\n" , buff_m2);
                    box(bg, 0, 0);
                    wrefresh(bg);
                    buff_m2[0] = '\0';

                    napms(5000);

                    refresh();
                    werase(bg);
                    box(bg, 0, 0);
                    wrefresh(bg);
                    buff_m2[0] = '\0';
                }
                else{
                    mvwprintw(bg , 6 , 1, "%s\n" , "Y");
                    box(bg, 0, 0);
                    wrefresh(bg);
                    mvwprintw(bg ,6+1,1, "Word \" %s \" is not found\n" , buff_m2);
                    box(bg, 0, 0);
                    wrefresh(bg);

                    napms(5000);

                    refresh();
                    werase(bg);
                    box(bg, 0, 0);
                    wrefresh(bg);
                    buff_m2[0] = '\0';
                }
                
                buff_m2[0] = '\0';
            }
        }
        else if(ch == 'N'){
            if(current_mode == 2){
                mvwprintw(bg , 6 , 1, "%s\n" , "N");
                box(bg, 0, 0);
                wrefresh(bg);
                mvwprintw(bg ,6+1,1, "Did not Delete \" %s \"\n" , buff_m2);
                box(bg, 0, 0);
                wrefresh(bg);
                buff_m2[0] = '\0';

                napms(5000);

                refresh();
                werase(bg);
                box(bg, 0, 0);
                wrefresh(bg);
                buff_m2[0] = '\0';
            }
        }
        else if(ch == '\n' || ch == '\t' || ch == ' ' || ch == '\'' || ch == '\"' || ch == ',' || ch == '.' || ch == ';'){
            
            werase(search_win);
            box(search_win, 0, 0);
            touchwin(search_win);
            wrefresh(search_win);

            if(current_mode == 3){
                cno += strlen(buff_m3);
                buff_m3[0] = '\0';
                if(cno != 0){
                    mvwprintw(bg , rno , cno , "%c" , ch);
                    wrefresh(bg);
                    touchwin(search_win);
                    wrefresh(search_win);
                }
                else{
                    mvwprintw(bg , rno , 0 , "%c" , ch);
                    wrefresh(bg);
                    touchwin(search_win);
                    wrefresh(search_win);
                }

                if(ch == '\n'){
                    rno++;
                    cno = 1;
                }
                else if(ch == '\t'){
                    cno += 4;
                }
                else if(ch == ' '){
                    cno++;
                }
                else{
                    mvwprintw(bg , rno , cno , "%c" , ch);
                    cno++;
                }
                box(bg , 0 , 0);
                wrefresh(bg);
                touchwin(search_win);
                wrefresh(search_win);
            }

        }
               
    }

    

    endwin();
}