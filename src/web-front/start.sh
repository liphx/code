set -x

killall nginx
rm -rf .www

mkdir -p .www/logs
cp -r nginx.conf record animal-crossing .www

cd .www
nginx -p . -c nginx.conf
