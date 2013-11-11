#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loader.h"
#include "initializer.h"
#include "parser.h"

void printHelp(){
    puts("Mensaplan application in C\n");
    puts("Usage:");
    puts("<executable> [--init] [-n|--next-day] [--not-colored] [-S -M -F] [-h|--help]\n");
    puts("\t[--init]          Select your Mensa from a list to set your config file");
    puts("\t[-n|--next-day]   Show the meal-plan of the next day");
    puts("\t[--not-colored]   Don't use colored descriptions");
    puts("\t[-S -M -F]        Show only the prices for student (-S), workers (-M) or stranger (-F)");
    puts("\t[-h|--help]       Show this help");
    puts("");
}

int main(int argc, char ** argv) {

    int pInit = 0, pNextDay = 0, pColored = 0;
    int pPrices[3] = { 0, 0, 0};  /* studs - co-workers - foreigners */
    int i = 1;
    for(; i < argc; i++){
        if(!strcmp("--init", argv[i]))      { pInit = 1;    continue; }
        if(!strcmp("--next-day", argv[i]))  { pNextDay = 1; continue; }
        if(!strcmp("-n", argv[i]))          { pNextDay = 1; continue; }
        if(!strcmp("--not-colored",argv[i])){ pColored = 1;  continue; }
        if(!strcmp("-S", argv[i]))          { pPrices[0] = 1; continue; }
        if(!strcmp("-M", argv[i]))          { pPrices[1] = 1; continue; }
        if(!strcmp("-F", argv[i]))          { pPrices[2] = 1; continue; }
        if(!strcmp("-h", argv[i]))          { printHelp(); return 0; }
        if(!strcmp("--help", argv[i]))      { printHelp(); return 0; }
        /* if we get here... unknown parameter was given... */
        printf("Unknown option: \"%s\"\n", argv[i]);
    }

    if(!pPrices[0] && !pPrices[1] && !pPrices[2]){
        pPrices[0] = 1;
        pPrices[1] = 1;
        pPrices[2] = 1;
    }

    /* read config file */
    char * cPath = getConfigPath();
    FILE * myConfig = fopen(cPath, "r");
    free(cPath);
    if(pInit || !myConfig){
        return initializer();
    } else {
        char * buffer = calloc(200,1);
        if(fgets(buffer, 200, myConfig)){
            parsePlan(buffer, pNextDay, pColored, pPrices);
        }
        free(buffer);
    }

    return 0;
}