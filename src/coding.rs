pub mod file_is_gbk {
    use chardet::detect;
    use std::fs;
    // use chardetng::EncodingDetector;
    use encoding_rs::{GBK, UTF_8};

    pub fn detect_encoding(file_path: &str) -> std::io::Result<String> {
        let content = fs::read(file_path)?;

        let chardet_result = detect(&content);

        let (_, _, gbk_errors) = GBK.decode(&content);
        let (_, _, utf8_errors) = UTF_8.decode(&content);

        let encoding_name = chardet_result.0;
        let confidence = chardet_result.1;

        if encoding_name.starts_with("GB") && confidence > 0.7 && gbk_errors && !utf8_errors {
            Ok("GBK".to_string())
        } else {
            Ok(encoding_name.to_string())
        }
    }

    pub fn is_file_gbk(file_name: &str) -> bool {
        match detect_encoding(file_name) {
            Ok(ok) => {
                if ok.starts_with("GB") {
                    true
                } else {
                    false
                }
            }
            Err(_) => false,
        }
    }
}
