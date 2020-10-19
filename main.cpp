#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "automaton.h"

int main (int argc, char *argv[])
{
  if (argc != 5 && argc != 3)
    {
      fprintf (stderr, "Using: %s <command file> <input states file> [flag] [milliseconds]\n", argv[0]);
      return 1;
    }

  cellular_automaton CA;

  FILE *fp;
  if (!(fp = fopen (argv[1], "r")))
    {
      fprintf (stderr, "Cannot open %s!\n", argv[1]);
      return 1;
    }
  if (!CA.read_commands_from_file (fp))
    {
      fprintf (stderr, "Cannot read %s!\n", argv[1]);
      fclose (fp);
      return 1;
    }
  fclose (fp);
  //CA.print_commands ();

  if (!(fp = fopen (argv[2], "r")))
    {
      fprintf (stderr, "Cannot open %s!\n", argv[2]);
      return 1;
    }
  if (!CA.read_states_from_file (fp))
    {
      fprintf (stderr, "Cannot read %s!\n", argv[1]);
      fclose (fp);
      return 1;
    }
  fclose (fp);

  bool run_to_halt = false;
  int millisec = 500;
  if (argc == 5)
    {
      std::string flag = argv[3];
      if ((flag == "-r") || (flag == "--run"))
        {
          run_to_halt = true;
          millisec = atoi (argv[4]);
        }
      else
        {
          fprintf (stderr, "Unknown flag \'%s\'!\n", flag.c_str ());
          return 1;
        }
    }

  CA.run (!run_to_halt, millisec * 1000);

  return 0;
}
