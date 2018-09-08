CXX = arm-none-linux-gnueabi-gcc

targets = Lab6_DJK

all: $(targets)

netapi.o: netapi.c 
	$(CXX) -c netapi.c
	
omapl138_gpiofuncs.o: omapl138_gpiofuncs.c 
	$(CXX) -c omapl138_gpiofuncs.c

Lab6_DJK.o: Lab6_DJK.c
	$(CXX) -c Lab6_DJK.c	 

Lab6_DJK: netapi.o omapl138_gpiofuncs.o Lab6_DJK.o
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ 
clean:
	rm -rf *.o $(targets)
  
