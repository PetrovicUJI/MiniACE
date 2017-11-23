/**
 * \file  my_library_header.h
 * \brief Header for user created library functions.
 */
#define DAY iteration_loop
#define WEEK 5
#define MONTH 20
#define YEAR 240

#define MAX_TIME_WINDOW 100

#define max(A,B) ( (A) > (B) ? (A):(B))
#define min(A,B) ( (A) < (B) ? (A):(B))


int random_int(int min,int max);

/* A sample from a normally distribution with zero mean and standard deviation 1.0.
 */
double fast_gauss_distro();

int Idle();