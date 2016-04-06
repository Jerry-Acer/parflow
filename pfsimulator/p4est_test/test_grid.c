#include <stdio.h>
#include <parflow.h>
#include <parflow_p4est.h>

int
main (int argc, char **argv)
{
  Grid               *grid;

  if (amps_Init (&argc, &argv)) {
    amps_Printf ("Error: amps_Init initalization failed\n");
    exit (1);
  }

  NewGlobals (argv[1]);

  amps_ThreadLocal (input_database) = IDB_NewDB (GlobalsInFileName);

  GlobalsNumProcsX = GetIntDefault ("Process.Topology.P", 1);
  GlobalsNumProcsY = GetIntDefault ("Process.Topology.Q", 1);
  GlobalsNumProcsZ = GetIntDefault ("Process.Topology.R", 1);

  GlobalsNumProcs = amps_Size (amps_CommWorld);

  GlobalsBackground = ReadBackground ();

  GlobalsUserGrid = ReadUserGrid ();

  SetBackgroundBounds (GlobalsBackground, GlobalsUserGrid);

  /*
   * Initialize sc and p{4,casc8}est library
   */
  sc_init (amps_CommWorld, 1, 1, NULL, SC_LP_DEFAULT);
  p4est_init (NULL, SC_LP_DEFAULT);

  grid = CreateGrid (GlobalsUserGrid);

  PrintGrid ("pfgrid", grid);
  FreeGrid (grid);

  sc_finalize ();

  IDB_FreeDB (amps_ThreadLocal (input_database));
  FreeGlobals ();
  amps_Finalize ();

  return 0;
}
