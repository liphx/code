set -e

if [[ -z $1 ]]; then
    echo "$0 <PATH>"
    exit 1
fi

cd $1
for i in `ls`; do
    pushd $i
    git pull
    popd
done
