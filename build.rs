fn main() {
    cc::Build::new()
        .cpp(true)
        .file("native/src/utils/math_utils.cpp")
        .file("native/src/parsers/dex_parser.cpp")
        .include("native/include")
        .compile("native_core");

    println!("cargo:rerun-if-changed=native/src/utils/math_utils.cpp");
    println!("cargo:rerun-if-changed=native/src/parsers/dex_parser.cpp");
    println!("cargo:rerun-if-changed=native/include/dex_header.hpp");
}
