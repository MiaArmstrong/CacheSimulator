//Mia Armstrong
//CS 201 Assighment 5
//Filename: hw5.c
/* This program is a set assiciative cache simulator. */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
//#include <strings.h>
 
//function declarations
void print( int hit_count, int miss_count, int miss_time, int hit_time);

//structure for a line
typedef struct
{
   int time;
   int valid;
   unsigned int tag;
} Line;

//structure for a set: a pointer to an array of lines
typedef struct
{
   Line * lines;
}Set;

//structure for a cache; a pointer to an array of sets
typedef struct
{
   Set * sets;
}Cache;

int main(int argc, char * argv[])
{
   if(argc != 6)
   {
     fprintf(stderr, "Specify the parameters.\nhw5 <S> <E> <B> <H> <M>\nS = the number of sets\nE = the number of lines\nB = the block size in bytes\nH = the hit time in cycles\nM = the miss time in cycles\n");
              exit (1);
   }

   int S = atoi(argv[1]); //the number of sets
   int E = atoi(argv[2]); //the number of lines
   int B = atoi(argv[3]); //the block size in bytes
   int H = atoi(argv[4]); //the hit time in cycles
   int M = atoi(argv[5]); //the miss time in cycles
   int b = 0; //number of bits needed for byte offset
   int i = 0; //an index
   int j = 0; //an index
   int e = 0; //number of bits needed for the index
   unsigned int a = 0; //to read in address
   int empty = -1; //index of empty space
   int hit = 0; //for a hit
   int TSTAMP = 0; //value for last recently used
   int toEvict = 0; //keeps track of what to evict
   //counters
   int hit_count = 0;
   int miss_count = 0;
   int eviction_count = 0;
   

   i = B;
   while (i !=1)
   {
      i = i/2;
      ++b;
   }
   
   i = S * E;
   while (i !=1)
   {
      i = i/2;
      ++e;
   }

   
   


   //Allocate space for sets and for lines
   Cache cache;

   cache.sets = (Set *) malloc ( S * sizeof (Set));
   for (i=0; i<S; ++i)
   {
      cache.sets[i].lines = (Line *) malloc ( sizeof (Line));
   }

   while ( i != EOF )
   {
      //input address
      i = scanf(" %x", &a);
      if(i == EOF)
      {
         break;
      }
      if( i != 1)
      {
         fprintf(stderr, "Input Error.");
         break;
      }

      toEvict = 0;
      
      //calculate address tag and set index
      unsigned int addr_tag = a >> (e + b);
      unsigned int setid = a >> ( b);
      Set set = cache.sets[setid];
      int low = INT_MAX;

      for ( j=0; j<E; ++j)
      {   //look for hit
          if (set.lines[j].valid == 1)
          {
             if(set.lines[j].tag == addr_tag)
             {
                ++hit_count;
                hit = 1;
                set.lines[j].time = TSTAMP;
                ++TSTAMP;
             }
             //look for oldest for eviction
             else if (set.lines[j].time < low)
             {
                low = set.lines[j].time;
                toEvict = j;
             }
          }
          //if haven't found an empty, mark one that we found
          else if (empty == -1)
          {
             empty = j;
          }
      }
       
      //if there is a miss
      if ( hit != 1)
      {
         ++miss_count;
         //if there is an empty line
         if ( empty > -1)
         {
            set.lines[empty].valid = 1;
            set.lines[empty].tag = addr_tag;
            set.lines[empty].time = TSTAMP;
            ++TSTAMP;
         }
         //if the set is full, evict
         else if ( empty < 0)
         {
            set.lines[toEvict].tag = addr_tag;
            set.lines[toEvict].time = TSTAMP;
            ++TSTAMP;
            ++eviction_count;
          }
      }
      empty = -1;
      hit = 0;
      

    }
    
   print(hit_count, miss_count, M, H);
 
   return 0;
}

void print(int hit_count, int miss_count, int miss_time, int hit_time)
{
   
   printf( "Number of hits = %d\nNumber of misses = %d\n", hit_count, miss_count);

   double d;
   
   d = ((double) miss_count)/(hit_count + miss_count) * 100;
   
   printf( "Miss rate = %4.2f %%\n", d);

   int total = (miss_time * miss_count) + (hit_time * hit_count);

   printf( "Total cycles = %d\n", total);
   
   
}
