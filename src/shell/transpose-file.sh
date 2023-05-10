# https://leetcode.cn/problems/transpose-file/
# 给定一个文件 file.txt，转置它的内容。
# 你可以假设每行列数相同，并且每个字段由 ' ' 分隔。

# name age
# alice 21
# ryan 30

# =>

# name alice ryan
# age 21 30

# Read from the file file.txt and print its transposed content to stdout.
n=`awk '{print NF}' file.txt | uniq`
for i in `seq 1 $n`; do
    awk "{print \$$i}" file.txt | tr '\n' ' ' | sed 's/ $//g'
    echo
done
