
# we don't need to build the python version of ssq
TARGET_APP=SIM
SIM: $(patsubst %.cpp,%.o,$(wildcard *.cpp))