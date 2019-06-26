
default: clover_leaf

include $(KOKKOS_PATH)/Makefile.kokkos

CXX = mpic++

OBJ = build_field.o clover_leaf.o comms.o generate_chunk.o ideal_gas.o initialise.o initialise_chunk.o read_input.o report.o start.o

clover_leaf: $(OBJ) $(KOKKOS_CPP_DEPENDS)
	$(CXX) $(KOKKOS_LDFLAGS) -O3 $(OPTIONS) $(OBJ) $(KOKKOS_LIBS) -o $@

%.o: %.cpp
	$(CXX) $(KOKKOS_CPPFLAGS) $(KOKKOS_CXXFLAGS) -O3 $(OPTIONS) -c $<

.PHONY: clean
clean:
	rm -f clover_leaf $(OBJ)

