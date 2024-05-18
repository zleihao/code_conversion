##  简介

此项目是将文件编码由 **GBK** 转成 **UTF-8**，目前只支持由编码 **GBK** 或 **GB2312** 或 **GB18030** 转到 **UTF-8**



##  编译

```bash
git clone git@github.com:zleihao/code_conversion.git
cd code_conversion
cmake -S . -B build
cd build
make
```

上述指令执行完后，会在目录 **build** 下生成可执行文件：**code_conversion**



##  用法

````bash
./build/code_conversion <file_path>
````

file_path 可以是一个文件，也可以是一个文件夹，若是文件夹，则会把文件夹内的所有 **.c**、**.h** 编码由 **GBK** 转成 **UTF-8**，目前暂不支持指定文件类型转换，只支持 **.c ** 、**.h** 。