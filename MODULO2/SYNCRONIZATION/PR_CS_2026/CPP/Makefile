CPP = g++ 

CPPFLAGS = -Wall -Wextra -std=c++11 -pthread

SOURCE1 = 1_producer_consumer.cpp 
SOURCE2 = 2_producer_consumer_fixed_v1.cpp
SOURCE3 = 3_producer_consumer_fixed_v2.cpp
SOURCE4 = 4_producer_consumer_fixed_Crono.cpp


all: prodcons prodconsFix1 prodconsFix2 prodconsFixCrono

prodcons: $(SOURCE1)
	$(CPP) $(CPPFLAGS) $(SOURCE1) -o $@

prodconsFix1: $(SOURCE2)
	$(CPP) $(CPPFLAGS) $(SOURCE2) -o $@

prodconsFix2: $(SOURCE3)
	$(CPP) $(CPPFLAGS) $(SOURCE3) -o $@

prodconsFixCrono: $(SOURCE4)
	$(CPP) $(CPPFLAGS) $(SOURCE4) -o $@	


clean:
	$(RM) all: prodcons prodconsFix1 prodconsFix2 prodconsFixCrono *.txt
