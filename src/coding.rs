pub mod file_is_gbk {
    fn is_valid_gbk(bytes: &[u8]) -> bool {
        let mut i = 0;
        while i < bytes.len() {
            if bytes[i] <= 0x7F {
                // 单字节（ASCII字符）
                i += 1;
            } else if bytes[i] >= 0x81 && bytes[i] <= 0xFE {
                // 双字节字符
                if i + 1 >= bytes.len() {
                    return false;
                }
                let next_byte = bytes[i + 1];
                if (next_byte >= 0x40 && next_byte <= 0x7E)
                    || (next_byte >= 0x80 && next_byte <= 0xFE)
                {
                    i += 2;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
        true
    }
    use std::{fs, io::Read};

    pub fn is_file_gbk(file_name: &str) -> bool {
        let mut fp = fs::File::open(file_name).unwrap();
        let mut buf: Vec<u8> = Vec::new();

        match fp.read(&mut buf) {
            Ok(size) => {
                if size == 0 {
                    false
                } else {
                    return is_valid_gbk(&buf);
                }
            }
            Err(_) => {
                false
            }
        }
    }
}
