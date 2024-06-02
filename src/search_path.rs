use std::fs;
use std::collections::VecDeque;

pub fn walk(path: &str, suffix: String, q_root: &mut VecDeque<String>) {
    let paths = fs::read_dir(path).unwrap();

    for path in paths.filter_map(|e| e.ok()) {
        //忽略以 .开头的文件
        if path.file_name().into_string().unwrap().starts_with('.') {
            continue;
        }

        for s in suffix.split_ascii_whitespace().into_iter() {
            if path.file_name().into_string().unwrap().ends_with(s) {
                let n = path.path().display().to_string();
                q_root.push_back(n);
            }
        }

        if path.file_type().unwrap().is_dir() {
            walk(path.path().display().to_string().as_str(), suffix.clone(), q_root);
        }
    }
}
