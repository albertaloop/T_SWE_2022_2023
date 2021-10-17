use bamocar::*;
use std::io::{Read};
use serialport;

fn main() {
    let slave = get_slave_from_config();
    let mut port = serialport::new(slave.to_string_lossy(), 9600).open().expect(&format!("Failed to open {:?}", slave));
    port.set_timeout(std::time::Duration::from_millis(1000)).unwrap();
    port.write(b"a").unwrap();
    read_port_and_print(&mut port);
    port.write(b"b").unwrap();
    read_port_and_print(&mut port);
    port.write(b"c").unwrap();
    read_port_and_print(&mut port);
}

fn read_port(port:&mut std::boxed::Box<dyn serialport::SerialPort>, buf: &mut Vec<u8>) -> String {
    let n = port.read(&mut buf[..]).unwrap();
    std::str::from_utf8_mut(&mut buf[..n]).unwrap().to_string()
}

fn read_port_and_print(port: &mut std::boxed::Box<dyn serialport::SerialPort>) {
    let mut buf: Vec<u8> = vec![0; 1024];
    println!("read: {:?}", read_port(port, &mut buf));
}