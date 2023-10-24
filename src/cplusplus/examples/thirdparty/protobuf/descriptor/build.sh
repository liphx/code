protoc app.proto --cpp_out=.
g++ -std=c++20 -c app.pb.cc
g++ -std=c++20 main.cc app.pb.o -o main -lprotobuf
