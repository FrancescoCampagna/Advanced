EASYLOCAL = easylocal-3
FLAGS = -std=c++11 -Wall -Wfatal-errors -O3 # -g
COMPOPTS = -I$(EASYLOCAL)/include $(FLAGS)
LINKOPTS = -lboost_program_options -pthread

SOURCE_FILES = VRP_Data.cc VRP_Basics.cc VRP_Helpers.cc  VRP_Main.cc
OBJECT_FILES = VRP_Data.o VRP_Basics.o VRP_Helpers.o VRP_Main.o
HEADER_FILES = VRP_Data.hh VRP_Basics.hh VRP_Helpers.hh  

vrp: $(OBJECT_FILES)
	g++ $(OBJECT_FILES) $(LINKOPTS) -o vrp

VRP_Data.o: VRP_Data.cc VRP_Data.hh
	g++ -c $(COMPOPTS) VRP_Data.cc

VRP_Basics.o: VRP_Basics.cc VRP_Basics.hh VRP_Data.hh
	g++ -c $(COMPOPTS) VRP_Basics.cc

VRP_Helpers.o: VRP_Helpers.cc VRP_Helpers.hh VRP_Basics.hh VRP_Data.hh
	g++ -c $(COMPOPTS) VRP_Helpers.cc

VRP_Main.o: VRP_Main.cc VRP_Helpers.hh VRP_Basics.hh VRP_Data.hh
	g++ -c $(COMPOPTS) VRP_Main.cc

clean:
	rm -f $(OBJECT_FILES) vrp

