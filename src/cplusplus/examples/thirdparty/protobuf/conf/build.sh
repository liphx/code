protoc conf.proto --cpp_out=.
g++ -std=c++20 -c conf.pb.cc
# g++ -std=c++20 write.cpp conf.pb.o -o write -lprotobuf -labsl_log_internal_message -labsl_log_internal_check_op
# g++ -std=c++20 read.cpp conf.pb.o -o read -lprotobuf -labsl_log_internal_message -labsl_log_internal_check_op
g++ -std=c++20 read.cpp conf.pb.o -o read -lprotobuf
g++ -std=c++20 write.cpp conf.pb.o -o write -lprotobuf
