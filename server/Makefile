# Makefile for asio_protobuf_samples
# Expects protobuf and boost to be installed 
#

PROTOBUF_CFLAGS = `pkg-config --cflags protobuf`
PROTOBUF_LFLAGS = `pkg-config --libs protobuf` 
BOOST_LFLAGS = -lboost_system -lboost_system -lboost_thread
CXXFLAGS+=-c -std=c++11 -Wall -pedantic

server_main: server_main.o CommunicationMgr.o TestbeamConnection.o Messages.pb.o MessageHandler.o
	g++ -o server_main server_main.o TestbeamConnection.o CommunicationMgr.o MessageHandler.o Messages.pb.o \
 	$(PROTOBUF_LFLAGS) $(BOOST_LFLAGS)	

Messages.pb.o: ../common/Messages.proto
	protoc --proto_path=../common/ --cpp_out=. --python_out=. ../common/Messages.proto
	g++ -c Messages.pb.cc $(PROTOBUF_CFLAGS)  $(CXXFLAGS) 


HEADERS = $(wildcard *.h)

%.o: %.cpp Messages.pb.o ${HEADERS}
	g++ $(CXXFLAGS) -c $<

clean:
	rm -f *.pb.*
	rm -f *_pb2.*
	rm -f *.o 
	rm -f server_main 
