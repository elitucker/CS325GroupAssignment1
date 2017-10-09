TARGET = select
CC = c++ -std=c++11 -stdlib=libc++

$(TARGET): select.cpp
	$(CC) -o $(TARGET) select.cpp

clean:
	rm -f *.o
	rm -f $(TARGET)
