#ifndef APP_BASE_H
#define APP_BASE_H

#ifndef __cplusplus
typedef enum {
        false = 0,
        true = 1
} bool;
#endif

#define ASSERT(clause) if( !clause) { fprintf(stderr, "Assertion '%s' failed at %s:%d\n", #clause, __FILE__, __LINE__ ); exit(-1); }

#endif //APP_BASE_H
