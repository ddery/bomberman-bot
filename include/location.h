#ifndef _H_LOCATION
#define _H_LOCATION

typedef struct {
    int x,y;
} Location;

bool operator<(const Location& a, const Location& b);

#endif
