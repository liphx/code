- https://www.rust-lang.org/zh-CN/
- https://doc.rust-lang.org/book/
- https://kaisery.github.io/trpl-zh-cn/

- 高性能：速度、内存利用率高、没有运行时和垃圾回收
- 可靠性：丰富的类型系统和所有权模型保证了内存安全和线程安全、编译期就能够消除各种各样的错误
- 生产力：文档、工具、包管理器

install & update

```shell
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
. "$HOME/.cargo/env"
rustup update
```

Cargo

- cargo new
- cargo build
- cargo check
- cargo run
- cargo test
- cargo doc
- cargo publish
- cargo update

for vim

```
git clone https://github.com/rust-lang/rust.vim ~/.vim/pack/plugins/start/rust.vim
```

`cargo new hello-rust`

```
hello-rust
|- Cargo.toml
|- src
  |- main.rs
```

hello, world

```rust
fn main() {
    println!("Hello, world!");
}
```

```shell
$ rustc main.rs
$ ./main
Hello, world!
```

rustfmt 格式化rust源代码

rust缩进风格 4个空格

- `println!` 宏
- `println`  函数
