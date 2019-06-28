
//  @brief Fortran mpi buffer packing kernel
//  @author Wayne Gaudin
//  @details Packs/unpacks mpi send and receive buffers

#include "pack_kernel.h"

void clover_pack_message_left(int x_min, int x_max, int y_min, int y_max,
  Kokkos::View<double**>& field, Kokkos::View<double*>& left_snd_buffer,
  int cell_data, int vertex_data, int x_face_data, int y_face_data,
  int depth, int field_type, int buffer_offset) {

  // Pack

  int x_inc, y_inc;

  // These array modifications still need to be added on, plus the donor data location changes as in update_halo
  if (field_type == cell_data) {
    x_inc = 0;
    y_inc = 0;
  }
  if (field_type == vertex_data) {
    x_inc = 1;
    y_inc = 1;
  }
  if (field_type == x_face_data) {
    x_inc = 1;
    y_inc = 0;
  }
  if (field_type == y_face_data) {
    x_inc = 0;
    y_inc = 1;
  }

  Kokkos::RangePolicy<> range(y_min-depth, y_max+y_inc+depth+1);
  Kokkos::parallel_for("clover_pack_message_left", range, KOKKOS_LAMBDA (const int k) {
    for (int j = 1; j <= depth; ++j) {
      int index = buffer_offset + j + (k+depth-1) * depth;
      left_snd_buffer(index) = field(x_min+x_inc-1+j,k);
    }
  });

}

