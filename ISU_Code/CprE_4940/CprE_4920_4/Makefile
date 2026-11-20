CXX = C:/School/ISU/CPRE/CprE_4920_01/mingw64/bin/g++.exe

CXXFLAGS = -std=c++17 -I"./SFML-3.0.2/include"
LDFLAGS = -L"./SFML-3.0.2/lib" -lsfml-graphics -lsfml-window -lsfml-system

SRCS = SaltBot_Navigation_Sim.cpp Navigation.cpp
OUT = SaltSim_New.exe

all: build

build:
	$(CXX) $(SRCS) -o $(OUT) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)