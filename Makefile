default:
	g++ -std=c++14 -O3 -o workdb src/main.cpp src/allocator/pool.cpp src/column.cpp src/table.cpp src/printer.cpp src/reader.cpp src/row.cpp src/buffer.cpp
