mod coding;
mod convert;
mod search_path;
use convert::convert;
use search_path::walk;
use std::collections::VecDeque;
use std::sync::{Arc, Mutex};
use std::{fs, thread};

fn main() {
    let argv = std::env::args().collect::<Vec<String>>();

    if 1 == argv.len() {
        print!("{}", "user err!\n".to_string());
        print!("{}", "1, Transcoding a single file\n".to_string());
        print!("{}", "    cargo run <file_name>\n".to_string());
        print!("{}", "2, Transfer in the form of a folder\n".to_string());
        print!(
            "{}",
            "    cargo run <file_path> <file_suffix_list>\n".to_string()
        );
        return;
    }

    if fs::metadata(argv[1].clone()).unwrap().is_file() {
        convert(argv[1].as_str());
    } else {
        if argv.len() < 2 {
            panic!("user err: {} <file path>", argv[0]);
        }

        //创建队列
        let mut q_root: VecDeque<String> = VecDeque::new();
        walk(&argv[1], argv[2].clone(), &mut q_root);
        /* 使用多线程 */
        let arc = Arc::new(Mutex::new(q_root));
        let mut tid = vec![];

        for _ in 0..10 {
            let q = Arc::clone(&arc);
            let handle = thread::spawn(move || {
                while let Some(t) = q.lock().unwrap().pop_front() {
                    convert(&t);
                }
            });
            tid.push(handle);
        }

        for t in tid {
            t.join().unwrap();
        }
        println!("{} 转码完成", argv[1]);
    }
}
