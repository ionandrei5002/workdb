default:
	clang-3.9 -lstdc++ -std=c++14 -O3 -g -o workdb src/main.cpp src/allocator/pool.cpp src/column.cpp src/table.cpp src/printer.cpp src/reader.cpp src/row.cpp src/buffer.cpp src/rawbuffer.cpp src/writeraheadlogger.cpp -lsnappy
