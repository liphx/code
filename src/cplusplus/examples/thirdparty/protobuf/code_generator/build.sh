g++ -std=c++20 main.cc -o myplugin -lprotobuf -lprotoc

# protoc --plugin=protoc-gen-NAME=path/to/mybinary --NAME_out=OUT_DIR
protoc --plugin=protoc-gen-myplugin=myplugin --myplugin_out=. app.proto
