#ifdef STANDARD
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#else
#include <my_global.h>
#include <my_sys.h>
#endif
#include <mysql.h>
#include <ctype.h>
#include <gmp.h>
#include "paillier.h"
//static pthread_mutex_t LOCK_hostname;

char public_key[128] =  "30217388200631950129090459934004151318189944848301345317254019538698525090449";
//char public_key[1024] = "7";
mpz_t integ;

my_bool Sum_he_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  mpz_init(integ);
  mpz_set_str(integ, public_key, 10);
  char *i = (char *)malloc(sizeof(char) * 128);
  *i = '0';
  initid->ptr = i;
  if (args->arg_count != 1)
    {
      strcpy(message, "SUM_HE() requires one argument");
      return 1;
    }
  if (args->arg_type[0] != STRING_RESULT)
    {
      strcpy(message, "SUM_HE() requires a string");
      return 1;
    }
  return 0;
}

void Sum_he_deinit(UDF_INIT *initid)
{
  free(initid->ptr);
  mpz_clear(integ);
}

void Sum_he_clear(UDF_INIT *initid, char *is_null, char *error)
{
  initid->ptr[0] = '\0';
}

void Sum_he_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
  mpz_t rem;
  mpz_t prod;
  mpz_t rop;
  mpz_t rop2;
//  mpz_t i;
 mpz_init(rem);
  mpz_init(prod);
  mpz_init(rop);
  mpz_init(rop2);
 // mpz_init(i);
  mpz_set_str(rop, initid->ptr, 10);
  mpz_set_str(rop2, args->args[0], 10);
  mpz_mul(prod, rop, rop2);
  signed long int i = 0;
 // if (mpz_cmp_si(prod, i) == 0) mpz_set(prod, rop2);
  mpz_mod(rem, prod, integ);
  if (mpz_cmp_si(prod, i) == 0) mpz_set(rem, rop2);
  mpz_get_str(initid->ptr, 10, rem);
  mpz_clear(rem);
  mpz_clear(prod);
  mpz_clear(rop);
  mpz_clear(rop2);
//  *((long int*) initid->ptr) = 10;
  //*(initid->ptr) = *((long long int*)initid->ptr) * *((long long int*)args->args[0]);
  //*((long long int*)initid->ptr) = *((long long int*)initid->ptr) % public_key);
  //if (*((long long int*)initid->ptr) == 0) *((long long int*)initid->ptr) = *((long long int*)args->args[0]);
}

char *Sum_he(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error)
{
  unsigned long l = 0;
  char *trav = initid->ptr;
  while (isdigit(*trav)) {
    l++;
  trav++;
}
  memcpy(result, initid->ptr, l+1);
 // result = initid->ptr;
 // *length = sizeof(char) * (l + 1);
  *length = (l + 1);
//  mpz_clear(integ);
return initid->ptr;
}
