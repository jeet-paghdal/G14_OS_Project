#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


static int flag_lines=0;   // -l 
static int flag_words=0;   // -w 
static int flag_chars=0;   // -c 

// Count structure
typedef struct{
    long lines;
    long words;
    long chars;
}Counts;

//count_stream():Reads from an already-opened FILE* and fills a Counts struct.
static void count_stream(FILE *fp, Counts *out){
    out->lines=0;
    out->words=0;
    out->chars=0;

    int ch;
    int in_word=0; // are we currently inside a word?

    while((ch=fgetc(fp))!=EOF){
        out->chars++;

        // Count newlines
        if(ch=='\n'){
            out->lines++;
        }

        //Count words: a word starts on transition from whitespace->non-ws
        if(isspace(ch)){
            in_word=0;
        }else{
            if(!in_word){
                out->words++;
                in_word=1;
            }
        }
    }
}

//print_counts():Prints whichever counts are requested, then the label.
static void print_counts(const Counts *c, const char *label){
    //If no specific flag was set, print all three
    int print_all=(!flag_lines && !flag_words && !flag_chars);

    if(print_all || flag_lines)printf("%8ld", c->lines);
    if(print_all || flag_words)printf("%8ld", c->words);
    if(print_all || flag_chars)printf("%8ld", c->chars);

    if(label != NULL)printf("  %s", label);
    printf("\n");
}

// process_file():Opens a named file, counts, prints, and accumulates into total.
// Returns 0 on success, 1 on error.
static int process_file(const char *filename, Counts *total){
    FILE *fp=fopen(filename, "r");
    if(fp==NULL){
        fprintf(stderr, "custom_wc: %s: No such file or directory\n", filename);
        return 1;
    }

    Counts c;
    count_stream(fp, &c);
    fclose(fp);

    print_counts(&c, filename);

// Accumulate into totals
    total->lines+=c.lines;
    total->words+=c.words;
    total->chars+=c.chars;

    return 0;
}

// argc:number of arguments , argv:array of arguments
int main(int argc, char *argv[]){
    int errors=0;

    int i;
    int first_file=1;   //index of first non-option argument(argv[0]:command name)

    for(i=1;i<argc;i++){
        if(argv[i][0]=='-' && argv[i][1]!='\0'){
            //Could be a combined flag like -lw 
            int j;
            for(j=1;argv[i][j]!='\0';j++){
                switch(argv[i][j]){
                    case 'l':flag_lines=1;break;
                    case 'w':flag_words=1;break;
                    case 'c':flag_chars=1;break;
                    default:
                        fprintf(stderr, "custom_wc: invalid option -- '%c'\n", argv[i][j]);
                        fprintf(stderr, "Usage: custom_wc [-lwc] [FILE...]\n");
                        return 1;
                }
            }
        }else{
            //First non-option argument:rest are file names
            first_file = i;
            break;
        }
        first_file = i + 1;  //advance past all options
    }

    Counts total={0, 0, 0};
    int file_count=argc-first_file; // how many file arguments remain

    if(file_count==0){
        //No files given — read from stdin
        Counts c;
        count_stream(stdin, &c);
        print_counts(&c, NULL); // no label for stdin
    }else{
        // Process each file
        for(i=first_file;i<argc;i++){
            errors+=process_file(argv[i], &total);
        }

        //Print totals only when more than one file was given
        if(file_count>1){
            print_counts(&total, "total");
        }
    }

    return (errors>0)?1:0;
}