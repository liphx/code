# nl - number lines of files

用法 `nl [OPTION]... [FILE]...`

选项

```
-b          # 根据如下STYLE计算行号
    a       # 每行前均列出行号 
    t       # 非空行前列出行号，默认值
    n       # 不列出行号
    pBRE    # 在包含正则表达式BRE的行前列出行号

-n          # 根据如下FORMAT列出行号
    ln      # 左对齐，没有前导0
    rn      # 右对齐，没有前导0，默认值
    rz      # 右对齐，有前导0

-w          # 前导0占用的位数，默认为6
```
