fn main() {
    let x: i32 = 10;
    println!("The value of x is: {}", x);

    greet("Neil");
}

fn greet(name: &str) {
    println!("Hello, {}!", name);
}

struct Person {
    name: String,
    age: u32,
}
