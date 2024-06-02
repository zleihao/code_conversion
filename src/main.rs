mod coding;
mod convert;
mod search_path;
use convert::convert;
use search_path::walk;
use std::collections::VecDeque;
use std::fs;

fn main() {
    let argv = std::env::args().collect::<Vec<String>>();

    if 1 == argv.len() {
        print!("{}", "user err!\n".to_string());
        print!("{}", "1, Transcoding a single file\n".to_string());
        print!("{}", "    cargo run <file_name>\n".to_string());
        print!("{}", "2, Transfer in the form of a folder\n".to_string());
        print!("{}", "    cargo run <file_path> <file_suffix_list>\n".to_string());
        return;
    }

    if fs::metadata(argv[1].clone()).unwrap().is_file() {
        convert(argv[1].as_str());
    } else {
        if argv.len() < 2 {
            panic!("user err: {} <file path>", argv[0]);
        }

        //创建队列
        let mut path: VecDeque<String> = VecDeque::new();

        walk(&argv[1], argv[2].clone(), &mut path);

        // 检查队列是否为空
        while !path.is_empty() {
            // 访问队列的第一个元素
            if let Some(front) = path.pop_front() {
                convert(&front);
                println!("file {} 转码成功", front);
            }
        }
    }
}
