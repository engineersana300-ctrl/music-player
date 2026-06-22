CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.
LDFLAGS = 
TARGET = music_player

SRCS = main.cpp \
       Application.cpp \
       MusicLibrary.cpp \
       Player.cpp \
       song.cpp \
       CSVLoader.cpp \
       M3uLoader.cpp \
       Playlist.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
