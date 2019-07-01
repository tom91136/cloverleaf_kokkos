
#include "flux_calc.h"
#include "timer.h"


//  @brief Fortran flux kernel.
//  @author Wayne Gaudin
//  @details The edge volume fluxes are calculated based on the velocity fields.
void flux_calc_kernel(
  int x_min, int x_max, int y_min, int y_max,
  double dt,
  Kokkos::View<double**>& xarea,
  Kokkos::View<double**>& yarea,
  Kokkos::View<double**>& xvel0,
  Kokkos::View<double**>& yvel0,
  Kokkos::View<double**>& xvel1,
  Kokkos::View<double**>& yvel1,
  Kokkos::View<double**>& vol_flux_x,
  Kokkos::View<double**>& vol_flux_y) {

  Kokkos::MDRangePolicy<Kokkos::Rank<2>> policy({x_min, y_min}, {x_max+1, y_max+1});

  // Note that the loops calculate one extra flux than required, but this
  // allows loop fusion that improves performance
  Kokkos::parallel_for("flux_calc", policy, KOKKOS_LAMBDA (const int j, const int k) {

    vol_flux_x(j,k)=0.25*dt*xarea(j,k)
      *(xvel0(j,k)+xvel0(j,k+1)+xvel1(j,k)+xvel1(j,k+1));
    vol_flux_y(j,k)=0.25*dt*yarea(j,k)
      *(yvel0(j,k)+yvel0(j+1,k)+yvel1(j,k)+yvel1(j+1,k));

  });

}

// @brief Driver for the flux kernels
// @author Wayne Gaudin
// @details Invokes the used specified flux kernel
void flux_calc(global_variables& globals) {

  double kernel_time;
  if (globals.profiler_on) kernel_time = timer();


  for (int tile=0; tile < globals.tiles_per_chunk; ++tile) {

    flux_calc_kernel(
      globals.chunk.tiles[tile].t_xmin,
      globals.chunk.tiles[tile].t_xmax,
      globals.chunk.tiles[tile].t_ymin,
      globals.chunk.tiles[tile].t_ymax,
      globals.dt,
      globals.chunk.tiles[tile].field.xarea,
      globals.chunk.tiles[tile].field.yarea,
      globals.chunk.tiles[tile].field.xvel0,
      globals.chunk.tiles[tile].field.yvel0,
      globals.chunk.tiles[tile].field.xvel1,
      globals.chunk.tiles[tile].field.yvel1,
      globals.chunk.tiles[tile].field.vol_flux_x,
      globals.chunk.tiles[tile].field.vol_flux_y);

  }

  if (globals.profiler_on) globals.profiler.flux += timer()-kernel_time;
  
}
