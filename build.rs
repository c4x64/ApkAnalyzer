fn main() {
    cc::Build::new()
        .cpp(true)
        .file("native/src/utils/math_utils.cpp")
        .compile("native_core");

    println!("cargo:rerun-if-changed=native/src/utils/math_utils.cpp");
}
