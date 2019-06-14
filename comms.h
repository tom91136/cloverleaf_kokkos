
#ifndef COMMS_H
#define COMMS_H

// Structure to hold MPI rank information
struct mpi_info {

  // MPI enabled?
  bool parallel;

  // Is current process the boss?
  bool boss;

  // Size of MPI communicator
  int max_task;

  // Rank number
  int task;

  // Rank of boss
  int boss_task;

  // Constructor, (replaces clover_init_comms())
  mpi_info();
};

#endif
