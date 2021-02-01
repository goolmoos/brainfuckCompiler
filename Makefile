SRC := $(wildcard ./*.cpp)
HDRS := $(wildcard ./*.hpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))

TARGET = brainfuckcompiler

all: $(TARGET)

$(TARGET): $(OBJ) $(HDRS)
	g++ -Wall -o3 $(OBJ) -o $@

./%.o: ./%cpp $(HDRS)
	g++ -Wall -o3 -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) *~
