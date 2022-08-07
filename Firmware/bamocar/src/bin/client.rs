use bamocar::*;
use serialport;
use std::io::{Read};

fn main() {
    env_logger::init();
    let slave = get_slave_from_config();
    let mut port = serialport::new(slave.to_string_lossy(), 9600).open().expect(&format!("failed to open {:?}", slave));
    port.set_timeout(std::time::Duration::from_millis(1000)).unwrap();
    log::debug!("polling various endpoints at 1 Hz");
    loop {
        let endpoint = &[rand::random::<u8>()];
        log::debug!("TX: {:?}", endpoint);
        port.write(endpoint).unwrap();
        read_port_and_print(&mut port);
        std::thread::sleep(std::time::Duration::from_millis(1000));
    }
}

fn read_port(port:&mut std::boxed::Box<dyn serialport::SerialPort>, buf: &mut Vec<u8>) -> String {
    let n = port.read(&mut buf[..]).unwrap();
    std::str::from_utf8_mut(&mut buf[..n]).unwrap().to_string()
}

fn read_port_and_print(port: &mut std::boxed::Box<dyn serialport::SerialPort>) {
    let mut buf: Vec<u8> = vec![0; 1024];
    log::debug!("RX: {:?}", read_port(port, &mut buf));
}
