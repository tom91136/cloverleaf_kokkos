
// @brief Driver for chunk initialisation.
// @author Wayne Gaudin
// @details Invokes the user specified chunk initialisation kernel.

#include "initialise_chunk.h"

void initialise_chunk(const int tile, global_variables& globals) {

  double dx = (globals.grid.xmax-globals.grid.xmin)/(double)(globals.grid.x_cells);
  double dy = (globals.grid.ymax-globals.grid.ymin)/(double)(globals.grid.y_cells);

  double xmin = globals.grid.xmin+dx*(double)(globals.chunk.tiles[tile].t_left-1);

  double ymin = globals.grid.ymin+dy*(double)(globals.chunk.tiles[tile].t_bottom-1);

////    CALL initialise_chunk_kernel(chunk%tiles(tile)%t_xmin,    &
 //     chunk%tiles(tile)%t_xmax,    &
 //     chunk%tiles(tile)%t_ymin,    &
 //     chunk%tiles(tile)%t_ymax,    &
 //     xmin,ymin,dx,dy,              &
 //     chunk%tiles(tile)%field%vertexx,  &
 //     chunk%tiles(tile)%field%vertexdx, &
 //     chunk%tiles(tile)%field%vertexy,  &
 //     chunk%tiles(tile)%field%vertexdy, &
 //     chunk%tiles(tile)%field%cellx,    &
 //     chunk%tiles(tile)%field%celldx,   &
 //     chunk%tiles(tile)%field%celly,    &
 //     chunk%tiles(tile)%field%celldy,   &
 //     chunk%tiles(tile)%field%volume,   &
 //     chunk%tiles(tile)%field%xarea,    &
 //     chunk%tiles(tile)%field%yarea     )

  const int x_min = globals.chunk.tiles[tile].t_xmin;
  const int x_max = globals.chunk.tiles[tile].t_xmax;
  const int y_min = globals.chunk.tiles[tile].t_ymin;
  const int y_max = globals.chunk.tiles[tile].t_ymax;

  size_t xrange = (x_max+3) - (x_min-2) + 1;
  size_t yrange = (y_max+3) - (y_min-2) + 1;

  Kokkos::parallel_for(xrange, KOKKOS_LAMBDA (const int j) {
    globals.chunk.tiles[tile].field.vertexx(j) = xmin + dx*(double)(j-x_min);
    globals.chunk.tiles[tile].field.vertexdx(j) = dx;
  });

  Kokkos::parallel_for(yrange, KOKKOS_LAMBDA (const int k) {
    globals.chunk.tiles[tile].field.vertexy(k) = ymin + dy*(double)(k-y_min);
    globals.chunk.tiles[tile].field.vertexdy(k) = dy;
  });

  xrange = (x_max+2) - (x_min-2) + 1;
  yrange = (y_max+2) - (y_min-2) + 1;

  Kokkos::parallel_for(xrange, KOKKOS_LAMBDA (const int j) {
    globals.chunk.tiles[tile].field.cellx(j) = 0.5*(globals.chunk.tiles[tile].field.vertexx(j) + globals.chunk.tiles[tile].field.vertexx(j+1));
    globals.chunk.tiles[tile].field.celldx(j) = dx;
  });

  Kokkos::parallel_for(yrange, KOKKOS_LAMBDA (const int k) {
    globals.chunk.tiles[tile].field.celly(k) = 0.5*(globals.chunk.tiles[tile].field.vertexy(k) + globals.chunk.tiles[tile].field.vertexy(k+1));
    globals.chunk.tiles[tile].field.celldy(k) = dy;
  });

  Kokkos::parallel_for(Kokkos::MDRangePolicy<Kokkos::Rank<2>>({0,0}, {xrange, yrange}), KOKKOS_LAMBDA (const int j, const int k) {
    globals.chunk.tiles[tile].field.volume(j,k) = dx*dy;
    globals.chunk.tiles[tile].field.xarea(j,k) = globals.chunk.tiles[tile].field.celldy(k);
    globals.chunk.tiles[tile].field.yarea(j,k) = globals.chunk.tiles[tile].field.celldx(j);
  });
}

