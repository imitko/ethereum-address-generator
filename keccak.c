#include "keccak-tiny.c"
#include <getopt.h>

int 
main (int argc, char ** argv)
{
#define ADDR_SZ 20
#define ADDR_OFFS 12
  unsigned char in[1024];
  unsigned char out[32]; 
  unsigned char ck[ADDR_SZ * 2 + 1], addr[ADDR_SZ * 2 + 1];
  size_t rt = 0;
  int opt = 0, enc = 0;
  while ( (opt = getopt(argc, argv, "e")) != -1) 
    {   
      switch (opt)
        {   
          case 'e':
              enc = 1;
              break;
        }   
    }   

  while (!feof(stdin))
    {
      rt += fread(in, 1, sizeof (in), stdin);
      if (rt >= sizeof (in))
        break;
    }
  keccak_256 (out, sizeof (out), in, rt);
  if (!enc)
    fwrite (out, sizeof (out), 1, stdout);
  else
    {
      int i;
      addr[0] = 0; ck[0] = 0;
      for (i = 0; i < ADDR_SZ; i ++)
        snprintf ((char *)addr + (i*2), 3, "%02x", out[i+ADDR_OFFS]);
      keccak_256 (out, sizeof (out), addr, strlen((char *)addr));
      for (i = 0; i < ADDR_SZ; i ++)
        snprintf ((char *)ck + (i * 2), 3, "%02x", out[i]);
      for (i = 0; i < ADDR_SZ * 2; i++)
        {
          if (ck[i] >= 0x38 && addr[i] > 0x39)
            addr[i] = addr[i] - 0x20;
        }
      printf ("0x%s", addr);
    }
  return 0;
}
