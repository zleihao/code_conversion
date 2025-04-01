use crate::coding::file_is_gbk::detect_encoding;
use encoding_rs::GBK;
use std::{
    fs,
    io::{Read, Write},
};

pub fn convert(path: &str) -> bool {
    //判断传入的文件是否是gbk
    match detect_encoding(path) {
        Ok(ok) => {
            if ok.starts_with("GB") {
                ()
            } else {
                println!("file {} not is gbk!, coding is {}", path, ok);
                return false;
            }
        }
        Err(_) => return false,
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
            return false;
        }
        Ok(f) => f,
    };

    let mut buf = Vec::new();
    match fp.read_to_end(&mut buf) {
        Err(_) => {
            println!("file {} read fail", path);
            let _ = fs::remove_file(file_back.clone());
            return false;
        }
        _ => (),
    };

    //将转码之后的内容写到新文件中
    let (cow, encoding, result) = GBK.decode(&buf);
    if result {
        println!("file {} Transcoding failure!", path);
        let _ = fs::remove_file(file_back.clone());
        return false;
    }

    let iconv = cow.into_owned().into_bytes();

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
            return false;
        }
    }
    true
}
