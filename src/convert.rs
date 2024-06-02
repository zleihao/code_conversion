use std::{
    fs,
    io::{Read, Write},
};

use crate::coding::file_is_gbk;

pub fn convert(path: &str) {
    //判断传入的文件是否是gbk
    if !file_is_gbk::is_file_gbk(path) {
        println!("file {} not is gbk!", path);
        return;
    }

    //创建备份文件
    let file_back = format!("{}.tmp", path);
    let mut tp = std::fs::File::options()
        .truncate(true)
        .write(true)
        .create(true)
        .open(file_back.clone())
        .unwrap();

    //打开文件
    let mut fp = match std::fs::File::open(path) {
        Err(_) => {
            println!("file {} open fail!", path);
            let _ = fs::remove_file(file_back.clone());
            return;
        }
        Ok(f) => f,
    };

    let mut buf = Vec::new();
    match fp.read_to_end(&mut buf) {
        Err(_) => {
            println!("file {} read fail", path);
            let _ = fs::remove_file(file_back.clone());
            return;
        }
        _ => (),
    };

    //将转码之后的内容写到新文件中
    let res = iconv::iconv(&buf, "gbk", "utf-8");
    let iconv = match res {
        Ok(d) => d,
        Err(_) => {
            println!("file {} Transcoding failure!", path);
            let _ = fs::remove_file(file_back.clone());
            return;
        }
    };

    let res = tp.write_all(&iconv);
    match res {
        Ok(_) => {
            //删除源文件
            let _ = fs::remove_file(path);
            let _ = fs::rename(file_back, path);
        }
        Err(_) => {
            println!("file {} Transcoding failure!", path);
            let _ = fs::remove_file(file_back.clone());
            return;
        }
    }
}
