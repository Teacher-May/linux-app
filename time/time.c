#include<stdio.h>
#include<time.h>

int main(void)
{
    time_t now ;
    struct tm *tm_now ;
    
    time(&now) ;
    
    tm_now = localtime(&now) ;//get date

    printf("datetime: %d-%d-%d %d:%d:%d\n",
        tm_now->tm_year + 1900,
        tm_now->tm_mon + 1, 
        tm_now->tm_mday, 
        tm_now->tm_hour, 
        tm_now->tm_min,
        tm_now->tm_sec);

        
    return 0;
}

