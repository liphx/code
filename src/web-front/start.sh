set -e
SERVER_ROOT=/home/liph/record
cd $SERVER_ROOT
nohup python3 -m http.server 9000 &
