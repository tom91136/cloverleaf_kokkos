
#include "initialise.h"
#include "report.h"
#include "version.h"

#include <fstream>

void initialise(struct parallel_ &parallel) {

  if (parallel.boss) {
    std::ofstream of;
    of.open("clover.out");
    if (!of.is_open())
      report_error("initialise", "Error opening clover.out file.");

    g_out.rdbuf(of.rdbuf());

  } else {
    g_out.rdbuf(std::cout.rdbuf());
  }


  if (parallel.boss) {
    g_out << "Clover Version " << g_version << std::endl
      << "Kokkos Version" << std::endl
      << "Task Count " << parallel.max_task << std::endl
      << std::endl;

    std::cout << "Output file clover.out opened. All output will go there." << std::endl;
  }

  clover_barrier();

  if (parallel.boss) {
    g_out << "Clover will run from the following input:-" << std::endl
      << std::endl;

    // Try to open clover.in
    std::ifstream g_in("clover.in");
    if (!g_in.good()) {
      g_in.close();
      std::ofstream out_unit("clover.in");
      out_unit
        << "*clover" << std::endl
        << " state 1 density=0.2 energy=1.0" << std::endl
        << " state 2 density=1.0 energy=2.5 geometry=rectangle xmin=0.0 xmax=5.0 ymin=0.0 ymax=2.0" << std::endl
        << " x_cells=10" << std::endl
        << " y_cells=2" << std::endl
        << " xmin=0.0" << std::endl
        << " ymin=0.0" << std::endl
        << " xmax=10.0" << std::endl
        << " ymax=2.0" << std::endl
        << " initial_timestep=0.04" << std::endl
        << " timestep_rise=1.5" << std::endl
        << " max_timestep=0.04" << std::endl
        << " end_time=3.0" << std::endl
        << " test_problem 1" << std::endl
        << "*endclover" << std::endl;
      out_unit.close();
      g_in.open("clover.in");
    }
  }

  clover_barrier();

  if (parallel.boss) {
    g_out << std::endl
      << "Initialising and generating" << std::endl
      << std::endl;
  }

/*
  CALL read_input()

  CALL clover_barrier

  step=0

  CALL start

  CALL clover_barrier

  IF(parallel%boss)THEN
    WRITE(g_out,*) 'Starting the calculation'
  ENDIF

  CLOSE(g_in)
  */

}

