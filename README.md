##  简介

此项目是将文件编码由 **GBK** 转成 **UTF-8**，目前只支持由编码 **GBK** 或 **GB2312** 或 **GB18030** 转到 **UTF-8**



**`！！！转码之前记得备份一下，以免转码出错 ！！！`**



##  编译

```bash
git clone git@github.com:zleihao/code_conversion.git
cd code_conversion

cargo build
```

上述指令执行完后，会在目录 **target** 下生成可执行文件：**conver**



##  用法

本工具支持单个文件转码或者是整个文件夹下的文件进行转码，二者在使用时稍有不同。

- 单个文件进行转码

  ```bash
  ./target/debug/conver <file_path>
  ```

- 以文件夹形式进行转码

  ```bash
  ./target/debug/conver <file_path> <file_suffix_list>
  ```

  file_suffix_list表示的是文件的后缀，例如：.c、.h之类的。**传入多种类型时，不同类型之间用一个空格隔开，不要用一个以上。**

  假设想把一个文件夹下所有的 **.c .h .S** 文件由gbk编码转成utf-8只需执行下面的命令即可：

  ``` bash 
  ./build/code_conversion /project/src ".c .h .S"
  ```