
#ifndef COMMS_H
#define COMMS_H

#include "definitions.h"

// Structure to hold MPI rank information
struct parallel_ {

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
  parallel_();
};

void clover_abort();
void clover_barrier();

void clover_decompose(global_variables& globals, parallel_& parallel, int x_cells, int y_cells, int& left, int& right, int& bottom, int& top);
void clover_tile_decompose(global_variables& globals, int chunk_x_cells, int chunk_y_cells);
void clover_allocate_buffers(global_variables& globals, parallel_& parallel);
void clover_sum(double& value);
void clover_exchange(global_variables& globals, int fields[NUM_FIELDS], const int depth);
void clover_pack_left(global_variables& globals, int tile, int fields[NUM_FIELDS], int depth, int left_right_offset[NUM_FIELDS]);
void clover_send_recv_message_left(global_variables& globals, Kokkos::View<double*>& left_snd_buffer, Kokkos::View<double*>& left_rcv_buffer, int total_size, int tag_send, int tag_recv, int& req_send, int& req_recv);

#endif

